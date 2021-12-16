CXX = g++
CFLAGS ?= -g -Wall -Wextra
LDFLAGS ?=

TRGT = main
OBJS = $(TRGT).o

all: $(TRGT)

$(TRGT): $(TRGT).o
	$(CXX) $(CFLAGS) $(LDFLAGS) $< -o $@

%.o: %.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

.PHONY: all clean

clean:
	$(RM) $(TRGT) *.o

