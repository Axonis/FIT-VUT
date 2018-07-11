# ISAMON
## Skener siete pre zobrazenie aktívnych zariadení, ich otvorených TCP a UDP portov 
Autor: Jozef Urbanovsky

Login: xurban66

Ročník: 3BIT

## Použitie
```
isamon [-h] [-i <interface>] [-t] [-u] [-p <port>] [-w <ms>] -n <net_address/mask> 
   -h --help -- zobrazí nápovedu 
   -i --interface <interface> -- rozhranie na kterom bude nástroj scanovať
   -n --network <net_address/mask> -- ip adresa siete s maskou definujúcou rozsah pre skenovanie
   -t --tcp -- použije TCP 
   -u --udp -- použije UDP 
   -p --port <port> -- špecifikácia skenovaného portu, ak nieje skenuje sa celý rozsah 
   -w --wait <ms> -- maximálne prípustné RTT
```

## Súbory

* isamon.cpp
* isamon.h
* manual.pdf
* README.md
* [src/]
* [doc/]

```
    ERR_HELP  = 0  - nesprávna špecifikácia argumentov
    ERR_PORT  = 1  - nesprávne zadaný port, mimo rozsah
    ERR_WAIT  = 2  - nesprávne zadaný časový údaj RTT
    ERR_DUPL  = 3  - duplicita argumentov
    ERR_NETW  = 4  - zadaná sieť nieje validná
    ERR_SOPT  = 5  - zlyhala funkcia setsockopt
    ERR_SOCK  = 6  - zlyhala funkcia socket
    ERR_GAIN  = 7  - zlyhala funkcia getifaddrs
    ERR_SEND  = 8  - zlyhala funkcia sendto
    ERR_IOCTL = 9  - zlyhala funkcia ioctl
    ERR_RECV  = 10 - zlyhala funkcia recv alebo recvfrom
```
