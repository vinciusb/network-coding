#ifndef NET_CODING_H_
#define NET_CODING_H_

// #include <stdio.h> /* For printf() */
// #include <stdlib.h>
// #include <sys/process.h> /* For process */
// #include "clock.h"       /* For timer */
// #include "contiki.h"
// #include "net/ipv6/simple-udp.h"
// #include "net/mac/tsch/tsch.h"
// #include "sys/log.h"

#include <inttypes.h>
#include <stdint.h>
#include "contiki.h"
#include "net/ipv6/simple-udp.h"
#include "net/netstack.h"
#include "net/routing/routing.h"
#include "random.h"
#include "sys/log.h"

/* ================ DEFINES ================================================= */
#define LOG_LEVEL LOG_LEVEL_INFO

/* ================ NODE CODE =============================================== */
typedef struct two_xor_node_t {
    struct simple_udp_connection udp_conn;

} two_xor_node;

/**
 * @brief Constructor for the two xor node class.
 *
 * @return two_xor_node*
 */
two_xor_node *two_xor_node_constructor();

/**
 * @brief Destructor for the two xor node class.
 *
 * @param node
 */
void two_xor_node_destructor(two_xor_node *node);

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
                                uint16_t datalen);

#endif /* NET_CODING_H_ */