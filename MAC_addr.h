#ifndef __MAC_ADDR_H__
#define __MAC_ADDR_H__

#include <cstdint>
#include <cstring>
#include <cstdio>
#include <string>
using namespace std;

class MAC_addr {
public:
	MAC_addr() { memset(_mac, 0, 6); }
	MAC_addr(const char* str) { *this = str; }
	// MAC_addr(const uint8_t *mac) { memcpy(_mac, mac, 6); }
	void clear() { memset(_mac, 0, 6); }
	void hex_dump();
	void write_mem(uint8_t *mem);

	MAC_addr& operator = (const MAC_addr& rhs) { memcpy(_mac, rhs._mac, 6); return *this; }
	//MAC_addr& operator = (const uint8_t *rhs) { memcpy(_mac, rhs, 6); return *this; }
	MAC_addr& operator = (const char *rhs);
	MAC_addr& operator = (const string& rhs);

private:
	uint8_t _mac[6];
};

#endif