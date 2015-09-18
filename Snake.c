//Snake.cpp
//Classic Snake Game
//Written by Joseph Dykstra
/* Compiled with Notepad++ and NppExec,
using MinGW (32-bit) and SDL (32-bit).
The following commands were used:
	cd $(CURRENT_DIRECTORY)
	gcc -std=c99 -o $(NAME_PART).exe $(FILE_NAME) -lmingw32 -lSDLmain -lSDL -lSDL_ttf -mwindows
	$(CURRENT_DIRECTORY)\$(NAME_PART).exe
*/

//#define DEBUG
#include <stdio.h>		//Untested
#include <stdbool.h>	//Untested
//#include <string.h>	//IDK
#include <windows.h>	// Message Box
#include <math.h>		// floor()
//#include <conio.h>	//IDK
//#include <stdlib.h>	//IDK
#include <time.h>		//time for random
#include "SDL\SDL.h"
#include "SDL\SDL_ttf.h"
#include "lib\EasySDL.h"
#define C_SNAKE					0x0000FF	//Deep Blue
#define C_FOOD					0x00C800	//Light Green
#define C_BACK					0xE1E1E1	//Light Gray
#define NO_MODES				5
#define BLOCK_SIZE				40
#define WIDTH_BLOCKS			16
#define HEIGHT_BLOCKS			12
#define SCR_WIDTH				(WIDTH_BLOCKS*BLOCK_SIZE)
#define SCR_HEIGHT				(HEIGHT_BLOCKS*BLOCK_SIZE)
#define SCR_BPP					32
#define BTN_X					BLOCK_SIZE
#define BTN_Y(num)				((num*SCR_HEIGHT)/NO_MODES)
#define BTN_WIDTH				4*BLOCK_SIZE
#define BTN_HEIGHT				((HEIGHT_BLOCKS*BLOCK_SIZE)/(NO_MODES+1))
#define LF						1
#define UP						2
#define RT						3
#define DN						4
#define modeName(n)				((n==0)?"Easy":(n==1)?"Medium":(n==2)?"Hard":(n==3)?"Death":"lol")	//not proper :(


const Uint32 modeColor[NO_MODES]=	//Button colors
	{
	0x00C800,
	0x00C8C8,
	0xFF7F00,
	0xFF0000,
	0xFFFFFF
	};


int firstx,firsty,lastx,lasty;
int selection,length,grow,startlength;
int dir=RT;
int score=0;
int board[WIDTH_BLOCKS+1][HEIGHT_BLOCKS+1];
float speedup, delaytime;
bool foodate=false;
bool dead=false;
bool stop=false;
SDL_Surface *screen=NULL;
SDL_Rect drawrect;
TTF_Font *font=NULL;


void quit(void)
	{
	TTF_Quit();
	SDL_Quit();
	}


int set(void)
	{
	if (!SDL_Init(SDL_INIT_VIDEO))
		{
		//Allow cout
#ifdef DEBUG
		freopen( "CON", "w", stdout );
		freopen( "CON", "w", stderr );
#endif
		//Graphics
		SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY,SDL_DEFAULT_REPEAT_INTERVAL);
		screen = SDL_SetVideoMode(SCR_WIDTH,SCR_HEIGHT,SCR_BPP,SDL_SWSURFACE);
		
		//OPEN TTF STUFF
		if( TTF_Init() == -1 )
			return -2;
		
		font = TTF_OpenFont( "arial.ttf", 28 );
		if( font == NULL )
			return -3;
		
		atexit(quit);
		}
	else
		return -1;
	return 0;
	}


