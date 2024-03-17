#include "netcoding/netcoding.h"

/* ================ DEFINES ================================================= */
#define LOG_LEVEL LOG_LEVEL_INFO

#define WITH_SERVER_REPLY 0
#define UDP_CLIENT_PORT 8765
#define UDP_SERVER_PORT 5678

PROCESS(two_xor_process, "2-XOR process");
AUTOSTART_PROCESSES(&two_xor_process);

/* ================ NODE CODE =============================================== */

PROCESS_THREAD(two_xor_process, ev, data) {
    static struct etimer timer;
    uip_ipaddr_t dest_ipaddr;
    static uint32_t tx_count = 0, missed_tx_count = 0;
    static char str[32];
    two_xor_node* this_node = two_xor_node_constructor();

    PROCESS_BEGIN();

    simple_udp_register(&this_node->udp_conn,
                        UDP_CLIENT_PORT,
                        NULL,
                        UDP_SERVER_PORT,
                        two_xor_rx_callback);

    /* Setup a periodic timer that expires after 5 seconds. */
    etimer_set(&timer, CLOCK_SECOND * 5);

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

            simple_udp_sendto(&udp_conn, str, strlen(str), &dest_ipaddr);

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