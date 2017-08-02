#ifndef __IPV4_ADDR_CPP__
#define __IPV4_ADDR_CPP__

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
		cout << "inet_pton return zero ip='" << rhs << "'" << endl;
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
	const char *c = rhs.c_str();
	*this = c;
	return *this;
}

void IPv4_addr::hex_dump() {
	uint8_t hex[4];
	uint32_t __ip = htonl(_ip);
	memcpy(hex, &__ip, 4);
	printf("%02hhX.%02hhX.%02hhX.%02hhX", hex[0], hex[1], hex[2], hex[3]);
	// printf("IPv4 address hex dump %02hhX.%02hhX.%02hhX.%02hhX\n", hex[0], hex[1], hex[2], hex[3]);
}

void IPv4_addr::ascii_dump() {
	uint8_t hex[4];
	uint32_t __ip = htonl(_ip);
	memcpy(hex, &__ip, 4);
	printf("%hhu.%hhu.%hhu.%hhu", hex[0], hex[1], hex[2], hex[3]);	
	// printf("IPv4 address ascii dump %hhu.%hhu.%hhu.%hhu\n", hex[0], hex[1], hex[2], hex[3]);	
}

void IPv4_addr::write_mem(uint8_t *mem) {
	uint32_t __ip = htonl(_ip);
	memcpy(mem, &__ip, 4);
}
void IPv4_addr::parse_mem(uint8_t *mem) {
	memcpy(&_ip, mem, 4);
	_ip = ntohl(_ip);
}
void IPv4_addr::parse_mem(char *mem) {
	memcpy(&_ip, mem, 4);
	_ip = ntohl(_ip);
}

bool IPv4_addr::is_equal(uint32_t val) {
	return _ip == val;
}
bool IPv4_addr::is_equal(IPv4_addr addr) {
	return _ip == addr._ip;
}

#endif