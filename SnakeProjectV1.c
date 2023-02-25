//ID 208631572
//NAME LIOR NATAN
//SNAKE PROJECT A

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <stdbool.h>

#define UP 72  // Up Arrow 
#define DOWN 80 // Down Arrow 
#define LEFT 75  // Left Arrow 
#define RIGHT 77  // Right Arrow 
#define ESC 27  // Escape Key
#define SNAKESIZE 10 // starting snake size
#define MIDSIZEX 40 // starting point X
#define MIDSIZEY 12 // starting point Y
#define ROWS 25 // Game Rows
#define COLS 75 // Game Cols
#define INTERVALS 1000 // time difference between move

typedef struct snakeDot { // snake is made from snakeDots
	int x; // x coordinate
	int y; // y coordinate
	struct snakeDot* nextDot; // connected Dot
}snakeDot;

enum gameResults {
	GAME_LOST,
	GAME_WON,
	GAME_RESET
};
void gotoxy(int y, int x);
int getKey();
void prepareBoard(); // create board template
void snakeStep(snakeDot* snake, int x, int y); // snake moves one unit
snakeDot* prepareSnake(); // prepare starting snake
snakeDot* createDot(int x, int y, snakeDot* nextDot); // create a snake cell 
int startGame(snakeDot* snake); // game starting main function
void spawnRandomApple(snakeDot* snake, int* appleX, int* appleY); // spawns random apple and checks for no collision
void printWin(); // prints a game winning message
void printLose(); // prints a game losing message
bool checkCollision(snakeDot* snake, int x, int y); // check if any dots connected to snake are in x,y coordinates
void removeSnake(snakeDot* snake); // removes snake cell and all other snakeDots connected
int gameMenu(); // game selection menu

void main()
{
	int gameMode = gameMenu(); // print menu
	int gameResult;
	if (gameMode != 0) // no exit
	{
		srand(time(NULL));
		snakeDot* snake = prepareSnake(); // creates starting snake
		prepareBoard(); // prepares board
		gameResult = startGame(snake); // starts the game and returns the game status
		while (gameResult == GAME_RESET && gameMode != 0) // game was stopped with ESC
		{
			system("cls");
			gameMode = gameMenu();
			if (gameMode == 1)
			{
				removeSnake(snake);
				snake = prepareSnake();
				prepareBoard();
				gameResult = startGame(snake);
			}
		}
		if (gameResult == GAME_WON) // game won
			printWin();
		else if (gameResult == GAME_LOST) // game lost
			printLose();
		else // exit
		{
			system("cls");
			printf("Goodbye.");
		}
		removeSnake(snake); // frees dynamic memory of all dots connected to snake
	}

}

snakeDot* createDot(int x, int y, snakeDot* nextDot)
{
	snakeDot* snake = malloc(sizeof(snakeDot)); // allocate memory for new node
	if (snake)
	{
		snake->x = x; // set x
		snake->y = y; // set y
		snake->nextDot = nextDot; // sets next Node's address
	}
	return snake;
}

void removeSnake(snakeDot* snake)
{
	if (snake->nextDot != NULL)
		removeSnake(snake->nextDot); // frees next node
	free(snake);
}
snakeDot* prepareSnake()
{
	snakeDot* tempSnake = NULL;
	for (int i = SNAKESIZE - 1; i >= 0; i--) // tail to head
		tempSnake = createDot(MIDSIZEX - i, MIDSIZEY, tempSnake);
	return tempSnake; // returns snakes head
}

void snakeStep(snakeDot* snake, int x, int y)
{
	if (snake != NULL)
	{
		snakeStep(snake->nextDot, snake->x, snake->y); // make a move on next node
		snake->x = x;
		snake->y = y;
		gotoxy(x, y);
		printf("@");
	}
	else // tail of snake
	{
		gotoxy(x, y);
		printf(" ");
	}
}

void prepareBoard() {
	system("cls");
	int i;
	for (i = 0; i < COLS; i++)
		printf("#");
	for (i = 0; i < ROWS - 1; i++)
	{
		printf("\n#");
		for (int j = 1; j < COLS - 1; j++)
			printf(" ");
		printf("#");
	}
	printf("\n");
	for (i = 0; i < COLS; i++)
		printf("#");
}

