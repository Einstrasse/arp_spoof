#include "IPv4_addr.h"
#include <arpa/inet.h>
#include <iostream>
#include <string>
#include <cstdio>
#include <cstring>


using namespace std;

IPv4_addr& IPv4_addr::operator = (const char* rhs) {
	int ret = inet_pton(AF_INET, rhs, &_ip);
	switch(ret) {
		case 0:
		cout << "inet_pton return zero ip=" << rhs << endl;
		break;
		case 1:
		_ip = ntohl(_ip);
		break;
		default:
		cout << "inet_pton return " << ret << endl;
	}
	return *this;
}

IPv4_addr& IPv4_addr::operator = (const string& rhs) {

	return *this;
}

void IPv4_addr::hex_dump() {
	uint8_t hex[4];
	uint32_t __ip = htonl(_ip);
	memcpy(hex, &__ip, 4);
	printf("IPv4 address hex dump %02hhX.%02hhX.%02hhX.%02hhX\n", hex[0], hex[1], hex[2], hex[3]);
}

void IPv4_addr::ascii_dump() {
	uint8_t hex[4];
	uint32_t __ip = htonl(_ip);
	memcpy(hex, &__ip, 4);
	printf("IPv4 address ascii dump %hhu.%hhu.%hhu.%hhu\n", hex[0], hex[1], hex[2], hex[3]);	
}