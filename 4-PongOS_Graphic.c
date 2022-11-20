/*********************************************************************
*																	**
*		Author	  : Ahmet KARACALI									**
*		Due Date  : 19.03.2017										**
*																	**
*																	**
*	Problem:	Writing a pong game in Kernel Graphics.				**
*																	**
*																	**
*																	**
**********************************************************************/


#include "console.h"
#include "memory.h"
#include "process.h"
#include "interrupt.h"
#include "keyboard.h"
#include "mouse.h"
#include "clock.h"
#include "ata.h"
#include "cdromfs.h"
#include "string.h"
#include "graphics.h"
#include "ascii.h"
#include "syscall.h"
#include "rtc.h"
#include "kernelcore.h"
#include "kmalloc.h"
#include "memorylayout.h"

#define MainSIZE 3
#define UP                   'w'
#define DOWN                 's'
#define ENTER                13
#define LEFT1                 '4'
#define RIGHT1                '6'
#define LEFT2                 'a'
#define RIGHT2                'd'
#define ESC 27
#define SPACE 32

int gameWinner=0;
int playerscore=0;
int ball_x_prev;
int pausegame=0,continuegame=1,quit=0;
int stack=0,lstack=0;

int EASY=800000,MEDIUM=500000,HARD=300000,VETERAN=80000;
int level=800000; //standard value is 800K

struct graphics_color color_black = {0,0,0,0};
struct graphics_color color_white = {255,255,255,0};
struct graphics_color color_blue = {255,200,5,7};
struct graphics_color color_another = {255,100,58,58};
struct graphics_color color_ball = {255,255,255,50};

#define RACKETLEN 100
static int racket1_x = 290;
static int racket1_y = 40;
static int racket2_x = 290;
static int racket2_y = 400;
static int ball_x= 150, ball_y = 200;
struct graphics *g;

int ball_xMove=1,ball_yMove=1;

int racket1[RACKETLEN];
int racket2[RACKETLEN];

void prepareGame()
{
  racket1_x = 290;
  racket1_y = 40;
  racket2_x = 290;
  racket2_y = 400;
  ball_x= 150, ball_y = 200;
  gameWinner=0;
  pausegame=0;
  continuegame=1;
  graphics_clear(g, 0, 0, graphics_width(g), graphics_height(g));
  drawBoard();
  drawRackets();
  drawBall(ball_x,ball_y, color_ball);
}

void findRackets() // This function finds the racket's coordinates and saves them in an array.
{
    int i;

    for(i=0;i<RACKETLEN;i++)
    {
        racket1[i]=racket1_x+i;
        racket2[i]=racket2_x+i;
    }
}

int checkReturn() //This function checks return for racket1.
{
    int returned=0,i;

        for(i=0;i<RACKETLEN;i++)
        {
            if(ball_x==racket1[i]) returned=1;
        }

    return returned;
}

int checkReturn2() //This function checks return for racket2.
{
    int returned=0,i;

        for(i=0;i<RACKETLEN;i++)
        {
            if(ball_x==racket2[i]) returned=1;
        }

    return returned;
}


void DrawFilledCircle(struct graphics *g,int x0, int y0, int radius)
{
    int x = radius;
    int y = 0;
    int xChange = 1 - (radius << 1);
    int yChange = 0;
    int radiusError = 0;

    while (x >= y)
    {
        for (int i = x0 - x; i <= x0 + x; i++)
        {
            put_pixel(g, i, y0 + y);
            put_pixel(g, i, y0 - y);
        }
        for (int i = x0 - y; i <= x0 + y; i++)
        {
            put_pixel(g, i, y0 + x);
            put_pixel(g, i, y0 - x);
        }

        y++;
        radiusError += yChange;
        yChange += 2;
        if (((radiusError << 1) + xChange) > 0)
        {
            x--;
            radiusError += xChange;
            xChange += 2;
        }
    }
}

int abs(int n)
{
	if (n<0) return -n;
	return n;
} 

