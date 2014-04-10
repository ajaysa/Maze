maze: maze.c
	gcc -o maze maze.c -lncurses

clean:
	rm -f maze
