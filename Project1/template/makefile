CC       = gcc 
CFLAGS   = -O3 -Wall 
LIBS      = -lXi -lXmu -lglut -lGLEW -lGLU -lm -lGL
OBJDIR   = ../mylib
OBJS     = $(OBJDIR)/initShader.o $(OBJDIR)/matrixlib.o

proj1: proj1.c $(OBJS)
	$(CC) -o proj1 proj1.c $(OBJS) $(CFLAGS) $(LIBS)

readfile: readfile.c $(OBJS)
	$(CC) -o readfile readfile.c $(OBJS) $(CFLAGS) $(LIBS)

$(OBJDIR)/%.o: %.c %.h
	$(CC) -c @< -o $@ $(CFLAGS)

