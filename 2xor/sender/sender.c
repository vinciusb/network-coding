#include <stdio.h>
#include "../netcoding/netcoding.h"
#include "contiki-lib.h"
#include "contiki-net.h"
#include "contiki.h"
#include "sys/node-id.h"

#define UDP_PORT 61618

#define SEND_INTERVAL (4 * CLOCK_SECOND)

static struct simple_udp_connection broadcast_connection;

#ifndef SIZE
#define SIZE 100
#endif

/*---------------------------------------------------------------------------*/
PROCESS(udp_process, "UDP broadcast sender process");
AUTOSTART_PROCESSES(&udp_process);
/*---------------------------------------------------------------------------*/
static void receiver(struct simple_udp_connection *c,
                     const uip_ipaddr_t *sender_addr,
                     uint16_t sender_port,
                     const uip_ipaddr_t *receiver_addr,
                     uint16_t receiver_port,
                     const uint8_t *data,
                     uint16_t datalen) {
    printf("Data received on port %d from port %d with length %d\n",
           receiver_port,
           sender_port,
           datalen);
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(udp_process, ev, data) {
    static struct etimer periodic_timer;
    uip_ipaddr_t addr;

    PROCESS_BEGIN();

    simple_udp_register(
        &broadcast_connection, UDP_PORT, NULL, UDP_PORT, receiver);

    etimer_set(&periodic_timer, 20 * CLOCK_SECOND);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));
    etimer_set(&periodic_timer, SEND_INTERVAL);

    int packet_id = 0;
    printf("PACKET SIZE = %d in node %d\n", (int)PACKET_SIZE, node_id);
    char packet_message[PAYLOAD_SIZE];

    while(1) {
        sprintf(packet_message, "Message %d from %d", packet_id, node_id);
        netcoding_packet packet = create_packet(packet_id, packet_message);

        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));
        etimer_reset(&periodic_timer);

        printf("Sending broadcast message %d\n", packet_id);
        uip_create_linklocal_allnodes_mcast(&addr);

        // simple_udp_sendto(&broadcast_connection, &packet, PACKET_SIZE,
        // &addr);

        // Make sure no packet is going to have and invalid id
        packet_id++;
        // packet_id = packet_id % EMPTY_PACKET_ID;
    }

    PROCESS_END();
}
/*---------------------------------------------------------------------------*/
