DEBUG := true
PROFILING := false

CC := gcc

SRCS := $(wildcard src/*.c)
OBJS := $(SRCS:.c=.o)

ifeq ($(DEBUG),true)
ifeq ($(PROFILING),true)
	CFLAGS := -g -pg -Wall -Werror
	LFLAGS := -lcrypto -pg
else
	CFLAGS := -g -Wall -Werror
	LFLAGS := -lcrypto
endif
else
	CFLAGS := -Wall -Werror
	LFLAGS := -lcrypto
endif

repo_analyzer: $(OBJS)
ifeq ($(DEBUG),true)
ifeq ($(PROFILING),true)
	@echo "Profiling compilation"
else
	@echo "Debug compilation"
endif
else
	@echo "Release compilation"
endif
	$(CC) $(LFLAGS) -o $@ $^

%.o: src/%.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -f src/*.o
