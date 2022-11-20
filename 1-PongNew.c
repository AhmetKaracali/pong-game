/*=============================================================================
 |   Assignment:  CEN110_P01
 |
 |  Student ID :  160101015
 |       Author:  Ahmet KARACALI

 |   Instructor:  Asst.Prof.Dr.A** K*** D*** 
 |     Due Date:  19.03.2017
 |
 +-----------------------------------------------------------------------------
 |
 |  Description:  Writing a Pong Game which has two modes named Multiplayer and
 |      Singleplayer.In singleplayer mode,there will be three difficulty levels.
 |
 |        Input:  Program takes the arrow keys and some special keys
 |                for select the menu option.
 |
 |       Output:  The program prints the game board and ball instantly.
 |
 |    Algorithm:  I used different functions for each menu and each racket.
 |
 | 	 What's New:  I writed new funtions for change the ball's direction when
 |				it hits the racket's corners and middle.
 |
 |
 |   Known Bugs:  No Bug.
 |
 *===========================================================================*/


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#define FALSE   0
#define TRUE    1


#define FOREGROUND_BLUE      0x0001
#define FOREGROUND_GREEN     0x0002
#define FOREGROUND_RED       0x0004
#define FOREGROUND_INTENSITY 0x0008
#define MAX_MENUITEM         4
#define BACKGROUND_BLUE      0x0010
#define BACKGROUND_GREEN     0x0020
#define BACKGROUND_RED       0x0040
#define BACKGROUND_INTENSITY 0x0080

#define KEY_1                224
#define UP                   72
#define DOWN                 80
#define ENTER                13

#define ESC             27
#define LEFT1           'A'
#define LEFT2           'a'
#define RIGHT1          'D'
#define RIGHT2          'd'
#define LEFT3           '4'
#define RIGHT3          '6'
#define MOVE_LEFT       -1
#define MOVE_RIGHT      +1

#define MAX_X           80
#define MAX_Y           25

#define RACKET_X        35
#define RACKET_Y        23
#define RACKET2_X        35
#define RACKET2_Y       1
#define RACKET          219
#define RACKET_LEN      10
#define RACKET_XMOVE    2
#define RACKET_YMOVE    0
#define BALL_X          20
#define BALL_Y          6
#define BALL            'O'
#define BALL_XMOVE       1
#define BALL_YMOVE       1





int racket_x = RACKET_X , racket_y = RACKET_Y;
int racket2_x = RACKET2_X,racket2_y = RACKET2_Y;
int racket_xmove = RACKET_XMOVE, racket_ymove=RACKET_YMOVE;
int ball_x = BALL_X, ball_y = BALL_Y;
int ball_x_prev;
int ball_xmove = BALL_XMOVE, ball_ymove=BALL_YMOVE;
int continuegame = 1;
int pausegame=0;
int rackedCoord[RACKET_LEN]={35,36,37,38,39,40,41,42,43,44,45};
int rackedCoord2[RACKET_LEN]={35,36,37,38,39,40,41,42,43,44,45};
int stack=0;
int qstack=0;
int pstack=0;
int max_pausemenuitems;
int gameWinner=0;
int playerscore=0;
int max_difficultmenu;
int difficultstack=0;
int difficultmenu=0;
int difficultyDELAY=50; // Standard value is 50.


char menuitems[][80] = {{'S','i','n','g','I','e',' ','p','l','a','y','e','r','\0'}, \
                        {'M','u','l','t','i','p','l','a','y','e','r','\0'}, \
                        {'H','e','l','p','\0'}, \
                        {'W','h','a','t','\'','s',' ','N','e','w','?','\0'}, \
                        {'Q','u','i','t','\0'}, \
                        {'\0'}};
char quitmenu[][80] =  {{'Y','e','s','\0'}, \
                        {'N','o','\0'}, \
                        {'\0'}};
char pausemenuitems[][80]=  {{'R','e','s','u','m','e','\0'}, \
                        {'M','a','i','n',' ','M','e','n','u','\0'}, \
                        {'Q','u','i','t','\0'}, \
                        {'\0'}};

char difficultMenu[][80] = {{'E','a','s','y','\0'}, \
                        {'M','e','d','i','u','m','\0'}, \
                        {'H','a','r','d','\0'}, \
                        {'\0'}};


//******************************| MAIN FUNCTION |****************************//
void main()
{
    printMenu(stack);
    mainmenu();
    
    Gotoxy(0,35);
    hidecursor();
}
//******************************| ************* |****************************//



//******************************| PUBLIC FUNCTION |***************************//

/*- Hidecursor -----------------------------------------------------------
 |  Function hidecursor
 |
 |  Purpose:  This function hides the cursor.
 |
 |  Parameters:
 |      
 |              IN: -
 |             OUT: -
 |
 |  Returns:  -
 *--------------------------------------------------------------------------*/
