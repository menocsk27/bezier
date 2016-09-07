CC=gcc

SOURCE= bezier_MJ.c
OUTPUT=bezier_MJ
OBJECT=bezier_MJ.o
LDFLAGS= -lglut -lGL -lGLU -lm
CFLAGS = -I/usr/include/GL


$(OUTPUT): $(OBJECT)
	$(CC) $(OBJECT) -o $(OUTPUT) $(LDFLAGS) $(CFLAGS) 

$(OBJECT): $(SOURCE)
	$(CC) -c $(SOURCE) -o  $(OBJECT)

clean:
	rm -f *.o
	rm -f bezier_MJ
