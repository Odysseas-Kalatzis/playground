CC=gcc
DEBUG=no
EXEC=ring

CFLAGS+=-DTEST=1

# All c files in this directory.
SRC = $(wildcard *.c)
# Otherwise if one wishes to be specific:
#SRC = $(wildcard src_dir1/*.c) \
#      $(wildcard src_dir2/*.c)

# -g for debug.
# -Wall -Wextra to get all the possible warnings.
ifeq ($(DEBUG),yes)
	CFLAGS+=-W -Wall -Wextra -pedantic -g -std=c99
	LDFLAGS= -lm -lSDL2
else
	CFLAGS+=-W -Wall -Wextra -pedantic -O2
	LDFLAGS= -lm -lSDL2
endif

###############################################

all: $(EXEC)

obj = $(SRC:.c=.o)
dep = $(obj:.o=.d)

$(EXEC): $(obj)
	$(CC) -o $@ $^ $(LDFLAGS)

-include $(dep)

%.d: %.c
	@$(CC) $(CFLAGS) $< -MM -MT $(@:.d=.o) >$@

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

.PHONY: clean
clean:
	rm -f $(obj) $(EXEC) $(dep)
