#ifndef __INFOFETCHER_H__
#define __INFOFETCHER_H__

#include "IPv4_addr.h"
#include "MAC_addr.h"

int get_my_ip_str(char *, IPv4_addr&);
int get_my_mac_str(char *, MAC_addr&);
// int get_my_gateway_str(char *, char *, int);
// int send_arp_packet(pcap_t *, char* , char* , char* , char* , unsigned short int);

#endif