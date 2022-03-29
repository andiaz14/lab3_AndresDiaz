prefix=/usr/local
CC = g++

# Normal
CFLAGS = -O3 -Wall
LDFLAGS = -lpthread

# Profiling, debug
#CFLAGS = -g -pg -Wall -O3 

SRC = hebras.cpp
OBJ = hebras.o
APP = lab3

all: $(OBJ)
	$(CC) $(CFLAGS) -o $(APP) $(OBJ) $(LIBS) $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

clean:
	$(RM) $(OBJ) $(APP)

install: $(APP)
	install -m 0755 $(APP) $(prefix)/bin

uninstall: $(APP)
	$(RM) $(prefix)/bin/$(APP)
