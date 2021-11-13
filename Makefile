CC=gcc
CFLAGS=-Wall -g -std=gnu99
LIBS=-lm -lncurses
LIBSTEST= -lm -lcmocka
MAIN=main
TEST=test
SRC=src/
BUILD=build/

compile:
	$(CC) $(CFLAGS) $(SRC)$(MAIN).c -o $(BUILD)$(MAIN) $(LIBS)

run:
	./$(BUILD)$(MAIN) $(arg)

compile-tests:
	$(CC) $(CFLAGS) $(TEST)/$(TEST).c -o $(BUILD)$(TEST) $(LIBSTEST)

run-tests : 
	./$(BUILD)$(TEST)

clean:
	rm -rf *~ $(BUILD)*
