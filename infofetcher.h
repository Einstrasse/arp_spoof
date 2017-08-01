int get_my_mac_str(char *, char *, int);
int get_my_ip_str(char *, char *, int);
int get_my_gateway_str(char *, char *, int);
int send_arp_packet(pcap_t *, char* , char* , char* , char* , unsigned short int);