#include <ncurses.h>
#include <time.h>
#include <stdlib.h>

struct stack
{
	int x;
	int y;
	struct stack *next;
};

int main()
{	
	int ch, i, j;
	int MaxX, MaxY;
	char **board;
	struct stack *start, *temp;
	start = NULL;

	initscr();			/* Start curses mode 		*/
	curs_set(0);
	raw();				/* Line buffering disabled	*/
	noecho();

	keypad(stdscr, TRUE);

	getmaxyx(stdscr, MaxX, MaxY);

	board = (char **)malloc(sizeof(char *)*MaxX);
	for(i=0; i<MaxX; i++)
		board[i] = (char *)malloc(sizeof(char)*MaxY);

	for(i=0; i<MaxX; i++)
	{
		for(j=0; j<MaxY; j++)
		{
			if ( i%2 == 0 && j%2 == 0 )
				board[i][j] = '0';
			else
				board[i][j] = '1';
		}
	}

	int curX, curY, lastX, lastY;
	int buf[4];
	buf[0] = buf[1] = buf[2] = buf[3] = 0;

	start = (struct stack *)malloc(sizeof(struct stack));
	start->x = 0;
	start->y = 0;
	start->next = NULL;

	board[0][0] = '1';
	curX = 0;
	curY = 0;
	lastX = 0;
	lastY = 0;

	start_color();
	init_pair(1, COLOR_WHITE, COLOR_GREEN);
	//init_pair(1, COLOR_WHITE, COLOR_CYAN);
	//init_pair(1, COLOR_WHITE, COLOR_WHITE);
	attron(COLOR_PAIR(1));

	mvprintw(0, 0, " ");

	srand ( time(NULL) );
	while (start)
	{
		refresh();
		fflush(stdout);
		usleep(10000);

		int k=0;
		if ( curX-2 >=0 && board[curX-2][curY] == '0' )
		{
			buf[k] = 0;
			k++;
		}

		if ( curY+2 < MaxY && board[curX][curY+2] == '0' )
		{
			buf[k] = 1;
			k++;
		}

		if ( curX+2 < MaxX && board[curX+2][curY] == '0' )
		{
			buf[k] = 2;
			k++;
		}

		if ( curY-2 >=0 && board[curX][curY-2] == '0' )
		{
			buf[k] = 3;
			k++;
		}

		if ( k > 0 )
		{
			temp = (struct stack *)malloc(sizeof(struct stack));
			temp->next = start;
			start = temp;

			k = rand()%k;

			if ( buf[k] == 0 )
			{
				curX = curX-2;
				curY = curY;
			}
			else if ( buf[k] == 1 )
			{
				curX = curX;
				curY = curY+2;
			}
			else if ( buf[k] == 2 )
			{
				curX = curX+2;
				curY = curY;
			}
			else
			{
				curX = curX;
				curY = curY-2;
			}

			start->x = curX;
			start->y = curY;
			board[curX][curY] = '1';
		}
		else
		{
			temp = start;
			start = start->next;

			temp->next = NULL;
			curX = temp->x;
			curY = temp->y;

			free(temp);
		}

		mvprintw(curX, curY, " ");

		if ( curX == lastX )
		{
			if ( lastY + 2 == curY )
				mvprintw(curX, lastY+1, " ");
			else
				mvprintw(curX, lastY-1, " ");
		}
		else
		{
			if ( lastX - 2 == curX )
				mvprintw(lastX-1, curY, " ");
			else
				mvprintw(lastX+1, curY, " ");
		}

		lastX = curX;
		lastY = curY;
	}

	attroff(COLOR_PAIR(1));

	wgetch(stdscr);

	refresh();			/* Print it on to the real screen */
	endwin();			/* End curses mode		  */

	for(i=0; i<MaxX; i++)
		free(board[i]);

	free(board);

	return 0;
}
