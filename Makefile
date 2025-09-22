CC := gcc
CFLAGS := -g -Wall -Wextra `curl-config --cflags`
LDLIBS := `curl-config --libs`

bview: main.o file.o csv.o data.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

%.o: %.c %.h
	$(CC) -c $(CFLAGS) -o $@ $<
