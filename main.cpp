#include <iostream>
#include <cstdio>
#include <cstdlib>

#include "ui.h"
using namespace std;

int main(int argc, char *argv[]) {
	if (argc < 3) {
		fprintf(stderr, "Usage: ./%s <interface> <sender ip 1> <target ip 1> [<sender ip 2> <target ip 2>...]\n", argv[0]);
		return EXIT_FAILURE;
	}
}