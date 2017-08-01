#ifndef __IP_H__
#define __IP_H__

#include <cstdint>
#include <string>
using namespace std;

class IPv4_addr {
public:
	IPv4_addr() {}
	void clear() { _ip = 0; }
	void hex_dump();
	void ascii_dump();

	IPv4_addr& operator = (const IPv4_addr& rhs) { _ip = rhs._ip; return *this; }
	IPv4_addr& operator = (const uint32_t rhs) { _ip = rhs; return *this; }
	IPv4_addr& operator = (const char* rhs);
	IPv4_addr& operator = (const string& rhs);
private:
	uint32_t _ip;
};
#endif