# Makefile configuration
CC	= gcc
INCLUDE = -framework IOKit
CFLAGS	= -Wall -Winline -pipe -Wno-unused-label

LDFLAGS	= -L/usr/local/lib $(INCLUDE)

DEBUG	= -g -O0 -DDEBUG
RELEASE	= -O2

# Optional debugging
ifeq ($(dbg),1)
	CFLAGS += $(DEBUG)
else
	CFLAGS += $(RELEASE)
endif

# General makefile stuff
SRC	=	kbbutil.c

OBJ	=	$(SRC:.c=.o)

BINS =	$(SRC:.c=)

all:	kbbutil

kbbutil:	kbbutil.o
	@echo [link]
	@$(CC) -o $@ kbbutil.o $(LDFLAGS) $(LDLIBS)
	@rm -f $(OBJ)

.c.o:
	@echo [CC] $<
	@$(CC) -c $(CFLAGS) $< -o $@

clean:
	@echo "[Clean]"
	@rm -f $(OBJ) *~ core tags $(BINS)

tags:	$(SRC)
	@echo [ctags]
	@ctags $(SRC)

# root ownership and suid bit is set to enable non-root execution
#install:	kbbutil
#	@sudo rm -f $(PREFIX)/bin/kbbutil
#	@cp -a kbbutil $(PREFIX)/bin/kbbutil
#	@sudo chown root:root $(PREFIX)/bin/kbbutil
#	@sudo chmod 4755 $(PREFIX)/bin/kbbutil

depend:
	makedepend -Y $(SRC)
