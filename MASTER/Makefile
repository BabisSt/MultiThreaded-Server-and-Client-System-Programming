CC = gcc
CFLAGS = -g3 -Wall
LDFLAGS = -lm

ODIR = BUILD
IDIR = HEADERS
SDIR = SRC

EXECUTABLE = master

_DEPS = country_list.h worker.h pipe.h avl.h date.h hash.h list.h patient.h stats.h 
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o country_list.o worker.o pipe.o avl.o date.o hash.o patient.o list.o stats.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@ $(LDFLAGS)

$(EXECUTABLE): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f $(ODIR)/*.o
	# rm -f $(BDIR)/$(EXECUTABLE)

