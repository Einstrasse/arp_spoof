#include <iostream>
#include <vector>
#include <string>
#include <cstdio>
#include <cstdlib>

using namespace std;

int main(int argc, char *argv[]) {

	vector<string> sender_ips;
	vector<string> target_ips;
	char *ifname;
	if (argc < 4) {
		fprintf(stderr, "Usage: %s <interface> <sender ip 1> <target ip 1> [<sender ip 2> <target ip 2>...]\n", argv[0]);
		return EXIT_FAILURE;
	}
	ifname = argv[1];
	argc -= 2;
	for (int i = 2; i <= argc; i += 2) {
		sender_ips.push_back(argv[i]);
		target_ips.push_back(argv[i+1]);
	}

	//dump ips
	for (int i=0; i < sender_ips.size(); i++) {
		VLOG(4) << "sender_ip[" << i << "] - " << sender_ips[i] << endl;
	}
	for (int i=0; i < target_ips.size(); i++) {
		VLOG(4) << "target_ip[" << i << "] - " << target_ips[i] << endl;
	}

}