void hidecursor()                                                          
{
   HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_CURSOR_INFO info;
   info.dwSize = 100;
   info.bVisible = FALSE;
   SetConsoleCursorInfo(consoleHandle, &info);
}

/*- textcolor -----------------------------------------------------------
 |  Function textcolor
 |
 |  Purpose:  Changes the color of the text.
 |
 |  Parameters:
 |      
 |              IN: COLOR
 |             OUT: -
 |
 |  Returns:  -
 *--------------------------------------------------------------------------*/
void textcolor(unsigned short color){
    HANDLE hStdout;
    WORD wAttributes;
//    CONSOLE_SCREEN_BUFFER_INFO csbi;

    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

//    GetConsoleScreenBufferInfo(hStdout, &csbi);

    wAttributes = color ;
//   if (color & 0x08) wAttributes |= FOREGROUND_INTENSITY ;

    SetConsoleTextAttribute(hStdout, wAttributes);
}

/*- keyboardinput -----------------------------------------------------------
 |  Function keyboardinput
 |
 |  Purpose:  If user hits a key,this function takes the id of keys.
 |
 |  Parameters:
 |      
 |              IN: adress of key1 & key2
 |             OUT: key1 and key2 values
 |
 |  Returns:  -
 *--------------------------------------------------------------------------*/
void keyboardinput(unsigned int *key1, unsigned int *key2)
{
    unsigned int k1=0, k2=0;

    fflush(stdin);
    while (!kbhit());
    k1 = getch();
    if (k1 == 0 || k1 == 224) {
        k2 = getch();
    }

    *key1 = k1;
    *key2 = k2;
}

/*- mainmenu -----------------------------------------------------------
 |  Function mainmenu
 |
 |  Purpose:  This function controls main menu of the game.
 |
 |  Parameters:
 |      
 |              IN: -
 |             OUT: -
 |
 |  Returns:  -
 *--------------------------------------------------------------------------*/
int mainmenu()
{
    unsigned int key1, key2;
    int n;
    int i,j;


    while (1)
    {

    keyboardinput(&key1, &key2);

    if(key1==KEY_1 && key2==DOWN)
    {
        if(stack==MAX_MENUITEM){}
        else stack+=1;
    }
    if(key1==KEY_1 && key2== UP)
    {
        if(stack==0){}
        else stack-=1;
    }
    else if (key1 == ESC) {
            system("CLS");
            Beep(300, 600);
            printf("Goodbye\n");
            break;
        }
    else if(key1== ENTER)
    {
        system("CLS");

        if(stack==0)
        {
                difficultmenu=1;
                defineDifficultMenu();
                printDifficult(difficultstack);
                difficultMain();
            break;
        }
        else if(stack==1)
        {
            startGame();
            playGame();
            break;

        }
        else if(stack==2)
        {
            printHelp();
            system("PAUSE");
            system("CLS");
        }
        else if(stack==3)
        {
            printNew();
            system("PAUSE");
            system("CLS");
        }
        else if(stack==4)
        {
            printQuitMenu();
            quitMain();
            break;
        }
        else
        {
            for(i=0;menuitems[stack][i];i++)
            {
                printf("%c",menuitems[stack][i]);

            }
        printf("\n");
        break;
        }
    }
	else
    {
        if(key2!=DOWN) Beep(300, 600);
    }
    printMenu(stack);
    }

}

/*- quitmain -----------------------------------------------------------
 |  Function quitmain
 |
 |  Purpose:  This function controls the quit menu.
 |
 |  Parameters:
 |      
 |              IN: -
 |             OUT: -
 |
 |  Returns:  -
 *--------------------------------------------------------------------------*/
void quitMain()
{
        unsigned int key1, key2;
    int n;
    int i,j;


    while (1)
    {

    keyboardinput(&key1, &key2);

    if(key1==KEY_1 && key2==DOWN)
    {
        if(qstack==1){}
        else qstack+=1;
    }
    else if(key1==KEY_1 && key2== UP)
    {
        if(stack==0){}
        else qstack-=1;
    }
    else if (key1 == ESC) {
            Beep(300, 600);
            printf("Goodbye\n");
            break;
        }
    else if(key1== ENTER)
    {
        system("CLS");

        if(qstack==0)
        {
            printf("Goodbye....\n");
            break;

        }
        else if(qstack==1)
        {
            system("CLS");
            printMenu(stack);
            mainmenu();
            break;
        }

    }
	else
    {
        Beep(300, 600);
    }
    printQuitMenu(qstack);
    }
}

/*- printMenu -----------------------------------------------------------
 |  Function printMenu
 |
 |  Purpose:  This function prints the main menu
 |
 |  Parameters:
 |      
 |              IN: stack
 |             OUT: -
 |
 |  Returns:  -
 *--------------------------------------------------------------------------*/
