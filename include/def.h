#pragma once

// == Input
#define MACLEN 17
#define IS_DELIMITER i % 3 == 2
// ====================

// == Errors 
#define SUCCESS 1
#define FAILURE 0

#define EARG "-> sudo ./ft_malcolm [source ip] [source mac address] [target ip] [target mac address] -[options]\n"
#define EOPT "unknown option -> [%s], ignoring\n"
#define EBADMAC "invalid mac address -> [%s]\n"
// ====================

// == Program options == 
#define MAX_POSSIBLE_OPTS 1
#define VALID_OPTS "v"

#define VERBOSE 1
// ====================