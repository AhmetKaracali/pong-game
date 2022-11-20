/*
*       Author: Ahmet KARACALI
*     Due Date: 10.03.2017
*
*
*   Subject:
*       Write a C program to play pong game in C Graphics mode.
*
*
*/

#include <graphics.h>
#include <time.h>

#define RACKETLEN 50
#define RACKETWIDTH 5
#define UP 'w'
#define DOWN 's'
#define ENTER 13
#define ESC 27
#define SPACE 32

static int racket1_x = 20;
static int racket1_y = 280;
static int racket2_x = 770-RACKETWIDTH;
static int racket2_y = 280;
static int ball_x= 400, ball_y = 145;
static int ballSize = 5;
static int RacketColor = 2;
static int BallGNDColor = 0;
static int BallColor = 15;

int racket1[RACKETLEN];
int racket2[RACKETLEN];

int ball_xMove=1,ball_yMove=1;
int ball_y_prev;

int scoreP1=0,scoreP2=0; // Keeps scores of players,when someone reaches 3,the program prints the winner.
int raund=1; // It cannot be higher than six.

int EASY=10,MEDIUM=5,EXPERT=3,VETERAN=1;
int DELAY;
int menuStack=0,levelStack=0;


void findRackets() // This function finds the racket's coordinates and saves them in an array.
{
    int i;

    for(i=0;i<RACKETLEN;i++)
    {
        racket1[i]=racket1_y+i;
        racket2[i]=racket2_y+i;
    }
}
void drawBoard(int color) // This function prints the board.
{
	setcolor(color);
	rectangle (10,30, 780, 580);
}

void DrawLine(int color, int x0, int y0, int x1, int y1,int thickness) // DrawLine function.
{

  while(thickness>=1)
  {
      int tx0,ty0,tx1,ty1;
      tx0=x0;tx1=x1;ty0=y0;ty1=y1;
      int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
      int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1;
      int err = (dx>dy ? dx : -dy)/2, e2;

      for(;;){
        putpixel(x0, y0, color);
        //put_pixel(g,x0,y0);
        if (x0==x1 && y0==y1) break;
        e2 = err;
        if (e2 >-dx) { err -= dy; x0 += sx; }
        if (e2 < dy) { err += dx; y0 += sy; }
      }
      x0=tx0+1;
      y0=ty0;
      x1=tx1+1;
      y1=ty1;
      thickness--;
  }
}

