#include <stdio.h>
#include "utilities.h"
#include "io.hpp"
#include "gpio.hpp"

//function prototypes
void boardInput();
int upDown(int pos);
int leftRight(int pos);
void printBoard();
char checkWin();
void clearLED();
void setScore(char winChar);

//variables
GPIO redpin1(P1_29), redpin4(P1_30), redpin7(P1_31), redpin2(P1_22), redpin5(P1_23), redpin8(P1_28), redpin3(P0_29), redpin6(P0_30), redpin9(P1_20);
GPIO bluepin1(P2_4), bluepin4(P2_3), bluepin7(P2_2), bluepin2(P2_7), bluepin5(P2_6), bluepin8(P2_5), bluepin3(P0_26), bluepin6(P0_0), bluepin9(P0_1);
GPIO redpins[] = {redpin1, redpin2, redpin3, redpin4, redpin5, redpin6, redpin7, redpin8, redpin9};
GPIO bluepins[] = {bluepin1, bluepin2, bluepin3, bluepin4, bluepin5, bluepin6, bluepin7, bluepin8, bluepin9};
char board[11] = {'1','2','3','4','5','6','7','8','9'};
char player = 'X';
int pos = 0, score = 0;
bool gameLoop, winner = false;

int main(void)
{
    redpin1.setAsOutput();
    redpin2.setAsOutput();
    redpin3.setAsOutput();
    redpin4.setAsOutput();
    redpin5.setAsOutput();
    redpin6.setAsOutput();
    redpin7.setAsOutput();
    redpin8.setAsOutput();
    redpin9.setAsOutput();
    bluepin1.setAsOutput();
    bluepin2.setAsOutput();
    bluepin3.setAsOutput();
    bluepin4.setAsOutput();
    bluepin5.setAsOutput();
    bluepin6.setAsOutput();
    bluepin7.setAsOutput();
    bluepin8.setAsOutput();
    bluepin9.setAsOutput();
    LD.setNumber(00);
    while(1)
    {
        clearLED();
        if(player == 'X')
        {
            redpin1.setHigh();
        }
        else
        {
            bluepin1.setHigh();
        }
        enum stateType {play, end, restart} currentState;
        currentState = play;
        gameLoop = true;
        while(gameLoop)
        {
            switch(currentState)
            {
                case play:
                {
                    do
                    {
                        //button 1 moves up/down
                        bool sw1 = SW.getSwitch(1);
                        //button 2 moves left/right
                        bool sw2 = SW.getSwitch(2);
                        //button 3 is red placement
                        bool sw3 = SW.getSwitch(3);
                        //button 4 is blue placement
                        bool sw4 = SW.getSwitch(4);
                        if((sw1 || sw2 || sw3 || sw4))
                        {
                            boardInput();
                            printBoard();
                            checkWin();
                            delay_ms(1000);
                        }
                    }while(!winner);
                    currentState = end;
                    break;
                }
                case end:
                {
                    char playerWon = checkWin();
                    printf("The winner is Player %c", playerWon);
                    if(playerWon == 'X')
                    {
                        char winChar = checkWin();
                        setScore(winChar);
                        for(int i=0; i<9; i++)
                        {
                            bluepins[i].setLow();
                            redpins[i].setHigh();
                        }
                    }
                    else if(playerWon == 'O')
                    {
                        char winChar = checkWin();
                        setScore(winChar);
                        for(int i=0; i<9; i++)
                        {
                            redpins[i].setLow();
                            bluepins[i].setHigh();
                        }
                    }
                    else    // no one wins
                    {
                        clearLED();
                        redpins[0].setHigh();
                        redpins[4].setHigh();
                        redpins[8].setHigh();
                        redpins[2].setHigh();
                        redpins[6].setHigh();
                    }
                    //ADD THE BOARD'S TALLY MARKER
                    delay_ms(5000);
                    currentState = restart;
                    break;
                }
                case restart:
                {
                    char playerWon = checkWin();
                    clearLED();
                    for(int i=0; i<10; i++)
                    {
                        board[i] = 49+i;
                    }
                    pos = 0;
                    winner = false;
                    if(playerWon == 'X')
                    {
                        player = 'O';
                    }
                    else
                    {
                        player = 'X';
                    }
                    gameLoop = false;
                    break;
                }
            }
        }
    }
}

