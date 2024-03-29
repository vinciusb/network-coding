#ifndef NET_CODING_H_
#define NET_CODING_H_

#include <random.h>
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

/* ------------------- PACKET ----------------------------------------------- */

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
    memset(packet.header.holding_packets,
           EMPTY_PACKET_ID,
           sizeof(packet.header.holding_packets));
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

/* ------------------- NODE ----------------------------------------------- */
#define NETCODING_WINDOW_SIZE 8
#define COMBINATION_PERCENTAGE_RATE 30

typedef struct netcoding_node_t {
    uint32_t mixed_buffer[NETCODING_WINDOW_SIZE];
} netcoding_node;

static netcoding_node create_node() {
    netcoding_node node;

    node.mixed_buffer[0] = 2;

    return node;
}

static int should_combine_packet() {
    return rand() % 100 < COMBINATION_PERCENTAGE_RATE;
}

static netcoding_packet route_packet(netcoding_node* node,
                                     netcoding_packet* packet) {
    if(should_combine_packet()) {
        netcoding_packet combined_packet;
        // combine_packets(&packet, &combined_packet);
        return combined_packet;
    }
    else {
        return *packet;
    }
}

#endif /* NET_CODING_H_ */