void DrawLine(struct graphics *g, int x0, int y0, int x1, int y1) {
 
  int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
  int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1; 
  int err = (dx>dy ? dx : -dy)/2, e2;
 
  for(;;){
    put_pixel(g,x0,y0);
    if (x0==x1 && y0==y1) break;
    e2 = err;
    if (e2 >-dx) { err -= dy; x0 += sx; }
    if (e2 < dy) { err += dx; y0 += sy; }
  }
}

void DrawRectangle(struct graphics *g,int x,int y,int w,int h)
{
	DrawLine(g, x  , y  , x+w, y);
	DrawLine(g, x  , y+h, x+w, y+h);
	DrawLine(g, x  , y  , x  , y+h);
	DrawLine(g, x+w, y  , x+w, y+h);  
	DrawLine(g, 600, 10  , 600, 430);
	DrawLine(g, 10, 430  , 600, 430);
}

void drawBoard()
{
	graphics_fgcolor(g, color_another);
	DrawRectangle(g, 10, 10, video_xres-20, video_yres-20);
}

void drawRackets()
{
	graphics_fgcolor(g, color_blue);
	DrawLine(g,racket1_x, racket1_y, racket1_x+RACKETLEN,racket1_y);			
	DrawLine(g,racket2_x, racket2_y, racket2_x+RACKETLEN,racket2_y);			

}

void drawBall(int x, int y, struct graphics_color c)
{
	graphics_fgcolor(g, c);
	DrawFilledCircle(g, x, y, 10);
}

void menuMain()
{
	char pressed;
	do
	{
	
		if(kbhit())
		{
		pressed=keyboard_read();
		
		if(pressed==DOWN)
		   {
			if(stack==3);
			else stack++;
		   }
		else if(pressed==UP)
		   {
			if(stack==0);
			else stack--;
		   }
		else if(pressed==13)
		   {
			if(stack==0) { playGameSinglePlayer(); break;}
			else if(stack==1) { playGameMulti(); break; }
			else if(stack==2) { writeText(); break; }
			else if(stack==3) { graphics_fgcolor(g, color_another); quit=1;  break; }
		   }
		if(quit!=1) printMain(stack);
		}
	
	}while(quit!=1);
	printf("\n\nGoodbye");
}

void printMain(mstack)
{
	g = graphics_create_root();
	console_init(g);
	graphics_clear(g, 0, 0, graphics_width(g), graphics_height(g));
	graphics_fgcolor(g, color_blue);
	printf("_________________________________________________________________________\n");
	printf("|                                                                       |\n");
	printf("|   **********    ***********   ***     ***    **********               |\n");
	printf("|   **      **    ***     ***   ****    ***    **      **               |\n");
	printf("|   **      **    ***     ***   *****   ***    **                       |\n");
	printf("|   **      **    ***     ***   *** **  ***    **********               |\n");
	printf("|   **********    ***     ***   ***  ** ***    **      **               |\n");
	printf("|   **            ***     ***   ***   *****    **      **               |\n");
	printf("|   **            ***     ***   ***    ****    **********               |\n");
	printf("|   **            ***********   ***     ***    **********               |\n");
	printf("|                                                                       |\n");
	printf("|   **********        ***         ****            ****    ***********   |\n");
	printf("|   **      **       *****        *****          *****    ***********   |\n");
	printf("|   **              **   **       **  **        **  **    ***           |\n");
	printf("|   **********     **     **      **   **      **   **    *******       |\n");
	printf("|   **      **    ***********     **    **    **    **    ***           |\n");
	printf("|   **      **   **         **    **     **  **     **    ***           |\n");
	printf("|   **********  **           **   **      ****      **    ***********   |\n");
	printf("|   ********** ***           ***  **       **       **    ***********   |\n");
	printf("|                                                                       |\n");
	printf("|_______________________________________________________________________|\n\n");
	printf("                                                                         \n\n");
	printf("                                                           Ahmet Karacali\n\n");
	if(mstack==0)
	{
	graphics_fgcolor(g, color_blue);
	printf("Single Player");printf("\n");
	graphics_fgcolor(g, color_white);
	printf("Multiplayer");printf("\n");
	printf("Instructions");printf("\n");
	printf("Quit");printf("\n");
	
	}
	if(mstack==1)
	{	graphics_fgcolor(g, color_white);
	printf("Single Player"); printf("\n");
	graphics_fgcolor(g, color_blue);
	printf("Multiplayer");printf("\n");
	graphics_fgcolor(g, color_white);
	printf("Instructions");printf("\n");
	printf("Quit");printf("\n");
	}
	if(mstack==2)
	{
	graphics_fgcolor(g, color_white);
	printf("Single Player"); printf("\n");
	printf("Multiplayer");printf("\n");
	graphics_fgcolor(g, color_blue);
	printf("Instructions");printf("\n");
	graphics_fgcolor(g, color_white);
	printf("Quit");printf("\n");
	}
	if(mstack==3)
	{
	graphics_fgcolor(g, color_white);
	printf("Single Player"); printf("\n");
	printf("Multiplayer");printf("\n");
	printf("Instructions");printf("\n");
	graphics_fgcolor(g, color_blue);
	printf("Quit");printf("\n");
	}
}

