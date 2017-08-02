#ifndef __ARP_H__
#define __ARP_H__

#include "MAC_addr.h"
#include "IPv4_addr.h"
#include <pcap.h>

int send_arp_request(pcap_t* handle, MAC_addr smac, IPv4_addr sip, IPv4_addr dip);
#endif