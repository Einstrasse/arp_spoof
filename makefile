arp_spoof: main.o IPv4_addr.o infofetcher.o MAC_addr.o
	g++ main.o IPv4_addr.o infofetcher.o MAC_addr.o -o arp_spoof -std=c++11 -lpcap -Wall
main.o: main.cpp
	g++ -o main.o -c main.cpp -std=c++11 -lpcap -Wall 
IPv4_addr.o: IPv4_addr.h
	g++ -o IPv4_addr.o -c IPv4_addr.cpp -std=c++11 -Wall
infofetcher.o: infofetcher.cpp
	g++ -o infofetcher.o -c infofetcher.cpp -std=c++11 -Wall
MAC_addr.o: MAC_addr.cpp
	g++ -o MAC_addr.o -c MAC_addr.cpp -std=c++11 -Wall

clean:
	rm *.o arp_spoof