void writeText()
{
	char c;
	
	g = graphics_create_root();
	console_init(g);
	graphics_clear(g, 0, 0, graphics_width(g), graphics_height(g));
	graphics_fgcolor(g, color_blue);
	printf("_________________________________________________________________________\n");
	printf("|                                                                       |\n");
	printf("|   **  **     **  ********** ********* ********   **      **           |\n");
	printf("|   **  ***    **  **      **    ***    **      *  **      **           |\n");
	printf("|   **  ****   **  **             *     **      *  **      **           |\n");
	printf("|   **  ** **  **  **********     *     ** *****   **      **           |\n");
	printf("|   **  **  ** **  **********     *     **  **     **      ** --------- |\n");
	printf("|   **  **   ****          **     *     **   **    **      **           |\n");
	printf("|   **  **    ***  **      **     *     **    **   **      **           |\n");
	printf("|   **  **     **  **********     *     **     **   ********            |\n");
	printf("|                                                                       |\n");
	printf("|     *********  *********  **  ******   **      **  ***********        |\n");
	printf("|    **********     ***        ********  ***     **  **       **        |\n");
	printf("|   **               *      ** **    **  ****    **  **                 |\n");
	printf("|  **                *      ** **    **  ** **   **  ***********        |\n");
	printf("|  **                *      ** **    **  **  **  **           **        |\n");
	printf("|   **               *      ** **    **  **   ** **  **       **        |\n");
	printf("|    **********      *      ** ********  **    ****  ***********        |\n");
	printf("|     *********      *      **  ******   **     ***  ***********        |\n");
	printf("|                                                                       |\n");
	printf("|_______________________________________________________________________|\n\n");
	printf("                                                                         \n\n");
	printf("                                                           Ahmet Karacali\n\n");


	printf("   RACKET1 Controls:                     Racket2 Controls:        \n");
	printf("                                                                  \n");
	printf("         A <--LEFT                             7 <--LEFT          \n");
	printf("         D -->RIGHT                            9 -->RIGHT         \n");
	printf("   MENU HANDLING KEYS:                                            \n");
	printf("                                                                  \n");
	printf("         W -->UP                                                  \n");
	printf("         S -->DOWN                                                \n");
	printf("     ENTER -->EXECUTE                                             \n");
	printf("                                                                  \n");
	printf("                                                                  \n");
	printf("   PRESS SPACE TWICE TO SEE THE MAIN MENU.                              \n");

	do{
	c=keyboard_read();
	}while(c!=SPACE);
	if(c==SPACE) menuMain();
}

void initGame()
{
	graphics_clear(g, 0, 0, graphics_width(g), graphics_height(g));
	drawBoard();
	drawRackets();
        drawBall(ball_x,ball_y, color_ball);
}

void DelaySome(unsigned int m)
{
	unsigned int i;

	for (i=0; i<m; i++);
}

void gameDelay(int level)
{ 
	if(lstack==0) level=EASY;
	if(lstack==1) level=MEDIUM;
	if(lstack==2) level=HARD;
	if(lstack==3) level=VETERAN;

	DelaySome(level);
	
}