int startGame(snakeDot* snake) {

	char c;
	char lastMove;
	int x = MIDSIZEX, y = MIDSIZEY;  // x for coulmn and y for line 
	int appleX, appleY;
	int applesLeft = 10;
	spawnRandomApple(snake, &appleX, &appleY);
	// start from the middle of the screen
	gotoxy(x, y);
	// the direction is Right untill an arrow hit
	c = RIGHT;
	lastMove = RIGHT;
	while (c != ESC)
	{
		// sleeps for a second before continuing to the next command
		Sleep(INTERVALS);

		// only if a key was hit we read what key code it was
		if (_kbhit()) // if any key was hit
		{
			c = getKey();  // change direction
			if (c != RIGHT && c != LEFT && c != ESC && c != UP && c != DOWN) // button is not valid
				c = lastMove; // keep direction
		}
		switch (c) // adjust direction based on button
		{
		case RIGHT:
			x++;
			break;
		case DOWN:
			y++;
			break;
		case LEFT:
			x--;
			break;
		case UP:
			y--;
			break;
		}
		lastMove = c;
		gotoxy(appleX, appleY); // reprint apple
		printf("$");
		if (x == 0 || x == COLS - 1 || y == ROWS || y == 0) // snake hits a wall
			return GAME_LOST;
		else if (x == appleX && y == appleY) // snake eats an apple
		{
			applesLeft--;
			if (applesLeft == 0) // 10 apple were eaten
				return GAME_WON;
			snake = createDot(x, y, snake); // increase snake size (creates another cell)
			gotoxy(x, y);
			printf("@"); // prints cell location
			spawnRandomApple(snake, &appleX, &appleY); // spawns a random new apple
		}
		else
		{
			if (checkCollision(snake->nextDot, x, y)) // checks for collision with snake body
				return GAME_LOST;
			snakeStep(snake, x, y); // create a step based on new coordinates

		}
		gotoxy(26, 81);
	}
	return GAME_RESET; // ESC was pressed
}

void spawnRandomApple(snakeDot* snake, int* x, int* y)
{
	*x = ((rand() % (COLS - 1)) + 1); // randomize an apple location
	*y = ((rand() % (ROWS - 1)) + 1);
	if (checkCollision(snake, &x, &y)) // check that snake is not on the coordinates
		spawnRandomApple(snake, &x, &y);
}

void printWin() {
	system("cls");
	printf("Congratulations, You Win!");
}

void printLose() {
	system("cls");
	printf("Unfortunately, You Lost.");
}

bool checkCollision(snakeDot* snake, int x, int y) {
	if (snake == NULL) // no snake no collision
		return false;
	if (checkCollision(snake->nextDot, x, y) || snake->x == x && snake->y == y) // check the snake and the next cell
		return true;
	return false; // no collisions were found
}

int gameMenu() {
	printf("Welcome To Snake!\nPlease Choose an Option:\n\n1.Easy Mode\n2.Medium Mode\n3.Hard Mode\n\n0.Exit\n");
	int choice = -1;
	scanf("%d", &choice);
	switch (choice) {
	case 0:
		return 0;
	case 1:
		return 1;
		break;
	case 2:
		system("cls");
		break;
	case 3:
		system("cls");
		break;
	default:
		system("cls");
		printf("Please enter a valid choice.\n");
		break;
	}
	return gameMenu();
}

// This function moves the cursor to the line y and column x.
// (0,0) is the upper left corner of the screen.
void gotoxy(int y, int x) {
	printf("\x1b[%d;%df", x + 1, y + 1);
}

// This function gets keyCodes for special keys such as arrows , escape and others
// It returns the Key code such as 72 for arrow up, 27 for Escape... 
int getKey()
{
	char KeyStroke = _getch();
	if (KeyStroke == 0 || KeyStroke == -32)
		KeyStroke = _getch();
	return (KeyStroke);
}
