#include <genesis.h>
#include "resources.h"
#include <string.h>

//BOUNDS TO MEGADRIVE RES
const int LEFT_EDGE = 0;
const int RIGHT_EDGE = 320;
const int TOP_EDGE = 0;
const int BOTTOM_EDGE = 224;

typedef struct
{
    Sprite *ball;
    int x;
    int y;
    int width;
    int height;
    int velX;
    int velY;
} Ball;

typedef struct
{
    Sprite *paddle;
    int x;
    int y;
    int velX;
    int width;
    int height;
} Player;

//score system
int score = 0;
char label_score[6] = "SCORE\0";
char str_score[3] = "0";

//game system
game_on = FALSE;
char msg_start[22] = "PRESS START TO BEGIN!\0";
char msg_reset[37] = "GAME OVER! PRESS START TO PLAY AGAIN.";

Ball mainBall;
Player mainPlayer;

void myJoyHandler(u16 joy, u16 changed, u16 state)
{
    if (joy == JOY_1)
    {
        if (state & BUTTON_RIGHT)
        {
            addVelocity(&mainPlayer, 3);
        }
        else if (state & BUTTON_LEFT)
        {
            addVelocity(&mainPlayer, -3);
        }
        else
        {
            if ((changed & BUTTON_RIGHT) | (changed & BUTTON_LEFT))
            {
                addVelocity(&mainPlayer, 0);
            }
        }
        if (state & BUTTON_START)
        {
            if (!game_on)
            {
                startGame();
            }
        }
    }
}

int sign(int x)
{
    return (x > 0) - (x < 0);
}

int main() //The main function and entry point
{

    JOY_init();
    JOY_setEventHandler(&myJoyHandler);
    VDP_setTextPlan(PLAN_A);
    VDP_drawText("Hyuan's Incredible Game", 8, 1);
    VDP_drawText(label_score, 32, 2);
    updateScoreDisplay();
    showText(msg_start);
    VDP_loadTileSet(bgtile.tileset, 1, DMA);
    VDP_setPalette(PAL1, bgtile.palette->data);
    SPR_init(0, 0, 0);
    mainBall.ball = SPR_addSprite(&imgball, 10, 10, TILE_ATTR(PAL1, 0, FALSE, FALSE));
    mainPlayer.paddle = SPR_addSprite(&paddle, mainPlayer.x, mainPlayer.y, TILE_ATTR(PAL1, 0, FALSE, FALSE));
    VDP_fillTileMapRect(PLAN_B, TILE_ATTR_FULL(PAL1, 0, FALSE, FALSE, 1), 0, 0, 40, 30);

    while (1) //The main game loop
    {
        if(game_on){
            checkCollision(&mainPlayer, &mainBall);
            moveBall(&mainBall);
            movePlayer(&mainPlayer);
        }
        SPR_update();
        VDP_waitVSync(); //wait for screen refresh
    }
    return (0); //End the program
}

void moveBall(Ball *mainBall)
{

    if (mainBall->x < LEFT_EDGE)
    {
        mainBall->x = LEFT_EDGE;
        mainBall->velX = -mainBall->velX;
    }
    else if (mainBall->x + mainBall->width > RIGHT_EDGE)
    {
        mainBall->x = RIGHT_EDGE - mainBall->width;
        mainBall->velX = -mainBall->velX;
    }
    if (mainBall->y < TOP_EDGE)
    {
        mainBall->y = TOP_EDGE;
        mainBall->velY = -mainBall->velY;
    }
    else if (mainBall->y + mainBall->height > BOTTOM_EDGE)
    {
        endGame();
    }

    mainBall->x += mainBall->velX;
    mainBall->y += mainBall->velY;
    SPR_setPosition(mainBall->ball, mainBall->x, mainBall->y);
}

void initBall(Ball *someBall)
{
    someBall->x = 100;
    someBall->y = 100;
    someBall->width = 8;
    someBall->height = 8;
    someBall->velX = 1;
    someBall->velY = 1;
}

void initPlayer(Player *somePlayer)
{
    somePlayer->x = 144;
    somePlayer->y = 200;
    somePlayer->velX = 0;
    somePlayer->width = 32;
    somePlayer->height = 8;
}

void addVelocity(Player *somePlayer, int velocity)
{
    somePlayer->velX = velocity;
}

void movePlayer(Player *somePlayer)
{
    somePlayer->x += somePlayer->velX;
    if (somePlayer->x < LEFT_EDGE)
        somePlayer->x = LEFT_EDGE;
    if (somePlayer->x > RIGHT_EDGE)
        somePlayer->x = RIGHT_EDGE - somePlayer->width;
    SPR_setPosition(somePlayer->paddle, somePlayer->x, somePlayer->y);
}

void checkCollision(Player *somePlayer, Ball *someBall)
{
    if (someBall->x < somePlayer->x + somePlayer->width && someBall->x + someBall->width > somePlayer->x)
    {
        if (someBall->y < somePlayer->y + somePlayer->height && someBall->y + someBall->height >= somePlayer->y)
        {
            someBall->y -= someBall->height - 1;
            someBall->velY = -someBall->velY;

            score++;
            updateScoreDisplay();

            if (score % 10 == 0)
            {
                someBall->velY += sign(someBall->velY);
                someBall->velX += sign(someBall->velX);
            }
        }
    }
}

void updateScoreDisplay()
{
    sprintf(str_score, "%d", score);
    VDP_clearText(38, 2, 3);
    VDP_drawText(str_score, 38, 2);
}

void showText(char s[])
{
    VDP_drawText(s, 20 - strlen(s) / 2, 15);
}

void endGame()
{
    showText(msg_reset);
    game_on = FALSE;
}
void startGame()
{
    score = 0;
    VDP_clearTextArea(0, 10, 40, 10);
    initBall(&mainBall);
    initPlayer(&mainPlayer);
    game_on = TRUE;
    updateScoreDisplay();
}