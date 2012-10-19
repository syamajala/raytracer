CC = g++
CFLAGS = -g -I. -I/usr/include/OpenEXR -Wall
LDFLAGS = -g -lIlmImf -lImath -lHalf -Wall

#all: clean raytra
all: clean raytra

#raytra: raytra.o gVector.o gPoint.o parse.o gShape.o gMaterial.o gLight.o
#	$(CC) $(LDFLAGS) raytra.o gVector.o gPoint.o parse.o gShape.o gMaterial.o gLight.o -o raytra

raytra:
	g++ -g *.cc -I. -I/usr/include/OpenEXR -lIlmImf -lImath -lHalf -Wall -o raytra

raytra.o: raytra.cc raytra.h 
	$(CC) $(CFLAGS) -c raytra.cc

parse.o: parse.cc parse.h gPoint.h gShape.h
	$(CC) $(CFLAGS) -c parse.cc

gLight.o: gLight.cc gLight.h gPoint.h gVector.h
	$(CC) $(CFLAGS) -c gLight.cc

gShape.o: gShape.cc gShape.h gPoint.h gMaterial.h
	$(CC) $(CFLAGS) -c gShape.cc

gMaterial.o: gMaterial.h gVector.h
	$(CC) $(CFLAGS) -c gMaterial.cc

gPoint.o: gPoint.cc gPoint.h gVector.h
	$(CC) $(CFLAGS) -c gPoint.cc 

gVector.o: gVector.cc gVector.h
	$(CC) $(CFLAGS) -c gVector.cc

clean:
	rm -f *~ *.o *.exr raytra 