void reset()
	{
	SDL_WM_SetCaption("Snake | Joseph Dykstra", NULL);

	SDL_Event tEvent;
	int tSelection=-1, tX, tY, tClickSelection;
	char appName[50];
	sprintf(appName,"Tetris");
	while (tSelection==-1)
		{
		for (int i=0; i<NO_MODES; i++)
			{
			EZ_apply_rect(screen, EZ_new_rect(BTN_X, BTN_Y(i), BTN_WIDTH, BTN_HEIGHT), modeColor[i]);
			
			EZ_apply_text(screen,modeName(i),font, EZ_new_rect(BLOCK_SIZE*2,BTN_Y(i)+(BLOCK_SIZE/2),0,0), EZ_Uint32_to_SDL(0x000000));
			}
		SDL_Flip(screen);
		
		while(SDL_PollEvent(&tEvent))
			{
			if (tEvent.type==SDL_MOUSEBUTTONDOWN)
				{
				if (tEvent.button.x>BTN_X && tEvent.button.x<BTN_WIDTH+BTN_X)
					{
					for (int i=0; i<=NO_MODES; i++)
						{
						if ( tEvent.button.y > BTN_Y(i) && tEvent.button.y < BTN_Y(i)+BTN_HEIGHT ) //
							tClickSelection=i;
						}
					}
				}
			
			if (tEvent.type==SDL_MOUSEBUTTONUP)
				{
				if (tEvent.button.x>BTN_X && tEvent.button.x<BTN_WIDTH+BTN_X)
					{
					for (int i=0; i<=NO_MODES; i++)
						{
						if ( tEvent.button.y > BTN_Y(i) && tEvent.button.y < BTN_Y(i)+BTN_HEIGHT ) //
							{
							if (tClickSelection==i)
								tSelection=i;
							}
						}
					}
				}
			
			if (tEvent.type==SDL_QUIT)
				exit(0);
				
			}
		}
	
	switch (tSelection)
		{
		case 0:
			grow=2;
			startlength=3;
			speedup=0.975;
			delaytime=300;
			break;
		case 1:
			grow=3;
			startlength=3;
			speedup=0.9375;
			delaytime=250;
			break;
		case 2:
			grow=4;
			startlength=3;
			speedup=0.9;
			delaytime=200;
			break;
		case 3:
			grow=10;
			startlength=10;
			speedup=1;
			delaytime=150;
			break;
		default:
			grow=3;
			startlength=3;
			speedup=0.9375;
			delaytime=250;
			break;
		}
	srand(time(0));
	foodate=0;
	dead=0;
	dir=RT;
	score=0;
	drawrect.x = 0;
	drawrect.y = 0;
	length=startlength;
	firstx=startlength-1;
	firsty=lastx=lasty=0;
	for (int x=0;x<WIDTH_BLOCKS;x++)
		{
		for (int y=0;y<HEIGHT_BLOCKS;y++) //CLEAR BOARD
			{
			board[x][y]=0; //(rand()%3)%2;
			}
		}
	if (startlength>WIDTH_BLOCKS) startlength=WIDTH_BLOCKS;
	for	(int x=0;x<startlength;x++) //MAKE SNAKE
		{
		board[x][0]=x+1;
		}
	}


void makeFood(void)
	{
	int n=1;
	while (n) //MAKE FOOD
		{
		int x=rand()%(WIDTH_BLOCKS-1)+1;
		int y=rand()%(HEIGHT_BLOCKS-1)+1;
		if (board[x][y]==0) {n=0; board[x][y]=-1;}
		}
	foodate=0;
	dead=0;
	}

void snakeMove(void)
	{
	int n=0;
	SDL_Event test_event;
	while(SDL_PollEvent(&test_event)) //GET INPUT
		{
		if (test_event.type == SDL_QUIT)
			{
			exit(0);
			}
		if (test_event.key.state == SDL_PRESSED)
			{
			switch (test_event.key.keysym.sym)
				{
				case SDLK_DOWN:	n=DN; break;
				case SDLK_RIGHT: n=RT; break;
				case SDLK_UP:		n=UP; break;
				case SDLK_LEFT:	n=LF; break;
				}
			}
		}
#ifdef DEBUG
	printf("1dir %d\n",dir);
	printf("1n	%d\n",n);
#endif
	if ((n==LF) || (n==UP) || (n==RT) || (n==DN)) //PROCESS INPUT
		{
		if((n==LF && dir!=RT) ||
			 (n==RT && dir!=LF) ||
			 (n==DN && dir!=UP) ||
			 (n==UP && dir!=DN))	 dir=n;
		}
#ifdef DEBUG
	printf("2dir %d\n",dir);
	printf("2n	%d\n",n);
#endif
	int tempx=firstx;
	int tempy=firsty;
	if (dir==LF) tempx--;
	if (dir==UP) tempy--;
	if (dir==RT) tempx++;
	if (dir==DN) tempy++;
	if	(tempx<0 || tempx>=WIDTH_BLOCKS || tempy<0 || tempy>=HEIGHT_BLOCKS)
		{
		dead = 1;
		foodate = 1;
		}
	else
		{
		n=board[tempx][tempy]; //Out of array?????
		if (n>1) dead=1;
		if	(n<=1)	board[tempx][tempy]=length+1;
		if	(n==-1) foodate=1;
		}
#ifdef DEBUG
	printf("tempx %d\n",tempx);
	printf("tempy %d\n",tempy);
	if (dead==0) 
	printf("alive\n");
	printf("length %d\n",length);
#endif
	if (dead==0) //GROW
		{
		int high = startlength-1;
		//length=0;
		for	(int x=0;x<WIDTH_BLOCKS;x++)
			{
			for	(int y=0;y<HEIGHT_BLOCKS;y++)
				{
				n=board[x][y];
				if (n>high) {firstx=x;firsty=y;high=n;}
				if (n==1)	 {lastx=x;lasty=y;}
				if (n>=1)	 {board[x][y]--;}//length++;}
				}
			}
		}
#ifdef DEBUG
	printf(" firstx=%d, firsty=%d, lastx=%d, lasty=%d\n\n",firstx,firsty,lastx,lasty);
#endif
	}


