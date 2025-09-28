#pragma once

// == Arp ==
#define ETH 1
#define REQUEST 1
#define REPLY 2
// ====================

// == Input ==
#define CONTINUE 0
#define IPLEN 4
#define NOT_FOUND 0
#define FOUND 1
#define MAC_LEN 17
#define BYTE_MAC_LEN 6
#define IP_MAX_LEN 16
#define IS_DELIMITER i % 3 == 2
#define MIN_CHAR_VAL 10
#define MAX_CHAR_RANGE 6
#define BASE_16 16
#define MAX_INTERFACE_SIZE 16

#define CLASS_A "10."
#define CLASS_B "172.16."
#define CLASS_C "192.168."
#define LOCALHOST "127.0.0.1"
// ====================

// == Messages ==
#define WAITING "waiting for arp packet...\n"
#define SSEND "ARP reply successfully sent to %s\n"
#define SINFND "Found interface -> %s !\n"
#define SMACADDR "Successfully formated MAC addresses\n"
#define SIPADDR "Successfully formated IP addresses\n"
// ====================

// == Errors ==
#define SUCCESS 1
#define FAILURE 0
#define ERROR 1

#define EARG "-> sudo ./ft_malcolm [source ip] [source mac address] [target ip] [target mac address] -[options]\n"
#define EOPT "unknown option -> [%s].. ignoring\n"
#define EBADINPT "invalid input.. exiting\n"
#define EBADMAC "invalid mac address -> [%s].. exiting\n"
#define EBADIP "invalid ip address -> [%s].. exiting\n"
#define ENTOP "inet_ntop() failed.. exiting\n"
#define ESOCK "socket(): %s\n"
#define ERECV "recvfrom(): %s\n"
#define ESENDTO "sendto() failed\n"
#define EIFADDR "getifaddrs(): cannot find interface\n"
#define EINOTFND "No valid interface found, please enter one manually -> -Iinterface (no space).. exiting\n"
#define WHY "%s ? why would you do this on a non private addess ? exiting\n"
#define WHYLOCAL "%s ? why would you do this on localhost ? exiting\n"
// ====================

// == Program options == 
#define MAX_POSSIBLE_OPTS 1
#define VALID_OPTS "vsi"
// ====================

#define VERBOSE 1
#define SPAM 's'
// ====================

// == Verbose ==
#define STARTMACFORM "formating MAC addresses..\n"
#define MACFORM "\t | SOURCE -> HEX: [%lX]; DEC: [%lu]\n\t | TARGET -> HEX: [%lX]; DEC: [%lu]\ndone !\n\n"
#define STARTIPFORM "formating IP addresses..\n"
#define RESOLVIPSRC "resolution found for : \n\t | SOURCE -> RESOLVED: [%s] = [%s]\n"
#define RESOLVIPTRGT "resolution found for : \n\t | TARGET -> RESOLVED: [%s] = [%s]\n"
#define VALID_IPS "valid IPS: \n"
#define IPFORM "\t | SOURCE -> ASCII: [%s]; DEC: [%u]\n\t | TARGET -> ASCII: [%s]; DEC: [%u]\ndone !\n\n"
#define TRGTFOUND "ARP request with target IP found !\n" // afficher en string ?
// ====================