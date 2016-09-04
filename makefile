CC=gcc

SOURCE= bezier.c
OUTPUT=bezier
OBJECT=bezier.o
LDFLAGS= -lglut -lGL -lGLU -lm
CFLAGS = -I/usr/include/GL


$(OUTPUT): $(OBJECT)
	$(CC) $(OBJECT) -o $(OUTPUT) $(LDFLAGS) $(CFLAGS) 

$(OBJECT): $(SOURCE)
	$(CC) -c $(SOURCE) -o bezier.o $(LDFLAGS)

clean:
	rm -f *.o
	rm -f bezier
