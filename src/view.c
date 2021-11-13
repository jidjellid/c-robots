#include <ncurses.h>
#include <string.h>
#include <unistd.h>
#include "config.c"
#include "robot.c"
#include "parser.c" 

#define NUMBER_BUTTONS 4

#define STEP 100000

#define PLAY_PAUSE 32
#define SPEED_UP 43
#define SPEED_DOWN 45
#define QUIT 113

/* Iteration of the script */
int iter = 0;

/* Main window */
WINDOW *window;

/* current_Delay between each iteration */
int current_delay;

/* Menu bottons */
char const *buttons[] = { "PLAY/PAUSE SPACE", "SPEED+ +", "SPEED- -", "QUIT Q" };

/**
 * Create a menu on view
 * 
 * @param buttons 
 */
void create_menu(char const *buttons[]){
	for (int i = 0; i < NUMBER_BUTTONS; i++)
		mvprintw(1+i,TERM_WIDTH-25, buttons[i]);
	mvprintw(2+NUMBER_BUTTONS,TERM_WIDTH-25, "DELAY: %i usec", current_delay);
	mvprintw(3+NUMBER_BUTTONS,TERM_WIDTH-25, "ITERATION: %i ", iter);
}

/**
 * Key listenner
 */
void key_listener(){
	switch (getch())
	{
	case PLAY_PAUSE:
		timeout(-1);
		getch();
		timeout(1);
		break;
	case SPEED_UP:
		if(current_delay-STEP>=MIN_DELAY){
			current_delay-=STEP;
		}
		break;
	case SPEED_DOWN:
		if(current_delay+STEP<=MAX_DELAY){
			current_delay+=STEP;
		}
		break;
	case QUIT:
		endwin();
		exit(EXIT_SUCCESS);
		break;
	default:
		break;
	}
}

/**
 * Get relative X to view
 * 
 * @param x from robot model
 * @return int 
 */
int get_posx(int x){
	return (int)((x *(TERM_WIDTH-INFO_WIDTH))/WIDTH);
}

/**
 * Get relative Y to view
 * 
 * @param y from robot model
 * @return int 
 */
int get_posy(int y){
	return (int)((y*TERM_HEIGHT)/HEIGHT);
}

/**
 * 	Get id of the winner
 * 
 * @param robots model array
 * @return int 
 */
int get_winner(robot robots[])
{
	for (size_t i = 0; i < NUMBER_ROBOTS; i++)
		if (robots[i].state)
			return robots[i].id;
	return -1;
}

/**
 * Get number of alive robots
 * 
 * @param robots 
 * @return int 
 */
int get_alives(robot robots[])
{	
	int alive = 0;
	for (size_t i = 0; i < NUMBER_ROBOTS; i++)
		alive+=robots[i].state;
	return alive;
}

/**
 * Convert int to char and display data at position
 * 
 * @param type int string
 * @param data to print
 * @param y on view
 * @param margin margin-right
 */
void print_info(char const * type, int data, int y, int margin){
	char info[10]; 
	sprintf(info, type,(int)data);
	mvprintw(y, TERM_WIDTH-INFO_WIDTH+margin, info);
}

/**
 * Update view for each iteration of action parsed
 * 
 * @param window of the view
 * @param robots model array
 */
void update(WINDOW *window, robot robots[]){
	clear();
	box(window,0,0);
	refresh();
	wrefresh(window);
	create_menu(buttons);
	for (int i = 0; i < NUMBER_ROBOTS; i++)
	{	
		//Display missiles
		for (int j = 0; j < NUMBER_MISSILES; j++)
		{	
			if(robots[i].missiles[j].state == ACTIVE){
				// Display missile
				mvprintw(get_posy(robots[i].missiles[j].posY), get_posx(robots[i].missiles[j].posX), ".");

				//Display info missile
				print_info("X: %i", robots[i].missiles[j].posX, 2+j+(i*7), 34);
				print_info("Y: %i", robots[i].missiles[j].posY, 2+j+(i*7), 42);
			}
		}

		//Display robot
		if(robots[i].state==ACTIVE){
			char id[5]; 
			sprintf(id, "%d", robots[i].id);
			mvprintw(get_posy(robots[i].posY), get_posx(robots[i].posX), id);
		}

		//Display info
		print_info("#%i", robots[i].id, 1+(i*7), 1);
		print_info("X: %i", robots[i].posX, 2+(i*7), 1);
		print_info("Y: %i", robots[i].posY, 2+(i*7), 10);
		print_info("LIFE: %i", robots[i].life, 3+(i*7), 1);
		print_info("STATE: %i", robots[i].state, 4+(i*7), 1);
		print_info("SPEED: %i", robots[i].speed, 5+(i*7), 1);
	}

	//Create Info panel line
	for (int i = 1; i < TERM_HEIGHT-1; i++)
		mvprintw(i, TERM_WIDTH-INFO_WIDTH, "|");

	key_listener();
	move(0,0);	
	refresh();
	wrefresh(window);
}

/**
 * Create a window object
 */
void create_window(FILE **files){
	current_delay = DELAY;
	robot robots[NUMBER_ROBOTS];
	create_robots(robots);
	initscr();
	window = newwin(TERM_HEIGHT, TERM_WIDTH, 0, 0);
	timeout(1);
	keypad(window, TRUE);

	while((robots[0].robdone != 2 || robots[1].robdone != 2 || robots[2].robdone != 2 || robots[3].robdone != 2) && get_alives(robots) != 1){
		update(window, robots);
		cycleRobot(files[0],files[1], files[2], files[3], robots);
		update_robots(robots);
		usleep(current_delay);
		iter++;
	}

	mvprintw(get_posy(WIDTH)/2,get_posy(HEIGHT)/2, "END OF SCRIPTS! ROBOT #%i WON", get_winner(robots));
	wrefresh(window);
	timeout(-1);
	getch();
	endwin(); 
	free(files);
}

/**
 * Main launch
 * 
 * @param argc 
 * @param argv 
 */
void launch(int argc, const char *argv[])
{	
	FILE **files = malloc(sizeof(FILE*) * NUMBER_ROBOTS);
	for (int i = 0; i < NUMBER_ROBOTS; i++)
	{	
		char name[128];
		strcpy(name, (const char*)SCRIPT);
		strcat(name, (const char*)argv[i+1]);
		strcat(name, (const char*)EXTENTION);
		strcat(name, "\0");
		files[i] = fopen(name,"r");
	}
	create_window(files);
}