void move_racket(int *x, int *y, int dir)
{
	graphics_fgcolor(g, color_black);
	DrawLine(g,*x, *y, *x+RACKETLEN,*y);			
	if (dir == 1)  	/* LEFT */
		*x = *x - 10;
	else		/* RIGHT */
		*x = *x + 10;	
	graphics_fgcolor(g, color_blue);
	DrawLine(g,*x, *y, *x+RACKETLEN,*y);			
}

void move_racket2(int *x, int *y, int dir)
{
	graphics_fgcolor(g, color_black);
	DrawLine(g,*x, *y, *x+RACKETLEN,*y);			
	if (dir == 1)  	/* LEFT */
		*x = *x - 10;
	else		/* RIGHT */
		*x = *x + 10;	
	graphics_fgcolor(g, color_blue);
	DrawLine(g,*x, *y, *x+RACKETLEN,*y);			
}

void move_ball()
{
	int condition=1;
	ball_x_prev=ball_x;
	drawBall(ball_x,ball_y,color_black);
	if(ball_x>=580)
        ball_xMove *= -1;
    	if(ball_x<=22)
        ball_xMove *= -1;


	if(ball_y==388)
    	{
        condition=checkReturn2();	
        	if(condition==1)
        	{
            	ball_yMove*=-1;
        	}
        	else
        	{
		gameWinner=2;
		continuegame=0;
        	}
    	}

      if(ball_y==53)
   	 {	
       		 condition=checkReturn();
       		 if(condition==1)
        	{
           	 ball_yMove*=-1;
		playerscore+=5;
        	}
    	    	else
      	 	 {
			gameWinner=1;
			continuegame=0;
      		 }
    	}


    ball_x += ball_xMove;
    ball_y += ball_yMove;

    drawBall(ball_x,ball_y,color_ball);
}

void initKernel()
{
	g = graphics_create_root();
	console_init(g);
	memory_init();
	kmalloc_init((char*)KMALLOC_START,KMALLOC_LENGTH);
	interrupt_init();
	rtc_init();
	clock_init();
	mouse_init();
	keyboard_init();
	process_init();
	graphics_clear(g, 0, 0, graphics_width(g), graphics_height(g));
}


//---------------------------------------------------------------SINGLEPLAYER START--------------------------------------------//

int whereBallGoes()
{
    int i,result,xCoordinate;
    
    result = ball_y - 47;
    
    xCoordinate=ball_x;
    
    if (ball_x_prev>ball_x)
    {
    for(i=0;i<result;i++)
    {
        xCoordinate-=1;
    }
    }
    
    if (ball_x_prev<ball_x)
    {
    for(i=0;i<result;i++)
    {
        xCoordinate+=1;
    }
    }
    
    return xCoordinate;
}


int pcMove()
{
    int right=0,left=0,condition,i;
    
    condition=whereBallGoes();
    
    for(i=0;i<RACKETLEN;i++)
    {
    if(condition>racket1[i]) { right=1; left=0; }
    if (condition<racket1[i]) { right=0; left=1; }
    if (condition==racket1[i]) { right=0; left=0; break;}
    }
    
    if(right==1)
    {
        {if(racket1_x<500 ) move_racket(&racket1_x,&racket1_y,0); }
            right=0;
            left=0;
    }
    if(left==1)
    {
       { if(racket1_x>20 ) move_racket(&racket1_x,&racket1_y,1); }
            right=0;
            left=0;
    }
    

}


int manageGameSinglePlayer()
{
     char c;
    
	gameDelay(level);
        if (kbhit()) 
	{
            c = keyboard_read();
		
	    if (c == ESC)  pausegame=1;

	    else if (c == 'a')    /* '7' character */
	     { if(racket2_x>20 ) move_racket(&racket2_x,&racket2_y,1); }

	    else if (c == 'd')    /* '9' character */
	     { if(racket2_x<500 ) move_racket(&racket2_x,&racket2_y,0); }

        }
        else if (continuegame!=1) return 1;

	if(ball_yMove==-1)  pcMove();

	findRackets();
    	move_ball();
}

