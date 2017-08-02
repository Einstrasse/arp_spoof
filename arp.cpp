#ifndef __ARP_CPP__
#define __ARP_CPP__

#include <netinet/if_ether.h>
#include <arpa/inet.h>
#include <pcap.h>
#include "arp.h"

#define ARP_PACK_LEN 42 // sizeof(struct ether_header) + sizeof(struct ether_arp)

int send_arp_request(pcap_t* handle, MAC_addr smac, IPv4_addr sip, IPv4_addr dip) {
	struct ether_header* eth_hdr;
	struct ether_arp* arp_hdr;
	u_char send_buf[ARP_PACK_LEN];
	eth_hdr = (struct ether_header*)send_buf;
	MAC_addr mac_addr = "ff:ff:ff:ff:ff:ff";
	mac_addr.write_mem(eth_hdr->ether_dhost);
	smac.write_mem(eth_hdr->ether_shost);
	eth_hdr->ether_type = ntohs(ETHERTYPE_ARP);

	arp_hdr = (struct ether_arp*)(send_buf + sizeof(struct ether_header));
	arp_hdr->ea_hdr.ar_hrd = ntohs(ARPHRD_ETHER);
	arp_hdr->ea_hdr.ar_pro = ntohs(ETHERTYPE_IP);
	arp_hdr->ea_hdr.ar_hln = 6;
	arp_hdr->ea_hdr.ar_pln = 4;
	arp_hdr->ea_hdr.ar_op = ntohs(ARPOP_REQUEST);

	smac.write_mem(arp_hdr->arp_sha);
	sip.write_mem(arp_hdr->arp_spa);
	mac_addr = "00:00:00:00:00:00";
	mac_addr.write_mem(arp_hdr->arp_tha);
	dip.write_mem(arp_hdr->arp_tpa);

	if (pcap_sendpacket(handle, send_buf, ARP_PACK_LEN) == -1) {
		fprintf(stderr, "pcap_sendpacket err %s\n", pcap_geterr(handle));
		return EXIT_FAILURE;
	} else {
		return EXIT_SUCCESS;
	}
}

int send_arp_reply(pcap_t* handle, MAC_addr smac, MAC_addr dmac, IPv4_addr sip, IPv4_addr dip) {
	struct ether_header* eth_hdr;
	struct ether_arp* arp_hdr;
	u_char send_buf[ARP_PACK_LEN];
	eth_hdr = (struct ether_header*)send_buf;
	dmac.write_mem(eth_hdr->ether_dhost);
	smac.write_mem(eth_hdr->ether_shost);
	eth_hdr->ether_type = ntohs(ETHERTYPE_ARP);

	arp_hdr = (struct ether_arp*)(send_buf + sizeof(struct ether_header));
	arp_hdr->ea_hdr.ar_hrd = ntohs(ARPHRD_ETHER);
	arp_hdr->ea_hdr.ar_pro = ntohs(ETHERTYPE_IP);
	arp_hdr->ea_hdr.ar_hln = 6;
	arp_hdr->ea_hdr.ar_pln = 4;
	arp_hdr->ea_hdr.ar_op = ntohs(ARPOP_REPLY);

	smac.write_mem(arp_hdr->arp_sha);
	sip.write_mem(arp_hdr->arp_spa);
	dmac.write_mem(arp_hdr->arp_tha);
	dip.write_mem(arp_hdr->arp_tpa);

	if (pcap_sendpacket(handle, send_buf, ARP_PACK_LEN) == -1) {
		fprintf(stderr, "pcap_sendpacket err %s\n", pcap_geterr(handle));
		return EXIT_FAILURE;
	} else {
		return EXIT_SUCCESS;
	}
}

#endif