#ifndef NET_CODING_H_
#define NET_CODING_H_

#include <stdio.h>

#define NUM_COMBINATIONS 2
#define PAYLOAD_SIZE 30

typedef struct netcoding_packet_header_t {
    int holding_packets[NUM_COMBINATIONS];
} netcoding_packet_header;

typedef struct netcoding_packet_t {
    netcoding_packet_header header;
    char body[PAYLOAD_SIZE];
} netcoding_packet;

#define PACKET_SIZE sizeof(netcoding_packet)

#endif /* NET_CODING_H_ */