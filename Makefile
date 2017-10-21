VPATH = $(SRC_DIR)
CFLAGS = -Wall -O3
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

SRCS ::= $(wildcard $(SRC_DIR)/*.c)
OBJS ::= $(SRCS:.c=.o)
DEPS ::= $(SRCS:.c=.d)

.PHONY: all
all: $(BIN_DIR)/plc

-include $(DEPS)

$(BIN_DIR)/plc: $(OBJS)
	-mkdir -p $(BIN_DIR)
	gcc $^ -o $@

%.d : %.c
	gcc -MM $< -o $@
	@echo "$(SRC_DIR)/$$(cat $@)" > $@

.PHONY: clean
clean:
	-rm $(OBJS) $(DEPS)
