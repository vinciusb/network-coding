#ifndef NET_CODING_H_
#define NET_CODING_H_

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define NUM_COMBINATIONS 2
#define PAYLOAD_SIZE 30
#define EMPTY_PACKET_ID UINT32_MAX

typedef struct netcoding_packet_header_t {
    uint32_t holding_packets[NUM_COMBINATIONS];
} netcoding_packet_header;

typedef struct netcoding_packet_t {
    netcoding_packet_header header;
    char body[PAYLOAD_SIZE];
} netcoding_packet;

#define PACKET_SIZE sizeof(netcoding_packet)

/* -------------------------------------------------------------------------- */

static void print_header(netcoding_packet_header* header) {
    for(int i = 0; i < NUM_COMBINATIONS; i++) {
        uint32_t origin = header->holding_packets[i];
        origin == EMPTY_PACKET_ID ? printf("-1") : printf("%u", origin);
        if(i < NUM_COMBINATIONS - 1) printf(", ");
    }
}

static void print_packet(netcoding_packet* packet) {
    printf("Header: [");
    print_header(&packet->header);
    printf("], Body: \"%s\"", packet->body);
}

static netcoding_packet create_packet(uint32_t packet_id, const char* message) {
    netcoding_packet packet;
    for(size_t i = 1; i < NUM_COMBINATIONS; i++)
        packet.header.holding_packets[i] = EMPTY_PACKET_ID;
    packet.header.holding_packets[0] = packet_id;

    size_t str_len = strlen(message);
    if(str_len >= PAYLOAD_SIZE) {
        printf("%zu is bigger then payload size %d", str_len, PAYLOAD_SIZE);
        strcpy(packet.body, "INVALID");
    }
    else {
        strcpy(packet.body, message);
    }

    return packet;
}

#endif /* NET_CODING_H_ */