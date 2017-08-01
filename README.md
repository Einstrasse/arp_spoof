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
Run mode
```
$ ./arp_spoof <interface> <sender ip 1> <target ip 1> [<sender ip 2> <target ip 2>...]
```
Development/Debug mode
```
$ LOG_LEVEL=1 ./arp_spoof <interface> <sender ip 1> <target ip 1> [<sender ip 2> <target ip 
```

## Fetching My IP / MAC addr
Fetching IPv4 addr cmd
```
/bin/bash -c 'ifconfig enp7s0' | grep 'inet ' | awk '{print $2}' | cut -d':' -f2
```
Fetching Mac addr cmd
```
/bin/bash -c "ifconfig enp7s0" | grep -oh '[0-9a-zA-Z][0-9a-zA-Z]:[0-9a-zA-Z][0-9a-zA-Z]:[0-9a-zA-Z][0-9a-zA-Z]:[0-9a-zA-Z][0-9a-zA-Z]:[0-9a-zA-Z][0-9a-zA-Z]:[0-9a-zA-Z][0-9a-zA-Z]'
```