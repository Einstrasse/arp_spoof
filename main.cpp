#include <iostream>
#include <vector>
#include <string>
#include <cstdio>
#include <cstdlib>
#include "IPv4_addr.h"

using namespace std;
#define LOG_LEVEL_DEBUG 1
#define LOG_LEVEL_INFO 0
int main(int argc, char *argv[]) {
	char *log_level_str = getenv("LOG_LEVEL");
	int log_level = LOG_LEVEL_INFO;
	vector<string> sender_ips;
	vector<string> target_ips;
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
		for (int i=0; i < sender_ips.size(); i++) {
			cout << "sender_ip[" << i << "] - " << sender_ips[i] << endl;
		}
		for (int i=0; i < target_ips.size(); i++) {
			cout << "target_ip[" << i << "] - " << target_ips[i] << endl;
		}
	}

	test_ip = argv[2];
	test_ip.hex_dump();
	test_ip.ascii_dump();

}