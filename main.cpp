#include <iostream>
#include <vector>
#include <string>
#include <cstdio>
#include <cstdlib>
#include "IPv4_addr.h"
#include "infofetcher.h"
#include "arp.h"

using namespace std;

#define LOG_LEVEL_DEBUG 1
#define LOG_LEVEL_INFO 0
#define PCAP_ERR_BUF_SIZE 1024
#define PACK_BUF_SIZE 1024 * 64

int main(int argc, char *argv[]) {
	char *log_level_str = getenv("LOG_LEVEL");
	pcap_t *handle;
	int log_level = LOG_LEVEL_INFO;
	char errbuf[PCAP_ERR_BUF_SIZE];
	IPv4_addr my_ip_addr;
	MAC_addr my_mac_addr;
	vector<IPv4_addr> sender_ips;
	vector<IPv4_addr> target_ips;
	vector<MAC_addr> sender_macs;
	IPv4_addr test_ip;
	char *ifname;
	if (argc < 4) {
		fprintf(stderr, "Usage: %s <interface> <sender ip 1> <target ip 1> [<sender ip 2> <target ip 2>...]\n", argv[0]);
		fprintf(stderr, "\tto use debug mode, you can specify environment variable 'LOG_LEVEL'\n");
		fprintf(stderr, "\te.g) LOG_LEVEL=1 %s eth0 192.168.0.5 192.168.0.1\n", argv[0]);
		return EXIT_FAILURE;
	}
	ifname = argv[1];
	argc -= 2;
	for (int i = 2; i <= argc; i += 2) {
		sender_ips.push_back(argv[i]);
		target_ips.push_back(argv[i+1]);
	}

	if (log_level_str != NULL) {
		log_level=atoi(log_level_str);
	}

	if (log_level >= LOG_LEVEL_DEBUG) {
		for (u_int i=0; i < sender_ips.size(); i++) {
			// cout << "sender_ip[" << i << "] - " << sender_ips[i] << endl;
			cout << "sender_ip[" << i << "] - ";
			sender_ips[i].ascii_dump();
			cout << endl;
		}
		for (u_int i=0; i < target_ips.size(); i++) {
			// cout << "target_ip[" << i << "] - " << target_ips[i] << endl;
			cout << "target_ip[" << i << "] - ";
			target_ips[i].ascii_dump();
			cout << endl;
		}
	}

	get_my_ip_str(ifname, my_ip_addr);
	get_my_mac_str(ifname, my_mac_addr);
	if (log_level >= LOG_LEVEL_DEBUG) {
		cout << "My IPv4 address is ";
		my_ip_addr.ascii_dump();
		cout << endl;
		cout << "My MAC address is ";
		my_mac_addr.hex_dump();
		cout << endl;
	}

	handle = pcap_open_live(ifname, PACK_BUF_SIZE, 0, 1, errbuf);
	if (handle == NULL) {
		fprintf(stderr, "Interface open err %s: %s\n", ifname, errbuf);
		exit(EXIT_FAILURE);
	}
	if (log_level >= LOG_LEVEL_DEBUG) {
		printf("Interface open success %s\n", ifname);
	}

	
	cout << "my_mac_addr -> "; my_mac_addr.hex_dump();
	cout << endl << "my_ip_addr -> "; my_ip_addr.ascii_dump();
	cout << endl << "sender_ip -> "; sender_ips[0].ascii_dump();
	cout << endl;
	send_arp_request(handle, my_mac_addr, my_ip_addr, sender_ips[0]);

	MAC_addr tmp_mac;
	recv_arp_reply(handle, sender_ips[0], tmp_mac);
	cout << "mac got -> "; tmp_mac.hex_dump();
	cout << endl;
}