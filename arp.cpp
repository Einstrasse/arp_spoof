#ifndef __ARP_CPP__
#define __ARP_CPP__

#include <netinet/if_ether.h>
#include <arpa/inet.h>
#include <pcap.h>
#include "arp.h"
#include <iostream>

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

int recv_arp_reply(pcap_t* handle, IPv4_addr sip, MAC_addr& ret) {
	struct pcap_pkthdr* header_ptr;
	const u_char *pkt_data;
	struct ether_header* eth_hdr;
	struct ether_arp* arp_hdr;
	while(true) {
		int status = pcap_next_ex(handle, &header_ptr, &pkt_data);
		if (status == 0) {
			//no pcaket
			continue;
		} else if (status == -1) {
			fprintf(stderr, "Fail to set buffer size on capture handle: %s\n", pcap_geterr(handle));
			return EXIT_FAILURE;
		} else if (status == -2) {
			fprintf(stderr, "Finishing read packet data from packet file\n");
			return EXIT_SUCCESS;
		}
		eth_hdr = (struct ether_header*)pkt_data;

		if (ntohs(eth_hdr->ether_type) == ETHERTYPE_ARP) {
			arp_hdr = (struct ether_arp*)(pkt_data + sizeof(struct ether_header));
		} else {
			//not arp protocol
			continue;
		}

		if (ntohs(arp_hdr->ea_hdr.ar_pro) != ETHERTYPE_IP) {
			//not IPv4
			continue;
		}
		if (ntohs(arp_hdr->ea_hdr.ar_op) != ARPOP_REPLY) {
			//not arp reply
			continue;
		}
		IPv4_addr tmp_ip;
		tmp_ip.parse_mem(arp_hdr->arp_spa);
		cout << "sender IP addr -> "; tmp_ip.ascii_dump();
		cout << endl;
		if ( !sip.is_equal( tmp_ip ) ) {
			//printf("ip is not matched\n");
			//Sender IP addr not matched
			continue;
		}
		ret.parse_mem(arp_hdr->arp_sha);
		return EXIT_SUCCESS;
	}
}

#endif