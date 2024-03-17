#include "netcoding.h"

two_xor_node *two_xor_node_constructor() {
    two_xor_node *node = (two_xor_node *)malloc(sizeof(two_xor_node));
    return node;
}

void two_xor_node_destructor(two_xor_node *node) { free(node); }

static void two_xor_rx_callback(struct simple_udp_connection *c,
                                const uip_ipaddr_t *sender_addr,
                                uint16_t sender_port,
                                const uip_ipaddr_t *receiver_addr,
                                uint16_t receiver_port,
                                const uint8_t *data,
                                uint16_t datalen) {
    LOG_INFO("Received request '%.*s' from ", datalen, (char *)data);
    LOG_INFO_6ADDR(sender_addr);
    LOG_INFO_("\n");
#if WITH_SERVER_REPLY
    /* send back the same string to the client as an echo reply */
    LOG_INFO("Sending response.\n");
    simple_udp_sendto(&udp_conn, data, datalen, sender_addr);
#endif
}