void DrawFilledCircle(int x0, int y0, int radius, int color) // This Function prints the ball as a circle.
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
            putpixel(i, y0 + y,color);
            putpixel(i, y0 - y,color);
        }
        for (int i = x0 - y; i <= x0 + y; i++)
        {
            putpixel(i, y0 + x,color);
            putpixel(i, y0 - x,color);
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

void drawRackets(int color) // this function prints the rackets.
{
	DrawLine(color,racket1_x,racket1_y,racket1_x,racket1_y+RACKETLEN,RACKETWIDTH);
	DrawLine(color,racket2_x,racket2_y,racket2_x,racket2_y+RACKETLEN,RACKETWIDTH);
}

void drawTop() // This function prints the top navigation.
{
    setcolor(14);
    outtextxy(30, 5, "Pong Game !");

    if(scoreP1==1 && scoreP2==0)  outtextxy(380, 5, "1 - 0");
    if(scoreP1==1 && scoreP2==1)  outtextxy(380, 5, "1 - 1");
    if(scoreP1==1 && scoreP2==2)  outtextxy(380, 5, "1 - 2");
    if(scoreP1==1 && scoreP2==3)  outtextxy(380, 5, "1 - 3");
    if(scoreP1==0 && scoreP2==0)  outtextxy(380, 5, "0 - 0");
    if(scoreP1==0 && scoreP2==1)  outtextxy(380, 5, "0 - 1");
    if(scoreP1==0 && scoreP2==2)  outtextxy(380, 5, "0 - 2");
    if(scoreP1==0 && scoreP2==3)  outtextxy(380, 5, "0 - 3");
    if(scoreP1==2 && scoreP2==0)  outtextxy(380, 5, "2 - 0");
    if(scoreP1==2 && scoreP2==1)  outtextxy(380, 5, "2 - 1");
    if(scoreP1==2 && scoreP2==2)  outtextxy(380, 5, "2 - 2");
    if(scoreP1==2 && scoreP2==3)  outtextxy(380, 5, "2 - 3");
    if(scoreP1==3 && scoreP2==0)  outtextxy(380, 5, "3 - 0");
    if(scoreP1==3 && scoreP2==1)  outtextxy(380, 5, "3 - 1");
    if(scoreP1==3 && scoreP2==2)  outtextxy(380, 5, "3 - 2");

    if(raund==1) outtextxy(680, 5, "Raund: 1");
    if(raund==2) outtextxy(680, 5, "Raund: 2");
    if(raund==3) outtextxy(680, 5, "Raund: 3");
    if(raund==4) outtextxy(680, 5, "Raund: 4");
    if(raund==5) outtextxy(680, 5, "Raund: 5");
    if(raund==6) outtextxy(680, 5, "Raund: 6");
}

void initGame() //This function prepares the start of the game.
{
	cleardevice();
	drawBoard(11);
	drawRackets(RacketColor);
	drawTop();
}

void move_racket(int *x, int *y, int dir) // This function moves the racket.
{
    drawRackets(0);
	if (dir == 1)  	/* LEFT */
		*y = *y - 5;
	else		/* RIGHT */
		*y = *y + 5;
	drawRackets(RacketColor);
}

int checkReturn() //This function checks return for racket1.
{
    int returned=0,i;

        for(i=0;i<RACKETLEN;i++)
        {
            if(ball_y==racket1[i]) returned=1;
        }

    return returned;
}
int checkReturn2() //This function checks return for racket2.
{
    int returned=0,i;

        for(i=0;i<RACKETLEN;i++)
        {
            if(ball_y==racket2[i]) returned=1;
        }

    return returned;
}

void move_ball() // This function helps to move the ball.
{
    int condition;
    int randomY;

    srand(time(NULL));

    if(ball_y>=575-ballSize)
        ball_yMove *= -1;
    if(ball_y<=38)
        ball_yMove *= -1;

	DrawFilledCircle(ball_x,ball_y,ballSize,BallGNDColor);

    if(ball_x==755)
    {
        condition=checkReturn2();
        if(condition==1)
        {
            ball_xMove*=-1;
        }
        else
        {
            scoreP1++;
            raund++;
            ball_x = 400;
            randomY= rand()%300+50;
            ball_y = randomY;
            ball_xMove*=-1;
        }
    }

        if(ball_x==30)
    {
        condition=checkReturn();
        if(condition==1)
        {
            ball_xMove*=-1;
        }
        else
        {
            scoreP2++;
            raund++;
            ball_x = 400;
            randomY= rand()%300+50;
            ball_y = randomY;
            ball_xMove*=-1;
        }
    }


    ball_x += ball_xMove;
    ball_y += ball_yMove;

	DrawFilledCircle(ball_x,ball_y,ballSize,BallColor);
	drawTop();
}

void playGameMultiplayer() // This function contains a loop which helps to play the pong game.
{
    char c;
    initGame();
    while(scoreP1!=3 && scoreP2!=3)
        {
        if (kbhit())
        {
            c = getch();
	    if (c == 'w')
        {
            if(racket1_y>35)
            {
                move_racket(&racket1_x,&racket1_y,1);
            }
        }
        if (c == 's')
        {
            if(racket1_y<520)
            {
             move_racket(&racket1_x,&racket1_y,0);
            }
        }
	    if (c == '8')   /* 'w' character */
        {
            if(racket2_y>35)
            {
             move_racket(&racket2_x,&racket2_y,1);
            }
        }

	    if (c == '5')   /* 's' character */

        {
            if(racket2_y<520)
            {
                move_racket(&racket2_x,&racket2_y,0);
            }
        }

        }
        findRackets();
        delay(DELAY);
       if (scoreP1!=3 || scoreP2!=3)
        {
		move_ball();
        }

    }
       if (scoreP1==3) setcolor(5); outtextxy(300, 200, "Player 1 Wins The Game !");
        if (scoreP2==3) setcolor(5); outtextxy(300, 200, "Player 2 Wins The Game !");
}

//******************************** SINGLEPLAYER START ******************************//

int whereBallGoes()
{
    int i,result,yCoordinate;

    result = 755 - ball_x;

    yCoordinate=ball_y;

    if (ball_y_prev>ball_y)
    {
    for(i=0;i<result;i++)
    {
        yCoordinate-=1;
    }
    }

    if (ball_y_prev<ball_y)
    {
    for(i=0;i<result;i++)
    {
        yCoordinate+=1;
    }
    }

    return yCoordinate;
}


int pcMove()
{
    int up=0,down=0,condition,i;

    condition=whereBallGoes();

    for(i=0;i<RACKETLEN;i++)
    {
    if(condition>racket2[i]) { up=0; down=1; }
    if (condition<racket2[i]) { up=1; down=0; }
    if (condition==racket2[i]) { up=0; down=0; break;}
    }

    if(up==1) // UP
    {
          if(racket2_y>35)
            {
             move_racket(&racket2_x,&racket2_y,1);
            }
            up=0;
            down=0;
    }
    if(down==1) // down
    {
       if(racket2_y<520)
       {
                move_racket(&racket2_x,&racket2_y,0);
       }
            up=0;
            down=0;
    }


}


int manageGameSinglePlayer()
{
     char c;
while(scoreP1!=3 && scoreP2!=3)
{
	delay(DELAY);
        if (kbhit())
	{
           c = getch();

	    if (c == 'w')
        {
            if(racket1_y>35)
            {
                move_racket(&racket1_x,&racket1_y,1);
            }
        }
        if (c == 's')
        {
            if(racket1_y<520)
            {
             move_racket(&racket1_x,&racket1_y,0);
            }
        }

    }


    if (scoreP1!=3 || scoreP2!=3)
    {
		move_ball();
    }
	if(ball_xMove==1)  pcMove();
    findRackets();
}

}

//******************************* SINGLEPLAYER END **********************//


//********************************************* MENU **************************************//


void printLevel(void)
{
    if(levelStack==0)
    {
    setcolor(5);
    outtextxy(450, 200, "> EASY <");
    setcolor(9);
    outtextxy(450, 220, "  MEDIUM  ");
    outtextxy(450, 240, "  EXPERT  ");
    outtextxy(450, 260, "  VETERAN  ");
    }
        if(levelStack==1)
    {
    setcolor(9);
    outtextxy(450, 200, "  EASY  ");
    setcolor(5);
    outtextxy(450, 220, "> MEDIUM <");
    setcolor(9);
    outtextxy(450, 240, "  EXPERT  ");
    outtextxy(450, 260, "  VETERAN  ");
    }
        if(levelStack==2)
    {
    setcolor(9);
    outtextxy(450, 200, "  EASY  ");
    outtextxy(450, 220, "  MEDIUM  ");
    setcolor(5);
    outtextxy(450, 240, "> EXPERT <");
    setcolor(9);
    outtextxy(450, 260, "  VETERAN  ");
    }
        if(levelStack==3)
    {
    setcolor(9);
    outtextxy(450, 200, "  EASY  ");
    outtextxy(450, 220, "  MEDIUM  ");
    outtextxy(450, 240, "  EXPERT  ");
    setcolor(5);
    outtextxy(450, 260, "> VETERAN <");
    }

}

int checkLevel()
{
int input;
int quit=0;
    while(quit!=1)
    {
        if(kbhit())
        {
            input=getch();

            if(input==UP)
            {
            if(levelStack>0) levelStack--;
            }
            if(input==DOWN)
            {
            if(levelStack<3) levelStack++;
            }
            if(input==ENTER)
            {

                if(levelStack==0) { DELAY=EASY;  initGame(); manageGameSinglePlayer(); break; }
                if(levelStack==1) { DELAY=MEDIUM;  initGame(); manageGameSinglePlayer(); break; }
                if(levelStack==2) { DELAY=EXPERT;  initGame(); manageGameSinglePlayer();break; }
                if(levelStack==3) { DELAY=VETERAN; initGame(); manageGameSinglePlayer();break; }
            }
            if(input=='a')
            {
                cleardevice();
                drawBoard(11);
                levelStack=0;
                return 1;
                break;

            }
            printLevel();
        }
    }
}


void instructions()
{
    cleardevice();
    drawBoard(11);
    setcolor(5);
    outtextxy(240, 160, "****** INSTRUCTIONS ******");
    setcolor(9);
    outtextxy(200, 180, "  WHO REACHES 3 POINT FIRST,WÝNS!");
    outtextxy(200, 200, "  YOU CAN USE W,S,A and ENTER for MENU.  ");
    outtextxy(200, 220, "  W and S for RACKET 1  ");
    outtextxy(200, 240, "  8 and 5 for RACKET 2 ( only multiplayer ) ");
    setcolor(14);
    outtextxy(240, 280, "  PRESS ANY KEY TO CONTINUE.  ");

    getch();
    cleardevice();
    drawBoard(11);
}
void printMenu(void)
{
    if(menuStack==0)
    {
    setcolor(5);
    outtextxy(300, 200, "> Single Player <");
    setcolor(9);
    outtextxy(300, 220, "  Multiplayer  ");
    outtextxy(300, 240, "  Instructions  ");
    outtextxy(300, 260, "  Quit  ");
    }
        if(menuStack==1)
    {
    setcolor(9);
    outtextxy(300, 200, "  Single Player  ");
    setcolor(5);
    outtextxy(300, 220, "> Multiplayer <");
    setcolor(9);
    outtextxy(300, 240, "  Instructions  ");
    outtextxy(300, 260, "  Quit  ");
    }
        if(menuStack==2)
    {
    setcolor(9);
    outtextxy(300, 200, "  Single Player  ");
    outtextxy(300, 220, "  Multiplayer  ");
    setcolor(5);
    outtextxy(300, 240, "> Instructions <");
    setcolor(9);
    outtextxy(300, 260, "  Quit  ");
    }
        if(menuStack==3)
    {
    setcolor(9);
    outtextxy(300, 200, "  Single Player  ");
    outtextxy(300, 220, "  Multiplayer  ");
    outtextxy(300, 240, "  Instructions  ");
    setcolor(5);
    outtextxy(300, 260, "> Quit <");
    }

}

void myMain()
{
int input;
int quit=0;
int goback=0;
    while(quit!=1)
    {
        if(kbhit())
        {
            input=getch();

            if(input== UP)
            {
            if(menuStack>0) menuStack--;
            }
            if(input==DOWN)
            {
            if(menuStack<3) menuStack++;
            }
            if(input==ENTER)
            {

                if(menuStack==0) {printLevel(); goback=checkLevel(); if(goback!=1){break; }  }
                else if(menuStack==1) {playGameMultiplayer(); break; }
                else if(menuStack==2) { instructions(); }
                else if(menuStack==3) {quit=1;  break;}
            }

            printMenu();
        }
    }
    setcolor(14);
    outtextxy(295, 310, "  SEE YOU LATER !  ");
}

//************************************************MENU END *************************//




int main(void)
{
    initwindow(800, 600);
    instructions();
    cleardevice();
    drawBoard(11);
    printMenu();
    myMain();
    getch();

}
