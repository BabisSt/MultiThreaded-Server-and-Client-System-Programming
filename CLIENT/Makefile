CC = gcc
CFLAGS = -g3 -Wall
LDFLAGS = -lm -lpthread

ODIR = BUILD
IDIR = HEADERS
SDIR = SRC

EXECUTABLE = client

_DEPS = client.h ../../MASTER/HEADERS/pipe.h ../../SERVER/HEADERS/queue.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = client.o ../../MASTER/BUILD/pipe.o ../../SERVER/BUILD/queue.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@ $(LDFLAGS)

$(EXECUTABLE): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f $(ODIR)/*.o
	# rm -f $(BDIR)/$(EXECUTABLE)
