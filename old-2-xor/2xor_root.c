// #include "netcoding/netcoding.h"
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h> /* For printf() */
#include <stdlib.h>
// #include <sys/process.h> /* For process */
// #include "clock.h"       /* For timer */
#include "contiki.h"
#include "net/ipv6/simple-udp.h"
#include "net/netstack.h"
#include "net/routing/routing.h"
#include "random.h"
#include "sys/log.h"

/* ================ DEFINES ================================================= */
#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO

#define WITH_SERVER_REPLY 1
#define UDP_CLIENT_PORT 8765
#define UDP_SERVER_PORT 5678

typedef struct two_xor_node_t {
    struct simple_udp_connection udp_conn;

} two_xor_node;

two_xor_node *two_xor_node_constructor() {
    two_xor_node *node = (two_xor_node *)malloc(sizeof(two_xor_node));
    return node;
}

void two_xor_node_destructor(two_xor_node *node) { free(node); }

static two_xor_node *this_node;

PROCESS(two_xor_process, "2-XOR process root");
AUTOSTART_PROCESSES(&two_xor_process);

/* ================ NODE CODE =============================================== */

/**
 * @brief
 *
 * @param c
 * @param sender_addr
 * @param sender_port
 * @param receiver_addr
 * @param receiver_port
 * @param data
 * @param datalen
 */
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
    // #if WITH_SERVER_REPLY
    //     /* send back the same string to the client as an echo reply */
    //     LOG_INFO("Sending response.\n");
    //     simple_udp_sendto(&this_node->udp_conn, data, datalen, sender_addr);
    // #endif
}

PROCESS_THREAD(two_xor_process, ev, data) {
    this_node = two_xor_node_constructor();

    PROCESS_BEGIN();

    /* Initialize DAG root */
    NETSTACK_ROUTING.root_start();

    /* Initialize UDP connection */
    simple_udp_register(&this_node->udp_conn,
                        UDP_SERVER_PORT,
                        NULL,
                        UDP_CLIENT_PORT,
                        two_xor_rx_callback);

    PROCESS_END();

    two_xor_node_destructor(this_node);
}