void printMenu(int stack)
{
    unsigned int key1, key2;
    int i,j,yCoord=1;

        for(i=0;i<menuitems[i][0];i++)
        {
              Gotoxy(5,yCoord);

        for(j=0;menuitems[i][j]!='\0';j++)
            {
                if(stack==i)
                {
                    textcolor(BACKGROUND_INTENSITY | FOREGROUND_RED);
                    printf("%c",menuitems[i][j]);
                }
                else{
                    textcolor(BACKGROUND_GREEN | FOREGROUND_BLUE);
                    printf("%c",menuitems[i][j]);
                }
            }
            yCoord+=2;
        }
    hidecursor();
}

/*- printQuitMenu -----------------------------------------------------------
 |  Function printQuitMenu
 |
 |  Purpose:  This function prints the quit menu.
 |
 |  Parameters:
 |      
 |              IN:-
 |             OUT:-
 |
 |  Returns:  
 *--------------------------------------------------------------------------*/
void printQuitMenu()
{
    system("CLS");
    system("color 4B");
    unsigned int key1, key2;
    int i,j,yCoord=1;

        for(i=0;i<quitmenu[i][0];i++)
        {
              Gotoxy(5,yCoord);

        for(j=0;quitmenu[i][j]!='\0';j++)
            {
                if(qstack==i)
                {
                    textcolor(BACKGROUND_INTENSITY | FOREGROUND_RED);
                    printf("%c",quitmenu[i][j]);
                }
                else{
                    textcolor(BACKGROUND_GREEN | FOREGROUND_BLUE);
                    printf("%c",quitmenu[i][j]);
                }
            }
            yCoord+=2;
        }
    hidecursor();
}

/*- Gotoxy -----------------------------------------------------------
 |  Function Gotoxy
 |
 |  Purpose:  This function puts the cursor whereever user wants.
 |
 |  Parameters:
 |      
 |              IN: x and y coordinates
 |             OUT: -
 |
 |  Returns:  -
 *--------------------------------------------------------------------------*/
void Gotoxy(int column,int row)
{
    COORD c;

    c.X = column;
    c.Y = row;

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),c);
}

/*- Gotoxy2 -----------------------------------------------------------
 |  Function Gotoxy2
 |
 |  Purpose:  This function puts the cursor whereever user wants.
 |
 |  Parameters:
 |      
 |              IN: x and y coordinates
 |             OUT: -
 |
 |  Returns:  -
 *--------------------------------------------------------------------------*/
void Gotoxy2(int column,int row)
{
    COORD c;

    c.X = column;
    c.Y = row;

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),c);
}

/*- createGameWindow -----------------------------------------------------------
 |  Function createGameWindow
 |
 |  Purpose:  This function creates the game window.
 |
 |  Parameters:
 |      
 |              IN: Global variables.
 |             OUT: Game Board
 |
 |  Returns:  -
 *--------------------------------------------------------------------------*/
void createGameWindow()
{
    int i;

    Gotoxy(0,0);
    for (i=0; i<MAX_X; i++) {
        printf("#");
    }

    for (i=1; i<MAX_Y-1; i++) {
        Gotoxy(0,i);
        printf("#");
        Gotoxy(MAX_X-1,i);
        printf("#");
    }

    Gotoxy(0,MAX_Y-1);
    for (i=0; i<MAX_X; i++) {
        printf("#");
    }
}

/*- drawRacket -----------------------------------------------------------
 |  Function drawRacket
 |
 |  Purpose:  This function draws the racket
 |
 |  Parameters:
 |      
 |              IN: x,y and c
 |             OUT: racket
 |
 |  Returns:  -
 *--------------------------------------------------------------------------*/
void drawRacket(int x,int y,int c)
{
    int i;

    if(x>0 && x<79){
    Gotoxy(x,y);
    for (i=0; i<RACKET_LEN; i++) {
        printf("%c",c);
    }
    }
}

/*- drawRacket2 -----------------------------------------------------------
 |  Function drawRacket2
 |
 |  Purpose:  This function draws the racket
 |
 |  Parameters:
 |      
 |              IN: x,y and c
 |             OUT: racket
 |
 |  Returns:  -
 *--------------------------------------------------------------------------*/
void drawRacket2(int x,int y,int c)
{
    int i;

    if(x>0 && x<79){
    Gotoxy(x,y);
    for (i=0; i<RACKET_LEN; i++) {
        printf("%c",c);
    }
    }
}

/*- drawBall -----------------------------------------------------------
 |  Function drawBall
 |
 |  Purpose:  This function draws the ball
 |
 |  Parameters:
 |      
 |              IN: x,y and c
 |             OUT: ball
 |
 |  Returns:  -
 *--------------------------------------------------------------------------*/
void drawBall(int x, int y, int c)
{
    if(continuegame!=0)
    {
    Gotoxy(x,y);
    printf("%c",c);
    }
    
    if (pausegame!=1 && continuegame==1)
    {
        Gotoxy(x,y);
        printf("%c",c);
    }
}


/*- startGame -----------------------------------------------------------
 |  Function startGame
 |
 |  Purpose:  This function starts the game with calling some functions.
 |
 |  Parameters:
 |      
 |              IN: -
 |             OUT: -
 |
 |  Returns:  -
 *--------------------------------------------------------------------------*/
