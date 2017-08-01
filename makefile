arp_spoof: main.o
	g++ main.o -o arp_spoof -std=c++11 -lpcap -Wall
main.o: main.cpp
	g++ -o main.o -c main.cpp -std=c++11 -Wall