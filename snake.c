/*    SNAKE    */
/*   lb13g16   */
/*    2018     */



/* 
**								TODO
**			1. Display died message
**			2. Levels
**			3. Fix screen refresh awfulness
**          4. Demo screen
**			5. snake made out of </>/v/^ depending on direction
*/



#include "os.h"
#include "color.h"
#include <util/delay.h>
#include <avr/io.h>

void init(void);

/* 
**	in_game - 0=false, any other value = true;
**	level - game Level
**	score - total number of points scored this game
**	x - horizontal distance
**  y - vertical distance
**	direction - 0 = UP , 1 = RIGHT, 2 = DOWN, 3 = LEFT
**	snake_length - initially = 3 , increases with every food item.
*/

typedef enum { false, true } bool;

bool in_game = true;
int level = 1, score = 0, x = 49, y = 22, direction = 0, snake_length = 3, i, j;

int position = 0;

int check_switches(int);

void main(void) {
    init();
	if(in_game == false){
		display_menu();
	}else{
		begin_game();
	}
	
}

void init(void) {

	os_init();
	os_add_task(check_switches, 100, 1 );
}

void display_menu(void) {
	display_string("\n");
	display_string(" --------------------------------------------------- \n");
	display_string(" Level 0              SNAKE                Score: 0  \n");
	display_string(" --------------------------------------------------- \n");
	display_string(" *************************************************** \n");
	uint8_t i;
	for(i=0;i<10;i++){
		display_string(" *                                                 * \n");
	}
	display_string(" *                   To play                       * \n");
	display_string(" *                press centre                     * \n");
	for(i=0;i<10;i++){
		display_string(" *                                                 * \n");
	}
	display_string(" *************************************************** \n");
	display_string("\n");
}

void display_end(void) {

	char score_string[2];
	itoa(score, score_string, 10);

	display_string("\n");
	display_string(" --------------------------------------------------- \n");
	display_string(" Level 0              SNAKE                Score: 0  \n");
	display_string(" --------------------------------------------------- \n");
	display_string(" *************************************************** \n");
	uint8_t i;
	for(i=0;i<10;i++){
		display_string(" *                                                 * \n");
	}
	display_string(" *                   THE END                       * \n");
	display_string(" *                press centre                     * \n");
	display_string(" *                                                 * \n");
	display_string(" *                YOUR SCORE:");
	display_string(score_string);
	display_string("                     * \n");
	for(i=0;i<10;i++){
		display_string(" *                                                 * \n");
	}
	display_string(" *************************************************** \n");
	display_string("\n");
}

void display_top(void) {
	char level_string[2];
	char score_string[2];

	itoa(level, level_string, 10);
	itoa(score, score_string, 10);

	display_string("\n");
	display_string(" --------------------------------------------------- \n");
	display_string(" Level ");
	display_string(level_string);
	display_string("              SNAKE                Score: ");
	display_string(score_string);
	display_string(" \n");
	display_string(" --------------------------------------------------- \n");
	display_string(" *************************************************** \n");
}

void display_bottom(void) {
	display_string(" *************************************************** \n");
	display_string("\n");
}

int check_switches(int state) {

	if(get_switch_press(_BV(SWN))) {
		direction = 0;		
	}

	if(get_switch_press(_BV(SWE))) {
		direction = 1;
	}

	if(get_switch_press(_BV(SWS))) {
		direction = 2;
	}

	if(get_switch_press(_BV(SWW))) {
		direction = 3;
	}

	if(get_switch_short(_BV(SWC))){
		in_game = !in_game;
	}

	return state;
}

void begin_game(void) {

	/* Grid = 49x * 22y */
	

	int grid[x][y];  

	/*Initialise Grid*/

	for(i = 0; i<x; i++){
		for(j =0; j<y; j++){
			if(i==32 && j==5){
				grid[i][j] = -1;
			}else if(i==25 && j==11){
				grid[i][j] = 1;
			}else if(i==25 && j==12){
				grid[i][j] = 2;
			}else if(i==25 && j==13){
				grid[i][j] = 3;
			}else{
				grid[i][j] = 0;
			}
		}
	}

	display_grid(grid);

	sei();
	
	
	for(;;){
		for(;;){
			move(grid);
			display_grid();
			if(!in_game){
				break;
			}
		}
		for(;;){
			clear_screen();
			display_end();
			if(in_game){
				level=1;
				score = 0;
				snake_length = 3;
				break;
			}
		}
	}; 
}

int move(int grid[x][y]){

	clear_screen();

	for(i=0;i<x;i++){
		for(j=0;j<y;j++){
			
			if(grid[i][j] > 0){
				grid[i][j]++; /*Increment all positive numbers in grid*/
			}
			
			if(grid[i][j] > snake_length){
				grid[i][j] = 0;
			}
		}
	}
	for(i=0;i<x;i++){
		for(j=0;j<y;j++){
			if(grid[i][j] == 2){
				if(direction == 0){

					if(grid[i][j-1] == -1){
						eat(grid);
					}
					if(grid[i][j-1] > 0){
						die();
					}
					grid[i][j-1] = 1;
					
				}else if(direction == 1){

					if((i+1)>x){
						i = i % x;
					}
					if(grid[i+1][j] == -1){
						eat(grid);
					}
					if(grid[i+1][j] > 0){
						die();
					}
					grid[i+1][j] = 1;
					
					

				}else if(direction == 2){

					if(grid[i][j+1] == -1){
						eat(grid);
					}
					if(grid[i][j+1] > 0){
						die();
					}
					grid[i][j+1] = 1;					

				}else if(direction == 3){

					if((i-1)<0){
						i = x;
					}
					if(grid[i-1][j] == -1){
						eat(grid);
					}
					if(grid[i-1][j] > 0){
						die();
					}
					grid[i-1][j] = 1;
				}
			}
		}
	}

	return grid;

}

void eat(int grid[x][y]){
	snake_length++;
	score++;
	//generate randomly placed food (Which isn't in the snake)
	
	int rx, ry;
	do{
		rx = rand() % 49;
		ry = rand() % 22;
	}while(grid[rx][ry] > 0);
	
	grid[rx][ry] = -1;

}

void die(void){
	in_game = !in_game;
}



void display_grid(int grid[x][y]){

	uint8_t i, j;

	display_top();

	
	for(j=0;j<y;j++){

		display_string(" *");

		for(i=0;i<x;i++){

			char grid_element[2] = " ";

			/*if(grid[i][j]>0){
				itoa(grid[i][j], grid_element, 10);
			}*/

			if(grid[i][j]==-1){
				display_string("#");
			}else if(grid[i][j] == 1){
				display_string("S");
			}else if(grid[i][j] == 2){
				display_string("N");
			}else if(grid[i][j]>2 && grid[i][j]<snake_length){
				display_string("E");
			}else if(grid[i][j] == snake_length){
				display_string("K");
			}else{
				display_string(grid_element);
			}
			
		}

		display_string("* \n");

	}

	display_bottom();
}