void startGame()
{
    system("CLS");
    createGameWindow();
    drawRacket(racket_x, racket_y, RACKET);
    drawRacket2(racket2_x, racket2_y, RACKET);
    drawBall(ball_x, ball_y, BALL);
    hidecursor();
}

/*- moveRacket -----------------------------------------------------------
 |  Function moveRacket
 |
 |  Purpose:  This function moves the racket.
 |
 |  Parameters:
 |      
 |              IN: dir
 |             OUT: racket
 |
 |  Returns:  -
 *--------------------------------------------------------------------------*/
void moveRacket(int dir)
{
    int i;
    drawRacket(racket_x, racket_y, ' ');
    racket_x = racket_x + (dir * racket_xmove);
    drawRacket(racket_x, racket_y, RACKET);

    for(i=0;i<RACKET_LEN;i++)
    {
        rackedCoord[i]=racket_x+i;
    }
}

/*- moveRacket2 -----------------------------------------------------------
 |  Function moveRacket2
 |
 |  Purpose:  This function moves the racket2.
 |
 |  Parameters:
 |      
 |              IN: dir
 |             OUT: racket2
 |
 |  Returns:  -
 *--------------------------------------------------------------------------*/
void moveRacket2(int dir)
{
    int i;
    drawRacket2(racket2_x, racket2_y, ' ');
    racket2_x = racket2_x + (dir * racket_xmove);
    drawRacket2(racket2_x, racket2_y, RACKET);

    for(i=0;i<RACKET_LEN;i++)
    {
        rackedCoord2[i]=racket2_x+i;
    }
}

/*- moveBall -----------------------------------------------------------
 |  Function moveBall
 |
 |  Purpose:  This function moves the ball.
 |
 |  Parameters:
 |      
 |              IN: -
 |             OUT: -
 |
 |  Returns:  -
 *--------------------------------------------------------------------------*/
void moveBall()
{
    int condition;
    ball_x_prev=ball_x;
    drawBall(ball_x, ball_y, ' ');

   // Bounch off left/right walls
    if ( ball_x > 77 || ball_x < 2) {
        ball_xmove *= -1.0;
    }

    if ( ball_y == 22)
    {
        condition=checkReturn();
            if(condition==1)
    {
       
        ball_ymove *= -1.0;
        playerscore+=5;
    }
        if(condition==0)
        {
            continuegame=0;
            gameWinner=2;
        }
    }
    // Bounch off top/bottom walls
    if ( ball_y == 2)
    {
        condition=checkReturn2();
            if(condition==1)
    {
        ball_ymove *= -1.0;
    }
        if(condition==0)
        {
            continuegame=0;
            gameWinner=1;
        }
    }




    ball_x += ball_xmove;
    ball_y += ball_ymove;
   
    drawBall(ball_x, ball_y, BALL);
}

/*- checkReturn -----------------------------------------------------------
 |  Function checkReturn
 |
 |  Purpose:  This function checks for return the ball.
 |
 |              If racket1 is on the right coordinates.Changes the direction
 |            of the ball.
 |
 |  Parameters:
 |      
 |              IN: -
 |             OUT: -
 |
 |  Returns:  -
 *--------------------------------------------------------------------------*/
int checkReturn()
{
    int i,con=1,hit;
    int middle,corner1,corner2;
    
    middle  = RACKET_LEN/2;
    corner1 = rackedCoord[0];
    corner2 = rackedCoord[RACKET_LEN-1];
    
    
    for(i=0;i<RACKET_LEN;i++)
    {
        if( ball_x == rackedCoord[i])
        {
            if(ball_x== rackedCoord[middle]) ball_xmove *= 0.0;
             if(ball_x== rackedCoord[corner1]) ball_xmove *= 2.0;
            if(ball_x== rackedCoord[corner2]) ball_xmove *= -2.0;
            con=0;
        }
    }

    if(con==0)  return 1;
    if(con==1)  return 0;
}

/*- checkReturn2 -----------------------------------------------------------
 |  Function checkReturn2
 |
 |  Purpose:  This function checks for return the ball.
 |
 |              If racket2 is on the right coordinates.Changes the direction
 |            of the ball.
 |
 |  Parameters:
 |      
 |              IN: -
 |             OUT: -
 |
 |  Returns:  -
 *--------------------------------------------------------------------------*/
int checkReturn2()
{
    int i,con=1,hit;
    int middle,corner1,corner2;
    
    middle=RACKET_LEN/2;
    corner1=rackedCoord2[0];
    corner2=rackedCoord2[RACKET_LEN-1];

    for(i=0;i<RACKET_LEN;i++)
    {
        if( ball_x == rackedCoord2[i]) 
        {
            if(ball_xmove==0)    ball_xmove=1;
             if(ball_x== rackedCoord2[middle]) ball_xmove *= 0.0;
             if(ball_x== rackedCoord2[corner1]) ball_xmove *= 2.0;
            if(ball_x== rackedCoord2[corner2]) ball_xmove *= -2.0;
            con=0;
        }
    }

    if(con==0)  return 1;
    if(con==1)  return 0;
}