void boardInput()
{
    bool sw1 = SW.getSwitch(1);
    bool sw2 = SW.getSwitch(2);
    bool sw3 = SW.getSwitch(3);
    bool sw4 = SW.getSwitch(4);
    if(sw1)
    {
        pos = upDown(pos);
    }
    else if(sw2)
    {
        pos = leftRight(pos);
    }
    else if(sw3 && (player == 'X')) //red player
    {
        if(board[pos] != 'O')
        {
            board[pos] = player;
            printf("Placed a %c at board[%i]\n\n", board[pos],pos);
            redpins[pos].setHigh();
            player = 'O';
        }
    }
    else if(sw4 && (player == 'O')) //blue player
    {
        if(board[pos] != 'X')
        {
            board[pos] = player;
            printf("Placed a %c at board[%i]\n\n", board[pos],pos);
            bluepins[pos].setHigh();
            player = 'X';
        }
    }
}

int upDown(int pos)
{
    bool sw1 = SW.getSwitch(1);
    //Button to move up/down
    if(sw1)
    {
        if(pos != 6 && pos != 7 && pos != 8)
        {
            if((board[pos] != 'O') && (player == 'O'))
            {
                bluepins[pos].setLow();
            }
            if((board[pos] != 'X') && (player == 'X'))
            {
                redpins[pos].setLow();
            }
            pos += 3;
            if(player == 'X')
            {
                redpins[pos].setHigh();
            }
            else
            {
                bluepins[pos].setHigh();
            }
        }
        else
        {
            if((board[pos] != 'O') && (player == 'O'))
            {
                bluepins[pos].setLow();
            }
            if((board[pos] != 'X') && (player == 'X'))
            {
                redpins[pos].setLow();
            }
            pos -= 6;
            if(player == 'X')
            {
                redpins[pos].setHigh();
            }
            else
            {
                bluepins[pos].setHigh();
            }
        }
    }
    return pos;
}

int leftRight(int pos)
{
    bool sw2 = SW.getSwitch(2);
    //Button to move left/right
    if(sw2)
    {
        if(pos != 2 && pos != 5 && pos != 8)
        {
            if((board[pos] != 'O') && (player == 'O'))
            {
                bluepins[pos].setLow();
            }
            if((board[pos] != 'X') && (player == 'X'))
            {
                redpins[pos].setLow();
            }
            pos++;
            if(player == 'X')
            {
                redpins[pos].setHigh();
            }
            else
            {
                bluepins[pos].setHigh();
            }
        }
        else
        {
            if((board[pos] != 'O') && (player == 'O'))
            {
                bluepins[pos].setLow();
            }
            if((board[pos] != 'X') && (player == 'X'))
            {
                redpins[pos].setLow();
            }
            pos -= 2;
            if(player == 'X')
            {
                redpins[pos].setHigh();
            }
            else
            {
                bluepins[pos].setHigh();
            }
        }

    }
    return pos;
}

void printBoard()
{
    for(int j=0; j<3; j++)
    {
        printf("%c  ", board[j]);
    }
    printf("\n");
    for(int k=3; k<6; k++)
    {
        printf("%c  ", board[k]);
    }
    printf("\n");
    for(int l=6; l<9; l++)
    {
        printf("%c  ", board[l]);
    }
    printf("\n\n");

}

char checkWin()
{
    if((board[0] == board[1]) && (board[1] == board[2]))
    {
        winner = true;
        return board[0];
    }
    else if((board[3] == board[4]) && (board[4] == board[5]))
    {
        winner = true;
        return board[3];
    }
    else if((board[6] == board[7]) && (board[7] == board[8]))
    {
        winner = true;
        return board[6];
    }
    else if((board[0] == board[3]) && (board[3] == board[6]))
    {
        winner = true;
        return board[0];
    }
    else if((board[1] == board[4]) && (board[4] == board[7]))
    {
        winner = true;
        return board[1];
    }
    else if((board[2] == board[5]) && (board[5] == board[8]))
    {
        winner = true;
        return board[2];
    }
    else if((board[0] == board[4]) && (board[4] == board[8]))
    {
        winner = true;
        return board[0];
    }
    else if((board[2] == board[4]) && (board[4] == board[6]))
    {
        winner = true;
        return board[2];
    }
    else if(board[0] != '1' && board[1] != '2' && board[2] != '3' && board[3] != '4' && board[4] != '5' && board[5] != '6' && board[6] != '7' && board[7] != '8' && board[8] != '9')
    {
        winner = true;
        return 'L';
    }
    else
    {
        return 0;
    }

}

void clearLED()
{
    for(int i=0; i<9; i++)
    {
        redpins[i].setLow();
        bluepins[i].setLow();
    }
}

void setScore(char winChar){
    if(winChar == 'X'){
        score += 10;
        LD.setNumber(score);
    }
    else if(winChar == 'O'){
        score += 1;
        LD.setNumber(score);
    }
    delay_ms(1000);
    if((score-9)%10 == 0){
        score -= 9;
        LD.setNumber(score);
    }
    if((score/10)/9 == 1){
        score -= 90;
        LD.setNumber(score);
    }
}