void askLevel()
{
	char pressed;
	do
	{
	
		if(kbhit())
		{
		pressed=keyboard_read();
		
		if(pressed==DOWN)
		   {
			if(lstack==3);
			else lstack++;
		   }
		else if(pressed==UP)
		   {
			if(lstack==0);
			else lstack--;
		   }
		else if(pressed==13)
		   {
			if(lstack==0) { level=0; break;}
			else if(lstack==1) { level=1; break; }
			else if(lstack==2) { level=2; break; }
			else if(lstack==3) { level=3; break; }
		   }
		printLevel(lstack);
		}
	
	}while(1);
}

void printLevel(lstack)
{
	g = graphics_create_root();
	console_init(g);
	graphics_clear(g, 0, 0, graphics_width(g), graphics_height(g));
	graphics_fgcolor(g, color_blue);
	printf("_________________________________________________________________________\n");
	printf("|                                                                       |\n");
	printf("|   **********    ***********   ***     ***    **********               |\n");
	printf("|   **      **    ***     ***   ****    ***    **      **               |\n");
	printf("|   **      **    ***     ***   *****   ***    **                       |\n");
	printf("|   **      **    ***     ***   *** **  ***    **********               |\n");
	printf("|   **********    ***     ***   ***  ** ***    **      **               |\n");
	printf("|   **            ***     ***   ***   *****    **      **               |\n");
	printf("|   **            ***     ***   ***    ****    **********               |\n");
	printf("|   **            ***********   ***     ***    **********               |\n");
	printf("|                                                                       |\n");
	printf("|   **********        ***         ****            ****    ***********   |\n");
	printf("|   **      **       *****        *****          *****    ***********   |\n");
	printf("|   **              **   **       **  **        **  **    ***           |\n");
	printf("|   **********     **     **      **   **      **   **    *******       |\n");
	printf("|   **      **    ***********     **    **    **    **    ***           |\n");
	printf("|   **      **   **         **    **     **  **     **    ***           |\n");
	printf("|   **********  **           **   **      ****      **    ***********   |\n");
	printf("|   ********** ***           ***  **       **       **    ***********   |\n");
	printf("|                                                                       |\n");
	printf("|_______________________________________________________________________|\n\n");
	printf("                                                                         \n\n");
	printf("                                                           Ahmet Karacali\n\n");
	if(lstack==0)
	{
	graphics_fgcolor(g, color_blue);
	printf("EASY");printf("\n");
	graphics_fgcolor(g, color_white);
	printf("MEDIUM");printf("\n");
	printf("HARD");printf("\n");
	printf("VETERAN");printf("\n");
	
	}
	if(lstack==1)
	{	graphics_fgcolor(g, color_white);
	printf("EASY");printf("\n");
	graphics_fgcolor(g, color_blue);
	printf("MEDIUM");printf("\n");
	graphics_fgcolor(g, color_white);
	printf("HARD");printf("\n");
	printf("VETERAN");printf("\n");
	}
	if(lstack==2)
	{
	graphics_fgcolor(g, color_white);
	printf("EASY");printf("\n");
	printf("MEDIUM");printf("\n");
	graphics_fgcolor(g, color_blue);
	printf("HARD");printf("\n");
	graphics_fgcolor(g, color_white);
	printf("VETERAN");printf("\n");
	}
	if(lstack==3)
	{
	graphics_fgcolor(g, color_white);
	printf("EASY");printf("\n");
	printf("MEDIUM");printf("\n");
	printf("HARD");printf("\n");
	graphics_fgcolor(g, color_blue);
	printf("VETERAN");printf("\n");
	}
}

void playGameSinglePlayer()
{
    printLevel(lstack);
    askLevel();
    initGame();
    prepareGame();

    while (continuegame==1) 
 {
        
        if(pausegame!=1)
        {
        manageGameSinglePlayer();
        }
        else if(pausegame==1)
        {
             pausing();
             break;
        }
 }
    endGameSingleplayer();
}

