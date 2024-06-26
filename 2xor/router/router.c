#include <stdio.h>
#include "../netcoding/netcoding.h"
#include "contiki-lib.h"
#include "contiki-net.h"
#include "contiki.h"
#include "log.h"
#include "sys/node-id.h"

#define UDP_PORT 61618

#define SEND_INTERVAL (4 * CLOCK_SECOND)

static struct simple_udp_connection broadcast_connection;
static netcoding_node node;
static char buffer[PACKET_SIZE];
static uip_ipaddr_t addr;

/*---------------------------------------------------------------------------*/
PROCESS(udp_process, "UDP broadcast process");
AUTOSTART_PROCESSES(&udp_process);
/*---------------------------------------------------------------------------*/
static void receiver(struct simple_udp_connection *c,
                     const uip_ipaddr_t *sender_addr,
                     uint16_t sender_port,
                     const uip_ipaddr_t *receiver_addr,
                     uint16_t receiver_port,
                     const uint8_t *data,
                     uint16_t datalen) {
    static netcoding_packet packet;
    memcpy(&packet, data, PACKET_SIZE);

    printf("Message received from ");
    log_6addr_compact(sender_addr);
    printf(". Packet[%u bytes]-> ", datalen);
    print_packet(&packet);
    printf("\n");

    static netcoding_packet packet_to_route;
    packet_to_route = route_packet(&node, &packet);

    memcpy(buffer, &packet_to_route, sizeof(packet));
    simple_udp_sendto(&broadcast_connection, buffer, PACKET_SIZE, &addr);
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(udp_process, ev, data) {
    static struct etimer periodic_timer;
    uip_ipaddr_t addr;

    PROCESS_BEGIN();

    uip_ip6addr(&addr, UIP_DS6_DEFAULT_PREFIX, 0, 0, 0, 0, 0, 0, 2);
    uip_ds6_addr_add(&addr, 0, ADDR_AUTOCONF);

    simple_udp_register(
        &broadcast_connection, UDP_PORT, NULL, UDP_PORT, receiver);

    etimer_set(&periodic_timer, 20 * CLOCK_SECOND);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));
    etimer_set(&periodic_timer, SEND_INTERVAL);

    printf("PACKET SIZE = %d in router %d with ip ", (int)PACKET_SIZE, node_id);
    log_6addr_compact(&uip_ds6_get_link_local(-1)->ipaddr);
    printf("\n");

    node = create_node();

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));
        etimer_reset(&periodic_timer);
        uip_create_linklocal_allnodes_mcast(&addr);
    }

    PROCESS_END();
}
/*---------------------------------------------------------------------------*/
