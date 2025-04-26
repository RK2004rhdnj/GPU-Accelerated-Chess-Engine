CC := gcc
CFLAGS := -O3
SOURCES := xboard.c blunderbuss.c uci.c evaluate.c pvtable.c init.c bitboards.c hashkeys.c board.c data.c attack.c io.c movegen.c validate.c makemove.c perft.c search.c misc.c polybook.c polykeys.c
EXECUTABLE := blunderbuss

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES)
	$(CC) $(CFLAGS) $^ -o $@

gits_up:
	git status
	git add .
	git commit -m "Improved Engine Evaluation"
	git push

clean:
	rm -f $(EXECUTABLE)
	rm -f *.o
