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


install:	kbbutil
	@mv kbbutil $(PREFIX)/bin/kbbutil

depend:
	makedepend -Y $(SRC)