/*- delay -----------------------------------------------------------
 |  Function delay
 |
 |  Purpose:  This function makes delay.
 |
 |  Parameters:
 |      
 |              IN: delay value as mseconds
 |             OUT: -
 |
 |  Returns:  -
 *--------------------------------------------------------------------------*/
void delay(unsigned int mseconds)
{
    clock_t goal = mseconds + clock();
    while (goal > clock());
}
//******************************| ************** |***************************//


//****************************** |FOR MULTIPLAYER|***************************//
/*- manageGameMultiplayer -----------------------------------------------------------
 |  Function manageGameMultiplayer
 |
 |  Purpose:  This function controls the multiplayer game.
 |
 |
 |  Parameters:
 |      
 |              IN: -
 |             OUT: -
 |
 |  Returns:  -
 *--------------------------------------------------------------------------*/
int manageGameMultiplayer()
{
    int keyPressed;

    delay (difficultyDELAY);

    if (kbhit()) {
        keyPressed=getch();
        if (keyPressed == ESC) {
            pausegame=1;
        }
        else if (keyPressed == LEFT1 || keyPressed == LEFT2) {
            if(racket_x>=3) moveRacket(MOVE_LEFT);
        }
        else if (keyPressed == RIGHT1 || keyPressed == RIGHT2) {
            if(racket_x<=67) moveRacket(MOVE_RIGHT);
        }
        else if (keyPressed == RIGHT3) {
            if(racket2_x<=67) moveRacket2(MOVE_RIGHT);
        }
        else if (keyPressed == LEFT3) {
            if(racket2_x>=3) moveRacket2(MOVE_LEFT);
        }
        else if (continuegame!=1) return 1;

    }
    moveBall();
    return FALSE;
}

/*- playGame -----------------------------------------------------------
 |  Function playGame
 |
 |  Purpose:  This function helps the main multiplayer function for manage 
 |            the game.
 |
 |
 |  Parameters:
 |      
 |              IN: -
 |             OUT: -
 |
 |  Returns:  -
 *--------------------------------------------------------------------------*/
void playGame()
{

    while (continuegame==1) 
    {
        system("color 4B");
        manageGameMultiplayer();
    
            if(pausegame!=1)
        {
        system("color 4B");
         manageGameMultiplayer();
        }
            else if(pausegame==1)
        {
             pausingMulti();
             break;
        }
        
    }
    endGameMultiplayer();
}

/*- endGameMultiplayer -----------------------------------------------------------
 |  Function endGameMultiplayer
 |
 |  Purpose:  This function prints the result of the game for multiplayer mode. 
 |
 |
 |
 |  Parameters:
 |      
 |              IN: -
 |             OUT: -
 |
 |  Returns:  -
 *--------------------------------------------------------------------------*/
void endGameMultiplayer()
{
    Gotoxy(0,30);
    if(gameWinner==1)
    {
        printf("Player 1 Wins!!\n");
        printf("Thanks For Playing.\n");
    }
    else if(gameWinner==2)
    {
        printf("Player 2 Wins!!\n");
        printf("Thanks For Playing.\n");
    }
   
}

//****************************** |***************|***************************//

//******************** |MULTIPLAYER PAUSE MENU FUNCTIONS|******************//
/*- pausingMulti -----------------------------------------------------------
 |  Function pausingMulti
 |
 |  Purpose:  This function manages the pausing functions of multiplayer mode. 
 |        
 |  Parameters:
 |      
 |              IN: -
 |             OUT: -
 |
 |  Returns:  -
 *--------------------------------------------------------------------------*/
int pausingMulti()
{
    system("CLS");
    definePauseMenuItems();
    printPauseMenu(pstack);
    mainPausemenuMulti();
}

/*- mainPausemanuMulti ---------------------------------------------------------
 |  Function mainPausemenuMulti
 |
 |  Purpose:  This function controls the pause menu of the multiplayer mode. 
 |
 |
 |  Parameters:
 |      
 |              IN: -
 |             OUT: -
 |
 |  Returns:  -
 *--------------------------------------------------------------------------*/
