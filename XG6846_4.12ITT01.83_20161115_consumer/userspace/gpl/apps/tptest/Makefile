# CC		= gcc
MAKEDEPEND	= makedepend
CDEBUG		= -g
EXTRADEFINES	= -DUNIX -DLINUX
CFLAGS		= ${CDEBUG} -O0 ${EXTRADEFINES} ${INCL} -Wall
LDFLAGS		=
LIBDIR		= 
LOCLIBS		= 
LIBS		= ${LOCLIBS} ${SYSLIBS}
OBJS		= tpio_unix.o tpengine.o tpcommon.o client.o tpclient.o getopt.o
SRCS		= tpio_unix.c tpengine.c tpcommon.c client.c tpclient.c getopt.c
LIBSRCS		= 
ISRCS		= tpengine.h tpio.h tpio_unix.h server.h tpclient.h
ALLSRCS		= ${SRCS} ${ISRCS} ${LIBSRCS}

all: tptest install

tptest: ${OBJS}
	${CC} ${LDFLAGS} -o tptest ${OBJS} ${LIBDIR} ${LIBS}

clean:
	rm -f tptest core *.o *.BAK *.bak *.CKP a.out

install: tptest
	install -m 755 tptest $(INSTALL_DIR)/bin
	$(STRIP) $(INSTALL_DIR)/bin/tptest
	
depend: 
	${MAKEDEPEND} ${INCL} ${SRCS} ${LIBSRCS}

