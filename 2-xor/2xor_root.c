#include "netcoding/netcoding.h"

/* ================ DEFINES ================================================= */
#define LOG_LEVEL LOG_LEVEL_INFO

#define WITH_SERVER_REPLY 1
#define UDP_CLIENT_PORT 8765
#define UDP_SERVER_PORT 5678

static struct simple_udp_connection udp_conn;

PROCESS(two_xor_process, "2-XOR process root");
AUTOSTART_PROCESSES(&two_xor_process);

/* ================ NODE CODE =============================================== */

PROCESS_THREAD(two_xor_process, ev, data) {
    two_xor_node* this_node = two_xor_node_constructor();

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