#ifndef NET_CODING_H_
#define NET_CODING_H_

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

#endif /* NET_CODING_H_ */