void endGameSingleplayer()
{
  	int key;
    	g = graphics_create_root();
	console_init(g);
	graphics_clear(g, 0, 0, graphics_width(g), graphics_height(g));
    	
	graphics_fgcolor(g, color_blue);


	printf("____________________________________________________________________________\n");
	printf("|                                                                          |\n");
	printf("|   **********        ***         ****            ****    ***********      |\n");
	printf("|   **      **       *****        *****          *****    ***********      |\n");
	printf("|   **              **   **       **  **        **  **    ***              |\n");
	printf("|   **********     **     **      **   **      **   **    *******          |\n");
	printf("|   **      **    ***********     **    **    **    **    ***              |\n");
	printf("|   **      **   **         **    **     **  **     **    ***              |\n");
	printf("|   **********  **           **   **      ****      **    ***********      |\n");
	printf("|   ********** ***           ***  **       **       **    ***********      |\n");
	printf("|                                                                          |\n");
	printf("|                                                                          |\n");
	printf("|   *********** **           ** *********  *******          ***            |\n");
	printf("|   *********** **           ** ********   **    **        *****           |\n");
	printf("|   **       **  **         **  **         **    **        *****           |\n");
	printf("|   **       **   **       **   ******     *******         *****           |\n");
	printf("|   **       **    **     **    ******     *****            ***            |\n");
	printf("|   **       **     **   **     **         ** ***           ***            |\n");
	printf("|   **       **      *****      ********   **  ***                         |\n");
	printf("|   ***********       ***       *********  **   ***         ***            |\n");
	printf("|                                                            *             |\n");
	printf("|__________________________________________________________________________|\n");
	printf("                                                                         \n\n");
	printf("                                                           Ahmet Karacali\n\n");


	if(gameWinner==1){
	printf("   PLAYER 1 WINS!                                                          \n\n");
	printf("   YOUR SCORE:%d\n\n",playerscore);
	printf("   PRESS SPACE TWICE TO SEE MAIN MENU    \n");
	
	}
	
     else if(gameWinner==2)
    {
	printf("   COMPUTER WINS!                                                          \n\n");
	printf("   YOUR SCORE:%d\n\n",playerscore);
	printf("   PRESS SPACE TWICE TO SEE MAIN MENU                                             \n");
	
    }

	do{
	key=keyboard_read();
	}while(key!=SPACE);
	if(key==SPACE) menuMain();
}

//---------------------------------------------------------------SINGLEPLAYER DONE--------------------------------------------//



//****************************** |FOR MULTIPLAYER|***************************//

int pausing()
{
	int key;
    	g = graphics_create_root();
	console_init(g);
	graphics_clear(g, 0, 0, graphics_width(g), graphics_height(g));
    	
	graphics_fgcolor(g, color_blue);


	printf("____________________________________________________________________________\n");
	printf("|                                                                          |\n");
	printf("|   **********        ***         ****            ****    ***********      |\n");
	printf("|   **      **       *****        *****          *****    ***********      |\n");
	printf("|   **              **   **       **  **        **  **    ***              |\n");
	printf("|   **********     **     **      **   **      **   **    *******          |\n");
	printf("|   **      **    ***********     **    **    **    **    ***              |\n");
	printf("|   **      **   **         **    **     **  **     **    ***              |\n");
	printf("|   **********  **           **   **      ****      **    ***********      |\n");
	printf("|   ********** ***           ***  **       **       **    ***********      |\n");
	printf("|                                                                          |\n");
	printf("|                                                                          |\n");
	printf("|   **********        ***        **     ** *********  *********   ******   |\n");
	printf("|   **      **       *****       **     ** **     **  *******     **    *  |\n");
	printf("|   **      **      **   **      **     ** **         *           **     * |\n");
	printf("|   **      **     **     **     **     ** *********  ******      **     * |\n");
	printf("|   **********    ***********    **     **        **  **          **     * |\n");
	printf("|   **           **         **   **     ** **     **  **          **     * |\n");
	printf("|   **          **           **  ********* *********  ********    **    *  |\n");
	printf("|   **         ***           ***  *******  *********  *********   ******   |\n");
	printf("|                                                                          |\n");
	printf("|__________________________________________________________________________|\n");
	printf("                                                                         \n\n");
	printf("                                                           Ahmet Karacali\n\n");

	printf("                                                                            \n");
	printf("   PRESS SPACE TWICE TO SEE MAIN MENU                                             \n");
	do{  	
	key= keyboard_read();
	
	if(key==SPACE) continuegame=1; pausegame=0; break;
	}while(key!= SPACE);
menuMain(); 
}


