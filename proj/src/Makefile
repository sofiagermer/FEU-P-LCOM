PROG=proj

SRCS = proj.c timer.c utils.c keyboard.c vd_card.c kbd_manager.c mole.c game.c 

# __LCOM_OPTIMIZED__ disables most of the LCF's overhead (and functionality!)
CPPFLAGS += -pedantic #-D __LCOM_OPTIMIZED__

DPADD += ${LIBLCF}
LDADD += -llcf

.include <minix.lcom.mk>
