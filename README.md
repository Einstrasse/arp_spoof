# arp_spoof
BoB 6기    
BoB 6th    
보안컨설팅트랙    
정한길    
ARP 스푸핑 및 패킷 릴레이 툴        

## 개발 및 테스트 환경
Ubuntu 16.04    

## 구현
C++ 언어, 패킷 캡쳐라이브러리 pcap
## 컴파일 시 필요사항
- pcap 라이브러리
- 리눅스 (데비안 계열 추천)
- g++

라이브러리 및 컴파일러 설치 커맨드
```
$ apt install -y libpcap-dev
$ apt install -y g++
```
## 컴파일 명령어
```
$ make
```
## 실행 명령어
실행 모드
```
$ ./arp_spoof <interface> <sender ip 1> <target ip 1> [<sender ip 2> <target ip 2>...]
```
개발 및 디버그 모드
```
$ LOG_LEVEL=1 ./arp_spoof <interface> <sender ip 1> <target ip 1> [<sender ip 2> <target ip 2>...]
```
환경변수 LOG_LEVEL을 1이상의 값으로 설정 시, 로그메시지가 나타나게 된다.

## 시스템의 IP주소, MAC주소 받아오기
IP주소 받아올 때 사용한 커맨드
```
/bin/bash -c 'ifconfig enp7s0' | grep 'inet ' | awk '{print $2}' | cut -d':' -f2
```
MAC주소 받아올 때 사용한 커맨드
```
/bin/bash -c "ifconfig enp7s0" | grep -oh '[0-9a-zA-Z][0-9a-zA-Z]:[0-9a-zA-Z][0-9a-zA-Z]:[0-9a-zA-Z][0-9a-zA-Z]:[0-9a-zA-Z][0-9a-zA-Z]:[0-9a-zA-Z][0-9a-zA-Z]:[0-9a-zA-Z][0-9a-zA-Z]'
```