# Customize below to fit your system

PREFIX ?= /usr/local
MANPREFIX = ${PREFIX}/share/man
# specify your systems terminfo directory
# leave empty to install into your home folder
TERMINFO := ${DESTDIR}${PREFIX}/share/terminfo

INCS = -I.
LIBS = -lc -lutil -lncursesw
DVTMCPPFLAGS = -D_POSIX_C_SOURCE=200809L -D_XOPEN_SOURCE=700 -D_XOPEN_SOURCE_EXTENDED
DVTMCFLAGS = -std=c99 ${INCS} -DNDEBUG ${DVTMCPPFLAGS} ${CFLAGS}
DVTMLDFLAGS = ${LIBS} ${LDFLAGS}

CC ?= cc
