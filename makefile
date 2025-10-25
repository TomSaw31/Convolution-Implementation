CC=gcc
CFLAGS=-std=c99 -Wextra -Wall -Werror -pedantic 
LDFLAGS=-lm -lpng -lz

ECHO = @
ifeq ($(VERBOSE),1)
	ECHO=
endif

ifeq ($(DEBUG),yes)
	CFLAGS += -g
	LDFLAGS +=
else
	CFLAGS += -O3 -DNDEBUG
	LDFLAGS +=
endif

EXEC=main
SRC= $(wildcard *.c)
OBJ= $(SRC:.c=.o)

all: 
ifeq ($(DEBUG),yes)
	@echo "Generating in debug mode"
else
	@echo "Generating in release mode"
endif
	@$(MAKE) $(EXEC)

$(EXEC): $(OBJ)
	$(ECHO)$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(ECHO)$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper

clean:
	$(ECHO)rm -rf *.o

mrproper: clean
	$(ECHO)rm -rf $(EXEC) documentation/html

doc: convolution.h
	$(ECHO)doxygen documentation/TP2
	
convolution.o: convolution.h
doc : convolution.c main.c
