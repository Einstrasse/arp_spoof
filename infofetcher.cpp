#ifndef __INFOFETCHER_CPP__
#define __INFOFETCHER_CPP__

#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/if_ether.h>
#include "infofetcher.h"
#include <arpa/inet.h>
#include <string>
#include <iostream>

using namespace std;

#define CMD_BUF_SIZE 256
#define STDOUT_BUF_SIZE 256
// int get_my_ip_str(char *ifname, char *str, int len) {
int get_my_ip_str(char *ifname, IPv4_addr& ip_addr) {
	FILE* fp;
	char cmdbuf[CMD_BUF_SIZE];
	char stdout_buf[STDOUT_BUF_SIZE];
	sprintf(cmdbuf, "/bin/bash -c 'ifconfig %s' | grep 'inet ' | awk '{print $2}' | cut -d':' -f2", ifname);
	fp = popen(cmdbuf, "r");
	if (fp == NULL) {
		perror("Fail to fetch ip address\n");
		return EXIT_FAILURE;
	}
	fgets(stdout_buf, STDOUT_BUF_SIZE - 1, fp);
	pclose(fp);
	for (int i=0; i < STDOUT_BUF_SIZE; i++) {
		//slice ending new line character to handle processing error
		if (stdout_buf[i] == '\n') {
			stdout_buf[i] = '\0';
			break;
		}
	}
	ip_addr = stdout_buf;
	return EXIT_SUCCESS;
}
int get_my_mac_str(char *ifname, MAC_addr& mac_addr) {
	FILE* fp;
	char cmdbuf[CMD_BUF_SIZE];
	char stdout_buf[STDOUT_BUF_SIZE];
	sprintf(cmdbuf, "/bin/bash -c \"ifconfig %s\" | grep -oh '[0-9a-zA-Z][0-9a-zA-Z]:[0-9a-zA-Z][0-9a-zA-Z]:[0-9a-zA-Z][0-9a-zA-Z]:[0-9a-zA-Z][0-9a-zA-Z]:[0-9a-zA-Z][0-9a-zA-Z]:[0-9a-zA-Z][0-9a-zA-Z]'", ifname);
	fp = popen(cmdbuf, "r");
	if (fp == NULL) {
		perror("Fail to fetch MAC address\n");
		return EXIT_FAILURE;
	}
	fgets(stdout_buf, STDOUT_BUF_SIZE - 1, fp);
	pclose(fp);
	mac_addr = stdout_buf;
	return EXIT_SUCCESS;
}
// int get_my_gateway_str(char *ifname, char *str, int len) {
// 	FILE* fp;
// 	char cmdbuf[CMD_BUF_SIZE];
// 	sprintf(cmdbuf, "/bin/bash -c 'route -n' | grep G | grep %s | awk '{print $2}'", ifname);
// 	fp = popen(cmdbuf, "r");
// 	if (fp == NULL) {
// 		perror("Fail to fetch gateway address\n");
// 		return EXIT_FAILURE;
// 	}
// 	fgets(str, len, fp);
// 	pclose(fp);
// 	return EXIT_SUCCESS;
// }

// int send_arp_packet(pcap_t *handle, char* src_mac, char* dst_mac, char* src_ip, char* dst_ip, unsigned short int type) {
// 	struct ether_header* eth_hdr;
// 	struct ether_arp* arp_hdr;
// 	u_char send_buf[42];
// 	eth_hdr = (struct ether_header*)send_buf;
	
// 	if (type == ARPOP_REQUEST || dst_mac == NULL) {
// 		sscanf("ff:ff:ff:ff:ff:ff", "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &eth_hdr->ether_dhost[0], &eth_hdr->ether_dhost[1],
// 		 &eth_hdr->ether_dhost[2], &eth_hdr->ether_dhost[3], &eth_hdr->ether_dhost[4], &eth_hdr->ether_dhost[5]);
// 	} else {
// 		sscanf(dst_mac, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &eth_hdr->ether_dhost[0], &eth_hdr->ether_dhost[1],
// 		 &eth_hdr->ether_dhost[2], &eth_hdr->ether_dhost[3], &eth_hdr->ether_dhost[4], &eth_hdr->ether_dhost[5]);
// 	}

// 	sscanf(src_mac, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &eth_hdr->ether_shost[0], &eth_hdr->ether_shost[1], &eth_hdr->ether_shost[2], &eth_hdr->ether_shost[3], &eth_hdr->ether_shost[4], &eth_hdr->ether_shost[5]);
// 	eth_hdr->ether_type = ntohs(ETHERTYPE_ARP);
	
// 	arp_hdr = (struct ether_arp*)(send_buf + sizeof(struct ether_header));
// 	arp_hdr->ea_hdr.ar_hrd = ntohs(ARPHRD_ETHER);
// 	arp_hdr->ea_hdr.ar_pro = ntohs(ETHERTYPE_IP);
// 	arp_hdr->ea_hdr.ar_hln = 6;
// 	arp_hdr->ea_hdr.ar_pln = 4;
// 	arp_hdr->ea_hdr.ar_op = ntohs(type);

// 	sscanf(src_mac, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &arp_hdr->arp_sha[0], &arp_hdr->arp_sha[1], &arp_hdr->arp_sha[2], &arp_hdr->arp_sha[3], &arp_hdr->arp_sha[4], &arp_hdr->arp_sha[5]);
// 	sscanf(src_ip, "%hhd.%hhd.%hhd.%hhd", &arp_hdr->arp_spa[0], &arp_hdr->arp_spa[1], &arp_hdr->arp_spa[2], &arp_hdr->arp_spa[3]);
// 	if (type == ARPOP_REQUEST || dst_mac == NULL) {
// 		sscanf("00:00:00:00:00:00", "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &arp_hdr->arp_tha[0], &arp_hdr->arp_tha[1], &arp_hdr->arp_tha[2], &arp_hdr->arp_tha[3], &arp_hdr->arp_tha[4], &arp_hdr->arp_tha[5]);
// 	} else {
// 		sscanf(dst_mac, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &arp_hdr->arp_tha[0], &arp_hdr->arp_tha[1], &arp_hdr->arp_tha[2], &arp_hdr->arp_tha[3], &arp_hdr->arp_tha[4], &arp_hdr->arp_tha[5]);
// 	}
// 	sscanf(dst_ip, "%hhd.%hhd.%hhd.%hhd", &arp_hdr->arp_tpa[0], &arp_hdr->arp_tpa[1], &arp_hdr->arp_tpa[2], &arp_hdr->arp_tpa[3]);

// 	int pack_len = sizeof(struct ether_header) + sizeof(struct ether_arp);
// 	if (pcap_sendpacket(handle, send_buf, pack_len) == -1) {
// 		fprintf(stderr, "pcap_sendpacket err %s\n", pcap_geterr(handle));
// 		return EXIT_FAILURE;
// 	} else {
// 		return EXIT_SUCCESS;
// 	}
// }

#endif