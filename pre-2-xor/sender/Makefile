CONTIKI=../../../..
# MAKE_MAC = MAKE_MAC_TSCH
all: $(CONTIKI_PROJECT)

CFLAGS+= -DPROJECT_CONF_H=\"project-conf.h\"

CONTIKI_WITH_IPV6 = 1

APPS += orchestra
MODULES += core/net/mac/tsch

include $(CONTIKI)/Makefile.include