int mainPausemenuMulti()
{    
    unsigned int key1, key2;
    int n;
    int i,j;
    pstack=0;
    while (pausegame==1)
    {

    keyboardinput(&key1, &key2);

    if(key1==KEY_1 && key2==DOWN)
    {
        if(pstack==2){}
        else pstack+=1;
    }
    else if(key1==KEY_1 && key2== UP)
    {
        if(pstack==0){}
        else pstack-=1;
    }
    else if (key1 == ESC) {
            Beep(300, 600);
            printf("Goodbye\n");
            continuegame=0;
            break;
        }
    else if(key1== ENTER)
    {

        if(pstack==0)
        {
            system("CLS");
            pausegame=0;
            continuegame=1;
            startGame();
            playGame();
            
        }
        else if(pstack==1)
        {
            system("CLS");
            printMenu(stack);
            mainmenu();
            pausegame=0;
        }
        
        else if(pstack==2)
        {
            Gotoxy(0,0);
            pausegame=0;
            continuegame=0;
            break;
        }

    }
	else
    {
        Beep(300, 600);
    }
    printPauseMenu();
    }
    system("CLS");
}
//******************** |**********************************|******************//



//************************|FOR SINGLEPLAYER|**********************************//
/*- pcMove ---------------------------------------------------------
 |  Function pcMove
 |
 |  Purpose:  This function controls the pc moves in singleplayer mode.
 |
 |
 |  Parameters:
 |      
 |              IN: -
 |             OUT: -
 |
 |  Returns:  -
 *--------------------------------------------------------------------------*/
int pcMove()
{
    int right=0,left=0,condition,i;
    
    condition=whereBallGoes();
    delay(difficultyDELAY);
    
    for(i=0;i<9;i++)
    {
    if(condition>rackedCoord2[i]) { right=1; left=0; }
    if (condition<rackedCoord2[i]) { right=0; left=1; }
    if (condition==rackedCoord2[i]) { right=0; left=0; }
    }
    
    if(right==1)
    {
        if(racket2_x<=67) moveRacket2(MOVE_RIGHT);
            right=0;
            left=0;
    }
    if(left==1)
    {
        if(racket2_x>=3) moveRacket2(MOVE_LEFT);
            right=0;
            left=0;
    }
    

}

/*- whereBallGoes ---------------------------------------------------------
 |  Function whereBallGoes
 |
 |  Purpose:  This function calculates the coordinate of the hitting point 
 |            of the ball.
 |
 |
 |  Parameters:
 |      
 |              IN: -
 |             OUT: -
 |
 |  Returns:  -
 *--------------------------------------------------------------------------*/
