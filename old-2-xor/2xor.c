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
#define WITH_SERVER_REPLY 0
#define UDP_CLIENT_PORT 8765
#define UDP_SERVER_PORT 5678

#define SEND_INTERVAL (10 * CLOCK_SECOND)

#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO

/* ================ NODE CODE =============================================== */
typedef struct two_xor_node_t {
    struct simple_udp_connection udp_conn;

} two_xor_node;

two_xor_node *two_xor_node_constructor() {
    two_xor_node *node = (two_xor_node *)malloc(sizeof(two_xor_node));
    return node;
}

void two_xor_node_destructor(two_xor_node *node) { free(node); }

static two_xor_node *this_node;

PROCESS(two_xor_process, "2-XOR process");
AUTOSTART_PROCESSES(&two_xor_process);

/* ================ NODE CODE =============================================== */

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
}

PROCESS_THREAD(two_xor_process, ev, data) {
    static struct etimer timer;
    uip_ipaddr_t dest_ipaddr;
    static uint32_t tx_count = 0, missed_tx_count = 0, rx_count = 0;
    static char str[32];
    this_node = two_xor_node_constructor();

    PROCESS_BEGIN();

    simple_udp_register(&this_node->udp_conn,
                        UDP_CLIENT_PORT,
                        NULL,
                        UDP_SERVER_PORT,
                        two_xor_rx_callback);

    /* Setup a periodic timer that expires after 5 seconds. */
    etimer_set(&timer, random_rand() % SEND_INTERVAL);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        if(NETSTACK_ROUTING.node_is_reachable()
           && NETSTACK_ROUTING.get_root_ipaddr(&dest_ipaddr)) {
            /* Print statistics every 10th TX */
            if(tx_count % 10 == 0) {
                LOG_INFO("Tx/Rx/MissedTx: %" PRIu32 "/%" PRIu32 "/%" PRIu32
                         "\n",
                         tx_count,
                         rx_count,
                         missed_tx_count);
            }

            /* Send to DAG root */
            LOG_INFO("Sending request %" PRIu32 " to ", tx_count);
            LOG_INFO_6ADDR(&dest_ipaddr);
            LOG_INFO_("\n");
            snprintf(str, sizeof(str), "hello %" PRIu32 "", tx_count);

            simple_udp_sendto(
                &this_node->udp_conn, str, strlen(str), &dest_ipaddr);

            tx_count++;
        }
        else {
            LOG_INFO("Not reachable yet\n");
            if(tx_count > 0) {
                missed_tx_count++;
            }
        }
    }

    PROCESS_END();

    two_xor_node_destructor(this_node);
}