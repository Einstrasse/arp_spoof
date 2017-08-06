#ifndef __IPV4_ADDR_H__
#define __IPV4_ADDR_H__

#include <cstdint>
#include <string>
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
	operator string() { return this->to_string(); };
	IPv4_addr& operator = (const IPv4_addr& rhs) { _ip = rhs._ip; return *this; }
	IPv4_addr& operator = (const uint32_t rhs) { _ip = rhs; return *this; }
	IPv4_addr& operator = (const char* rhs);
	IPv4_addr& operator = (const string& rhs);
private:
	uint32_t _ip;
};

#endif