// Author  : Ahmet KARACALI
// Due Date: 19.03.2017
// Problem : Writing Pong Game on Kernel base.
// Instructions:
//		RACKET 1 : <-- A D -->
//		RACKET 2 : <-- 7 9 -->


#include "screen.h"
#include "pic.h"
#include "keyboard.h"
#include "mem.h"
#include <string.h>

#define UP                  'w'
#define DOWN                's'
#define ENTER               13
#define LEFT2               'a'
#define RIGHT2              'd'
#define ESC 		     27
#define SPACE 		     32

#define MAX_X	80
#define MAX_Y	25

#define RACKETLEN 14
static int racket1_x = 36;
static int racket1_y = 1;
static int racket2_x = 36;
static int racket2_y = 22;
static int ball_x= 39, ball_y = 3;

int ball_xMove=1,ball_yMove=1;

int racket1[RACKETLEN]={36,37,38,39,40,41,42,43,44,45,46,47,48,49,50};
int racket2[RACKETLEN]={36,37,38,39,40,41,42,43,44,45,46,47,48,49,50};

int gameWinner=0;
int playerscore=0;
int ball_x_prev;
int pausegame=0,continuegame=1,quit=0;
int stack=0,lstack=0;


int level=1;


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
		else if(pressed=='q')
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
clear();

	println("   PONG GAME                                                             \n");
	println("                                                                         \n\n");
	println("                                                           Ahmet Karacali\n\n");
	if(lstack==0)
	{
	println("EASY <--\n");
	println("MEDIUM\n");
	println("HARD\n");
	println("VETERAN\n");
	
	}
	if(lstack==1)
	{
	println("EASY\n");

	println("MEDIUM <--\n");

	println("HARD\n");
	println("VETERAN\n");
	}
	if(lstack==2)
	{

	println("EASY\n");
	println("MEDIUM\n");

	println("HARD <--\n");

	println("VETERAN\n");
	}
	if(lstack==3)
	{

	println("EASY\n");
	println("MEDIUM\n");
	println("HARD\n");

	println("VETERAN <--\n");
	}
}

void prepareGame()
{
  racket1_x = 36;
  racket1_y = 1;
  racket2_x = 36;
  racket2_y = 22;
  ball_x= 34, ball_y = 10;
  gameWinner=0;
  pausegame=0;
  continuegame=1;
  clear();
  drawBoard();
  drawRackets();
  drawBall(ball_x,ball_y,'O');
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
		else if(pressed=='q')
		   {
			if(stack==0) { playGameSinglePlayer(); break;}
			else if(stack==1) { playGameMulti(); break; }
			else if(stack==2) { writeText(); break; }
			else if(stack==3) { quit=1;  break; }
		   }
		if(quit!=1) printMain(stack);
		}
	}while(quit!=1);
	println("\n\nGoodbye");
}

void printMain(mstack)
{
	clear();
	println("   PONG GAME                                                             \n");
	println("                                                                         \n\n");
	println("                                                           Ahmet Karacali\n\n");
	
	if(mstack==0)
	{
	println("Single Player <--");println("\n");
	println("Multiplayer");println("\n");
	println("Instructions");println("\n");
	println("Quit");println("\n");
	}
	if(mstack==1)
	{
	println("Single Player"); println("\n");
	println("Multiplayer <--");println("\n");
	println("Instructions");println("\n");
	println("Quit");println("\n");
	}
	if(mstack==2)
	{

	println("Single Player"); println("\n");
	println("Multiplayer");println("\n");
	println("Instructions <--");println("\n");
	println("Quit");println("\n");
	}
	if(mstack==3)
	{
	println("Single Player"); println("\n");
	println("Multiplayer");println("\n");
	println("Instructions");println("\n");
	println("Quit <--");println("\n");
	}
}

