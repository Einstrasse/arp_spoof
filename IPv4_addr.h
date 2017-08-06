#ifndef __IPV4_ADDR_H__
#define __IPV4_ADDR_H__

#include <cstdint>
#include <string>
#include <netinet/in.h>
using namespace std;

class IPv4_addr {
public:
	IPv4_addr() {}
	IPv4_addr(const char* str) { *this = str; }
	IPv4_addr(const string& str) { *this = str; }
	void clear() { _ip = 0; }
	void hex_dump();
	void ascii_dump();
	void write_mem(uint8_t *mem);
	void parse_mem(uint8_t *mem);
	void parse_mem(char *mem);
	bool is_equal(uint32_t val);
	bool is_equal(IPv4_addr addr);
	string to_string();
	char* to_string(char* mem, int buf_size);

	operator string() { return this->to_string(); }; // string = IPv4_addr case
	operator uint32_t() { return this->_ip; } // uint32_t = IPv4_addr case
	operator struct in_addr(); // struct in_addr = IPv4_addr case
	IPv4_addr& operator = (const IPv4_addr& rhs) { _ip = rhs._ip; return *this; }
	IPv4_addr& operator = (const string& rhs); // IPv4_addr = string case
	IPv4_addr& operator = (const uint32_t rhs) { _ip = rhs; return *this; } // IPv4_addr = uint32_t case
	IPv4_addr& operator = (const char* rhs);
	IPv4_addr& operator = (const struct in_addr rhs) { this->_ip = (uint32_t)rhs.s_addr; return *this; }
private:
	uint32_t _ip;
};

#endif