int whereBallGoes()
{
    int i,result,xCoordinate;
    
    result = ball_y - 1;
    
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

/*- manageGameSinglePlayer ---------------------------------------------------------
 |  Function manageGameSinglePlayer
 |
 |  Purpose:  This function manages the singleplayer game.
 |
 |
 |  Parameters:
 |      
 |              IN: -
 |             OUT: -
 |
 |  Returns:  -
 *--------------------------------------------------------------------------*/
int manageGameSinglePlayer()
{
    int keyPressed;

    delay (difficultyDELAY);

    if (kbhit()) {
        keyPressed=getch();
        if (keyPressed == ESC) {
            pausegame=1;
        }
        else if (keyPressed == LEFT1 || keyPressed == LEFT2) {
            if(racket_x>=3) moveRacket(MOVE_LEFT);
        }
        else if (keyPressed == RIGHT1 || keyPressed == RIGHT2) {
            if(racket_x<=67) moveRacket(MOVE_RIGHT);
        }
        
        }
        else if (continuegame!=1) return 1;
    
    if(ball_ymove==-1)  pcMove();
        
    moveBall();
    return FALSE;
}

/*- playGameSinglePlayer ---------------------------------------------------------
 |  Function playGameSinglePlayer
 |
 |  Purpose:  This function helps the manage function for manage to 
 |            singleplayer game.
 |
 |
 |  Parameters:
 |      
 |              IN: -
 |             OUT: -
 |
 |  Returns:  -
 *--------------------------------------------------------------------------*/
void playGameSinglePlayer()
{

    while (continuegame==1) {
        
        if(pausegame!=1)
        {
        system("color 4B");
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
/*- endGameSinglePlayer ---------------------------------------------------------
 |  Function endGameSinglePlayer
 |
 |  Purpose:  This function prints the end screen of singleplayer game.
 |
 |
 |  Parameters:
 |      
 |              IN: -
 |             OUT: -
 |
 |  Returns:  -
 *--------------------------------------------------------------------------*/
void endGameSingleplayer()
{
    Gotoxy(0,30);
    if(gameWinner==1)
    {
        printf("Player 1 Wins!!\n");
        printf("Thanks For Playing.\n");
    }
    else if(gameWinner==2)
    {
        printf("Computer Wins!!\n");
        printf("Your Score is:%d \n",playerscore);
        printf("Thanks For Playing.\n");
    }
   
}

//******************** |SINGLE PLAYER PAUSE MENU FUNCTIONS|******************//
/*- pausing ---------------------------------------------------------
 |  Function pausing
 |
 |  Purpose:  If user hits ESC when playing game. This function calls the other
 |              pausing functions.
 |  Parameters:
 |      
 |              IN: -
 |             OUT: -
 |
 |  Returns:  -
 *--------------------------------------------------------------------------*/
int pausing()
{
    system("CLS");
    definePauseMenuItems();
    printPauseMenu(pstack);
    mainPausemenu();
}

/*- definePauseMenuItems ---------------------------------------------------------
 |  Function definePauseMenuItems
 |
 |  Purpose:  This function checks for how many options pause menu have.
 | 
 |
 |  Parameters:
 |              IN: -
 |             OUT: -
 |
 |  Returns:  -
 *--------------------------------------------------------------------------*/
void definePauseMenuItems()
{
    max_pausemenuitems = countPauseMenuItems(pausemenuitems);
}

/*- countPauseMenuItems ---------------------------------------------------------
 |  Function countsPauseMenuItems
 |
 |  Purpose:  This function counts the items of the pause menu.
 |
 |  Parameters:
 |      
 |              IN: -
 |             OUT: -
 |
 |  Returns:  -
 *--------------------------------------------------------------------------*/
int countPauseMenuItems(char whichmenu[][80])
{
    int i,j,counter=0;
    
    for(i=0;whichmenu[i][0];i++)
    {
        if(whichmenu[i][0]) counter++;
        else if(!whichmenu[i][j]) break;
    }
    
    return counter-1;
    
}

/*- mainPauseMenu ---------------------------------------------------------
 |  Function mainPauseMenu
 |
 |  Purpose:  This function controls the pause menu.
 |
 |  Parameters:
 |      
 |              IN: -
 |             OUT: -
 |
 |  Returns:  -
 *--------------------------------------------------------------------------*/
int mainPausemenu()
{    
    unsigned int key1, key2;
    int n;
    int i,j;
    pstack=0;
    while (pausegame==1)
    {

    keyboardinput(&key1, &key2);

    if(key1==KEY_1 && key2==DOWN)
    {
        if(pstack==2){}
        else pstack+=1;
    }
    else if(key1==KEY_1 && key2== UP)
    {
        if(pstack==0){}
        else pstack-=1;
    }
    else if (key1 == ESC) {
            Beep(300, 600);
            printf("Goodbye\n");
            continuegame=0;
            break;
        }
    else if(key1== ENTER)
    {

        if(pstack==0)
        {
            system("CLS");
            pausegame=0;
            continuegame=1;
            startGame();
            playGameSinglePlayer();
            
        }
        else if(pstack==1)
        {
            system("CLS");
            printMenu(stack);
            mainmenu();
            pausegame=0;
        }
        
        else if(pstack==2)
        {
            Gotoxy(0,0);
            pausegame=0;
            continuegame=0;
            break;
        }
        pausegame==0;
    }
	else
    {
        Beep(300, 600);
    }
    printPauseMenu();
    }
    system("CLS");
}
/*- printPauseMenu ---------------------------------------------------------
 |  Function printPauseMenu
 |
 |  Purpose:  This function prints the pause menu to the screen.
 |
 |  Parameters:
 |      
 |              IN: -
 |             OUT: -
 |
 |  Returns:  -
 *--------------------------------------------------------------------------*/
void printPauseMenu()
{
    unsigned int key1, key2;
    int i,j,yCoord=1;



        for(i=0;i<pausemenuitems[i][0];i++)
        {
              Gotoxy(5,yCoord);

        for(j=0;pausemenuitems[i][j]!='\0';j++)
            {
                if(pstack==i)
                {
                    textcolor(BACKGROUND_INTENSITY | FOREGROUND_RED);
                    printf("%c",pausemenuitems[i][j]);
                }
                else{
                    textcolor(BACKGROUND_GREEN | FOREGROUND_BLUE);
                    printf("%c",pausemenuitems[i][j]);
                }
            }
            yCoord+=2;
        }
    hidecursor();
}

//****************************** |****************|***************************//

//****************************** |DIFFICULTY MENU |***************************//
/*- defineDifficultMenu ---------------------------------------------------------
 |  Function defineDifficultMenu
 |
 |  Purpose:  defines the number of items of difficult menu.
 |  Parameters:
 |      
 |              IN: -
 |             OUT: -
 |
 |  Returns:  -
 *--------------------------------------------------------------------------*/
void defineDifficultMenu()
{
    max_difficultmenu       = countDifficultItems(difficultMenu);
}

/*- countDifficultItems ---------------------------------------------------------
 |  Function countDifficultItems
 |
 |  Purpose:  counts the items of the difficult menu.
 |
 |  Parameters:
 |      
 |              IN: -
 |             OUT: -
 |
 |  Returns:  -
 *--------------------------------------------------------------------------*/
int countDifficultItems(char whichmenu[][80])
{
    int i,j,counter=0;
    
    for(i=0;whichmenu[i][0];i++)
    {
        if(whichmenu[i][0]) counter++;
        else if(!whichmenu[i][j]) break;
    }
    
    return counter-1;
    
}

/*- difficultMain ---------------------------------------------------------
 |  Function difficultMain
 |
 |  Purpose:  Manages the difficult menu.
 |
 |  Parameters:
 |      
 |              IN: -
 |             OUT: -
 |
 |  Returns:  -
 *--------------------------------------------------------------------------*/
int difficultMain()
{
    unsigned int key1, key2;
    int n;
    int i,j;
     while (difficultmenu==1)
    {
    keyboardinput(&key1, &key2);

    if(key1==KEY_1 && key2==DOWN)
    {
        if(difficultstack==max_difficultmenu){}
        else difficultstack+=1;
    }
    else if(key1==KEY_1 && key2== UP)
    {
        if(difficultstack==0){}
        else difficultstack-=1;
    }
    else if (key1 == ESC) 
    {
            system("CLS");
            Beep(300, 600);
            printf("Goodbye\n");
            break;
   }
    else if(key1== ENTER)
    {
       
        if(difficultstack==0)
        {
            difficultyDELAY=120;
            startGame();
            playGameSinglePlayer();
            manageGameSinglePlayer();
        }
        
        if(difficultstack==1)
        {
            difficultyDELAY=50;
            startGame();
            playGameSinglePlayer();
            manageGameSinglePlayer();
        }
        if(difficultstack==2)       
        {
            difficultyDELAY=20;
            startGame();
            playGameSinglePlayer();
            manageGameSinglePlayer();

        }
        difficultmenu=0;
    }
	else
    {
        Beep(300, 200);
    }
    printDifficult(difficultstack);
    }

}

/*- printDifficult ---------------------------------------------------------
 |  Function printDifficult
 |
 |  Purpose:  prints the difficult menu
 |
 |  Parameters:
 |      
 |              IN: -
 |             OUT: -
 |
 |  Returns:  -
 *--------------------------------------------------------------------------*/
void printDifficult(int difficultstack)
{
    unsigned int key1, key2;
    int i,j,yCoord=1;


system("color 4B");
        for(i=0;i<difficultMenu[i][0];i++)
        {
              Gotoxy(5,yCoord);

        for(j=0;difficultMenu[i][j]!='\0';j++)
            {
                if(difficultstack==i)
                {
                    textcolor(BACKGROUND_INTENSITY | FOREGROUND_RED);
                    printf("%c",difficultMenu[i][j]);
                }
                else
                {
                    textcolor(BACKGROUND_GREEN | FOREGROUND_BLUE);
                    printf("%c",difficultMenu[i][j]);
                }
            }
            yCoord+=2;
        }
hidecursor();
}
//****************************** |****************|***************************//

//****************************** |HELP SCREEN |***************************//
/*- printHelp ---------------------------------------------------------
 |  Function printHelp
 |
 |  Purpose:  counts the items of the difficult menu.
 |
 |  Parameters:
 |      
 |              IN: -
 |             OUT: -
 |
 |  Returns:  -
 *--------------------------------------------------------------------------*/
void printHelp()
{
    system("color 4B");
    Gotoxy(0,0);
    printf("|=========================|HELP|==========================|\n");
    printf("| SINGLEPLAYER MODE:                                      |\n");
    printf("|                                                         |\n");
    printf("|    1-Player Rules the bottom racket.                    |\n");
    printf("|    2-Player earns 5 points with every succesful return. |\n");
    printf("|                                                         |\n");
    printf("|  MULTIPLAYER MODE:                                      |\n");
    printf("|                                                         |\n");
    printf("|   1-Player1 Rules the bottom racket.                    |\n");
    printf("|   2-Player2 Rules the upper racket.                     |\n"); 
    printf("|   3-Game continues till a person loses.                 |\n");
    printf("|                                                         |\n");
    printf("|  KEY INSTRUCTIONS:                                      |\n");
    printf("|                                                         |\n");
    printf("|    Bottom Racket: A-Left D-Right                        |\n");
    printf("|    Upper Racket: 4-Left 6-Right                         |\n");
    printf("|    Press ESC for Exit.                                  |\n");
    printf("|    If you hit an invalid key.Program makes a beep sound.|\n");
    printf("|                                                         |\n");
    printf("|=========================================================|\n");
    
    Gotoxy(0,24);
    hidecursor();

}

void printNew()
{
    system("color 4B");
    Gotoxy(0,0);
    printf("|=====================|What's New?|=======================|\n");
    printf("|                                                         |\n");
    printf("|                                                         |\n");
    printf("|    1- If The Ball goes to one of the racket's corners,  |\n");
    printf("|    the ball gonna have a new direction to go now.       |\n");
    printf("|                                                         |\n");
    printf("|                                                         |\n");
    printf("|                                                         |\n");
    printf("|    2-If The Ball goes to middle of the racket,          |\n");
    printf("|    It will be lose its move on X direction till         |\n"); 
    printf("|    the other racket catches the ball.                   |\n");
    printf("|                                                         |\n");
    printf("|               Have a good time :)                       |\n");
    printf("|                                                         |\n");
    printf("|=========================================================|\n");
    
    Gotoxy(0,24);
    hidecursor();

}


//****************************** |****************|***************************//