void playGameMulti()
{
	initGame();
	prepareGame();

    while (continuegame==1) 
    {
        manageGameMultiplayer();
    
            if(pausegame!=1)
        {
         manageGameMultiplayer();
        }
            else if(pausegame==1)
        {
             pausing();
             break;
        }
        
    }
    endGameMultiplayer();
}



int manageGameMultiplayer()
{
    char c;
    
	gameDelay(level);
        if (kbhit()) 
	{
            c = keyboard_read();
		
	    if (c == ESC)  pausegame=1;

	    else if (c == '7')    /* 'a' character */
	     { if(racket1_x>20 ) move_racket(&racket1_x,&racket1_y,1); }
	    else if (c == '9')    /* 'd' character */
	      {if(racket1_x<500 ) move_racket(&racket1_x,&racket1_y,0); }
	    else if (c == 'a')    /* '7' character */
	     { if(racket2_x>20 ) move_racket(&racket2_x,&racket2_y,1); }
	    else if (c == 'd')    /* '9' character */
	     { if(racket2_x<500 ) move_racket(&racket2_x,&racket2_y,0); }

        }
        else if (continuegame!=1) return 1;
	findRackets();
    move_ball();
    return -1;
}



void endGameMultiplayer()
{
  	int key;
    	g = graphics_create_root();
	console_init(g);
	graphics_clear(g, 0, 0, graphics_width(g), graphics_height(g));
    	
	graphics_fgcolor(g, color_blue);


	printf("____________________________________________________________________________\n");
	printf("|                                                                          |\n");
	printf("|   **********        ***         ****            ****    ***********      |\n");
	printf("|   **      **       *****        *****          *****    ***********      |\n");
	printf("|   **              **   **       **  **        **  **    ***              |\n");
	printf("|   **********     **     **      **   **      **   **    *******          |\n");
	printf("|   **      **    ***********     **    **    **    **    ***              |\n");
	printf("|   **      **   **         **    **     **  **     **    ***              |\n");
	printf("|   **********  **           **   **      ****      **    ***********      |\n");
	printf("|   ********** ***           ***  **       **       **    ***********      |\n");
	printf("|                                                                          |\n");
	printf("|                                                                          |\n");
	printf("|   *********** **           ** *********  *******          ***            |\n");
	printf("|   *********** **           ** ********   **    **        *****           |\n");
	printf("|   **       **  **         **  **         **    **        *****           |\n");
	printf("|   **       **   **       **   ******     *******         *****           |\n");
	printf("|   **       **    **     **    ******     *****            ***            |\n");
	printf("|   **       **     **   **     **         ** ***           ***            |\n");
	printf("|   **       **      *****      ********   **  ***                         |\n");
	printf("|   ***********       ***       *********  **   ***         ***            |\n");
	printf("|                                                            *             |\n");
	printf("|__________________________________________________________________________|\n");
	printf("                                                                         \n\n");
	printf("                                                           Ahmet Karacali\n\n");


	if(gameWinner==1){
	printf("   PLAYER 1 WINS!                                                          \n\n");
	printf("   PRESS SPACE TWICE TO SEE MAIN MENU                                             \n");
	}
	
     else if(gameWinner==2)
    {
	printf("   PLAYER 2 WINS!                                                          \n\n");
	printf("   PRESS SPACE TWICE TO  SEE MAIN MENU                                             \n");
    }

	do{
	key=keyboard_read();
	}while(key!=SPACE);
	if(key==SPACE) menuMain();
}

//****************************** |***************|***************************//

int kernel_main()
{
	initKernel();
	writeText();
	printMain(stack);
	menuMain();
	while (1); /* kernel should never end */
	return 0;
}