void writeText()
{
	char c;
	
clear();

	println("   PONG GAME                                                             \n");
	println("                                                                         \n\n");
	println("                                                           Ahmet Karacali\n\n");


	println("   RACKET1 Controls:                     Racket2 Controls:        \n");
	println("                                                                  \n");
	println("         A <--LEFT                             7 <--LEFT          \n");
	println("         D -->RIGHT                            9 -->RIGHT         \n");
	println("   MENU HANDLING KEYS:                                            \n");
	println("                                                                  \n");
	println("         W -->UP                                                  \n");
	println("         S -->DOWN                                                \n");
	println("         Q -->EXECUTE                                             ");
	println("                                                                  ");
	println("                                                                  ");
	println("   PRESS SPACE TWICE TO SEE THE MAIN MENU.                        \n");

	do{
	c=keyboard_read();
	}while(c!=SPACE);
	if(c==SPACE) menuMain();
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


void drawBoard()
{
	int i;

	setpos(0, 0);
        for (i=0; i< MAX_X; i++)
		putch('#');
	for (i=1; i< (MAX_Y-2); i++) {
		setpos(0,i);
		putch('#');
		setpos(MAX_X-1,i);
		putch('#');
	}
	setpos(0,MAX_Y-2);
        for (i=0; i< MAX_X; i++)
		putch('#');
}

void drawRackets()
{
	int i;
	setpos(racket1_x,racket1_y);
	for (i=0; i<RACKETLEN; i++)
		putch('=');
	setpos(racket2_x,racket2_y);
	for (i=0; i<RACKETLEN; i++)
		putch('=');
}

void drawBall(int x, int y, char c)
{
	setpos(x,y);
	putch(c);
}

void initGame()
{
	clear();
	drawBoard();
	drawRackets();
        drawBall(ball_x,ball_y,'O');
}

void DelaySome(unsigned int m)
{
	unsigned int i;

	for (i=0; i<m; i++);
}

void gameDelay()
{
	if(level==0) DelaySome(9000000);
	if(level==1) DelaySome(7000000);
	if(level==2) DelaySome(5000000);
	if(level==3) DelaySome(3000000);
}

void move_racket(int *x, int *y, int dir)
{
	int i;
	setpos(*x,*y);
	for (i=0; i< RACKETLEN; i++)
		putch(' ');
	if (dir == 1)  	/* LEFT */
		*x = *x - 3;
	else		/* RIGHT */
		*x = *x + 3;	
	setpos(*x, *y);
	for (i=0; i< RACKETLEN; i++)
		putch('=');
}


void move_ball()
{

	int condition=1;
	ball_x_prev=ball_x;
	
	drawBall(ball_x,ball_y,' ');

    	if(ball_x>=75)
        ball_xMove *= -1;
    	if(ball_x<=2)
        ball_xMove *= -1;


	if(ball_y==21)
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

      if(ball_y==2)
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

     drawBall(ball_x,ball_y,'O');


}

void initKernel()
{
   init_pic();
   init_keyboard();
   update_cursor(81, 26);
   clear();
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
        {if(racket1_x<64 ) move_racket(&racket1_x,&racket1_y,0); }
            right=0;
            left=0;
    }
    if(left==1)
    {
       { if(racket1_x>3 ) move_racket(&racket1_x,&racket1_y,1); }
            right=0;
            left=0;
    }
    

}

int manageGameSinglePlayer()
{
     char c;
    
	gameDelay();
        if (kbhit()) 
	{
            c = keyboard_read();
		
	    if (c == 'a')    /* '7' character */
	     { if(racket2_x>3 ) move_racket(&racket2_x,&racket2_y,1); }

	    else if (c == 'd')    /* '9' character */
	     { if(racket2_x<63 ) move_racket(&racket2_x,&racket2_y,0); }

        }
        else if (continuegame!=1) return 1;

	if(ball_yMove==-1)  pcMove();

	findRackets();
    	move_ball();
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

 }
    endGameSingleplayer();
}

void endGameSingleplayer()
{
  	int key;

	clear();

	if(gameWinner==1)
	{
	println("   PLAYER 1 WINS!                                                          \n\n");
	println("   YOUR SCORE IS: "); putint(playerscore);
	println("\n PRESS SPACE TWICE TO SEE MAIN MENU    \n");
	
	}
	
     else if(gameWinner==2)
    {
	println("   COMPUTER WINS!                                                          \n\n");
	println("   YOUR SCORE IS: "); setpos(17,3); putint(playerscore);
	println("\n  PRESS SPACE TWICE TO SEE MAIN MENU                                             \n");
	setpos(19,3); putch(' '); putch(' ');
    }

	do{
	key=keyboard_read();
	}while(key!=SPACE);
	if(key==SPACE) menuMain();
}

//---------------------------------------------------------------SINGLEPLAYER DONE--------------------------------------------//


/******************************************* FOR MULTIPLAYER **********************************************/

int manageGameMultiplayer()
{
    char c;
    
	gameDelay();
        if (kbhit()) 
	{
            c = keyboard_read();
		
	    if (c == '7')    /* 'a' character */
		if(racket1_x>3 ) move_racket(&racket1_x,&racket1_y,1);
	    if (c == '9')    /* 'd' character */
		if(racket1_x<63 ) move_racket(&racket1_x,&racket1_y,0);

	    if (c == 'a')    /* '7' character */
		if(racket2_x>3 ) move_racket(&racket2_x,&racket2_y,1);
	    if (c == 'd')    /* '9' character */
		if(racket2_x<63 ) move_racket(&racket2_x,&racket2_y,0);

        }
        else if (continuegame!=1) return 1;
	findRackets();
    move_ball();
    return -1;
}


void playGameMulti()
{
    prepareGame();
    initGame();
    
    while (continuegame==1) 
    {
        manageGameMultiplayer();
    
    }
    endGameMultiplayer();
}

void endGameMultiplayer()
{
  	int key;

	clear();
	
	if(gameWinner==1){
	println("   PLAYER 1 WINS!                                                          \n\n");
	println("   PRESS SPACE TWICE TO SEE MAIN MENU                                             \n");
	}
	
     else if(gameWinner==2)
    {
	println("   PLAYER 2 WINS!                                                          \n\n");
	println("   PRESS SPACE TWICE TO  SEE MAIN MENU                                             \n");
    }

	do{
	key=keyboard_read();
	}while(key!=SPACE);
	if(key==SPACE) menuMain();
}


void kmain(void* mbd, unsigned int magic )
{
	initKernel();
	writeText();
	printMain(stack);
	menuMain();
	while (1); /* kernel should never end */
}