void snakeDraw(void)
	{
	SDL_FillRect(screen,NULL,C_BACK);
	drawrect.h = BLOCK_SIZE;
	drawrect.w = BLOCK_SIZE;
	for (int y=0;y<HEIGHT_BLOCKS;y++)
		{
		for (int x=0;x<WIDTH_BLOCKS;x++)
			{
			if (board[x][y]>0) //Snake
				{
				drawrect.x = x*BLOCK_SIZE;
				drawrect.y = y*BLOCK_SIZE;
				SDL_FillRect(screen , &drawrect , C_SNAKE);
				}
			if (board[x][y]<0) //Food
				{
				drawrect.x = x*BLOCK_SIZE;
				drawrect.y = y*BLOCK_SIZE;
				SDL_FillRect(screen , &drawrect , C_FOOD);
				}
			}
		}
	SDL_Flip(screen);
	}


void snakeGrow(void)
	{
	if (dead==0)
		{
		for (int y=0;y<HEIGHT_BLOCKS;y++)
			{
			for (int x=0;x<WIDTH_BLOCKS;x++)
				{
				if	(board[x][y]>=1)	{board[x][y]+=grow;}
				}
			}
		score+=1;
		length+=grow;
		delaytime*=speedup;
		floor(delaytime);
		char caption[30];
		sprintf(caption,"SNAKE | Score:%d | Joseph Dykstra",score);
		SDL_WM_SetCaption(caption,"Snake");
		}
	}


void gameOver(void)
	{
	if (!stop)
		stop=(MessageBox(MB_APPLMODAL,"Play Again?","GAME OVER",MB_YESNO|MB_ICONQUESTION)==IDNO);
#ifdef DEBUG
	cout<<"stop="<<stop;
#endif
	}


int main(int argc, char *argv[])
	{
	int go=set();
	if (go==0)
		{
		while (!stop)
			{
			reset();
			while (!dead && !stop) //WHILE: ALIVE
				{
				makeFood();
				while (!dead && !foodate && !stop) //WHILE: FOOD NOT EATEN
					{
					snakeDraw();
					for (int i=0; i<delaytime && !stop; i++)
						SDL_Delay(1); //WAIT
					snakeMove();
					}
					snakeGrow();
				}
			gameOver();
			}
		}
	SDL_Quit();
	return go;
	}


/* ISSUES - BUGS

does the snake speed up?
create custom mode?

*/



/*
void intro()
	{
	cout<<" /-----\\  " <<"|\\      |        /\\        |   /  |-----" <<endl;
	cout<<"(         "  <<"| \\     |       /  \\       |  /   |     " <<endl;
	cout<<" \\        " <<"|  \\    |      /    \\      | /    |     " <<endl;
	cout<<"  \\--\\    "<<"|   \\   |     /------\\     |(     |-----" <<endl;
	cout<<"      \\   " <<"|    \\  |    /        \\    | \\    |     "<<endl;
	cout<<"       )  "  <<"|     \\ |   /          \\   |  \\   |     "<<endl;
	cout<<"\\-----/   " <<"|      \\|  /            \\  |   \\  |-----"<<endl;
	cout<<endl;
	cout<<"|\\    /|   /\\   |--\\ |--"<<"    |--\\ \\  / "<<"   --|--  /--\\  /--\\ |-- |-\\ |  |    |--\\"<<endl;
	cout<<"| \\  / |  /--\\  |   ||--" <<"    |--<  \\/  " <<"     |   (    ) \\--\\ |-- |-/ |--|    |  |"  <<endl;
	cout<<"|  \\/  | /    \\ |--/ |--" <<"    |--/  |    " <<"  \\-|    \\--/  \\--/ |-- |   |  |    |--/."  <<endl;
	cout<<endl;
	cout<<" \\    /\\    / -|- --|-- |  |    --|--  /--\\  /--\\ -|-   /\\   |  |    | /\n";
	cout<<"  \\  /  \\  /   |    |   |--|      |   (    ) \\--\\  |   /--\\  |--|    |< \n";
	cout<<"   \\/    \\/   -|-   |   |  |    \\-|    \\--/  \\--/ -|- /    \\ |  |    | \\.\n\n";
	cout<<"Controls: WASD"<<endl;
	cout<<"Choose:" <<endl;
	cout<<"1"<<setw(8)<<"Easy"  <<endl;
	cout<<"2"<<setw(8)<<"Medium"<<endl;
	cout<<"3"<<setw(8)<<"Hard"  <<endl;
	cout<<"4"<<setw(8)<<"Death" <<endl;
	cout<<"5"<<setw(8)<<"Custom"<<endl;
	}
*/