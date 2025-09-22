#pragma once

// == Input ==
#define MAC_LEN 17
#define IP_MAX_LEN 46
#define IS_DELIMITER i % 3 == 2
#define MIN_CHAR_VAL 10
#define MAX_CHAR_RANGE 6
#define BASE_16 16

#define CLASS_A "10."
#define CLASS_B "172.16."
#define CLASS_C "192.168."
#define LOCALHOST "127.0.0.1"
// ====================

// == Errors ==
#define SUCCESS 1
#define FAILURE 0

#define EARG "-> sudo ./ft_malcolm [source ip] [source mac address] [target ip] [target mac address] -[options]\n"
#define EOPT "unknown option -> [%s].. ignoring\n"
#define EBADMAC "invalid mac address -> [%s].. exiting\n"
#define EBADIP "invalid ip address -> [%s].. exiting\n"
#define ENTOP "inet_ntop() failed.. exiting\n"
// ====================

// == Program options == 
#define MAX_POSSIBLE_OPTS 1
#define VALID_OPTS "v"

#define VERBOSE 1
// ====================

// == Verbose ==
#define STARTMACFORM "formating MAC addresses..\n"
#define MACFORM "\t | SOURCE -> HEX: [%lx]; DEC: [%ld]\n\t | TARGET -> HEX: [%lx]; DEC: [%ld]\ndone !\n\n"
#define STARTIPFORM "formating IP addresses..\n"
#define HOSTTOIPSRC "hostname resolution found for : \n\t | SOURCE -> HOSTNAME: [%s]; IP: [%s]\n"
#define HOSTTOIPTRGT "hostname resolution found for : \n\t | TARGET -> HOSTNAME: [%s]; IP: [%s]\n"
#define VALID_IPS "valid IPS: \n"
#define WHYPUBLIC "%s ? why would you do this on a public addess ? exiting\n"
#define WHYLOCAL "%s ? why would you do this on localhost ? exiting\n"
#define IPFORM "\t | SOURCE -> ASCII: [%s]; DEC: [%ld]\n\t | TARGET -> ASCII: [%s]; DEC: [%ld]\ndone !\n\n"
// ====================