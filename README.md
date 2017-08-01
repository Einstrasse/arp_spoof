# arp_spoof
BoB 6th    
Security Consulting Track    
Jung Han Gil    
arp spoofing and relay packet tool    

## Development and Test environment
Ubuntu 16.04    

## Implementation
C++ language with packet capture library
## Compilation Requirements
- pcap library
- Linux (Debian recommended)
- g++
```
$ apt install -y libpcap-dev
$ apt install -y g++
```
## Compilation Command
```
$ make
```
## Execution Command
```
$ ./arp_spoof <interface> <sender ip 1> <target ip 1> [<sender ip 2> <target ip 2>...]
```