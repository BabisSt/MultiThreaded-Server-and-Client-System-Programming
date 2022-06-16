CC = gcc
CFLAGS = -g3 -Wall
LDFLAGS = -lm -lpthread

ODIR = BUILD
IDIR = HEADERS
SDIR = SRC


EXECUTABLE = server

_DEPS = server.h queue.h ../../MASTER/HEADERS/pipe.h ../../MASTER/HEADERS/stats.h ../../MASTER/HEADERS/patient.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = server.o queue.o ../../MASTER/BUILD/pipe.o ../../MASTER/BUILD/stats.o ../../MASTER/BUILD/patient.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@ $(LDFLAGS)

$(EXECUTABLE): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f $(ODIR)/*.o
	# rm -f $(BDIR)/$(EXECUTABLE)
