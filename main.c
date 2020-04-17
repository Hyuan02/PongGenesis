#include <genesis.h>
#include "resources.h"

//BOUNDS TO MEGADRIVE RES
const int LEFT_EDGE = 0;
const int RIGHT_EDGE = 320;
const int TOP_EDGE = 0;
const int BOTTOM_EDGE = 224;

typedef struct{
    Sprite* ball;
    int x;
    int y;
    int width;
    int height;
    int velX;
    int velY;
} Ball;

typedef struct {
    Sprite* paddle;
    int x;
    int y;
    int velX;
    int width;
    int height;
} Player;

Ball mainBall;
Player mainPlayer;


void myJoyHandler(u16 joy, u16 changed, u16 state){
    if(joy == JOY_1){
        if(state & BUTTON_RIGHT){
            addVelocity(&mainPlayer, 3);
        }
        else if(state & BUTTON_LEFT){
            addVelocity(&mainPlayer, -3);
        }
        else{
            if((changed & BUTTON_RIGHT) | (changed & BUTTON_LEFT)){
                addVelocity(&mainPlayer, 0);
            }
        }
    }
}

int main() //The main function and entry point
{
    initBall(&mainBall);
    initPlayer(&mainPlayer);
    JOY_init();
    JOY_setEventHandler( &myJoyHandler );
    VDP_drawText("Hyuan's Incredible Game", 8, 12); //Draw a text on the screen
    VDP_loadTileSet(bgtile.tileset,1,DMA);
    VDP_setPalette(PAL1, bgtile.palette->data);
    SPR_init(0,0,0);
    mainBall.ball = SPR_addSprite(&imgball,10,10,TILE_ATTR(PAL1,0, FALSE, FALSE));
    mainPlayer.paddle = SPR_addSprite(&paddle,mainPlayer.x , mainPlayer.y, TILE_ATTR(PAL1, 0, FALSE, FALSE));
    VDP_fillTileMapRect(PLAN_B,TILE_ATTR_FULL(PAL1,0,FALSE, FALSE,1),0,0,40,30);

    while(1) //The main game loop
    {
        
        moveBall(&mainBall);
        movePlayer(&mainPlayer);
        SPR_update();
        VDP_waitVSync(); //wait for screen refresh
    }
    return (0); //End the program
}


void moveBall(Ball* mainBall){

    if(mainBall->x < LEFT_EDGE){
        mainBall->x = LEFT_EDGE;
        mainBall->velX = -mainBall->velX;
    } 
    else if(mainBall->x + mainBall->width > RIGHT_EDGE){
        mainBall->x = RIGHT_EDGE - mainBall->width;
        mainBall->velX = -mainBall->velX;
    }
    if(mainBall->y < TOP_EDGE){
	    mainBall->y = TOP_EDGE;
	    mainBall->velY = -mainBall->velY;
    } 
    else if(mainBall->y + mainBall->height > BOTTOM_EDGE){
	    mainBall->y = BOTTOM_EDGE - mainBall->height;
	    mainBall->velY = -mainBall->velY;
    }

    mainBall->x += mainBall->velX;
    mainBall->y += mainBall->velY;
    SPR_setPosition(mainBall->ball,mainBall->x,mainBall->y);
}

void initBall(Ball* someBall){
    someBall->x = 100;
    someBall->y = 100;
    someBall->width = 8;
    someBall->height = 8;
    someBall->velX = 1;
    someBall->velY = 1;
}

void initPlayer(Player* somePlayer){
    somePlayer->x = 144;
    somePlayer->y = 200;
    somePlayer->velX = 0;
    somePlayer->width = 32;
    somePlayer->height = 8;
}

void addVelocity(Player* somePlayer, int velocity){
    somePlayer->velX = velocity;
}


void movePlayer(Player* somePlayer){
    somePlayer->x += somePlayer->velX;
    if(somePlayer->x < LEFT_EDGE) somePlayer->x = LEFT_EDGE;
    if(somePlayer->x > RIGHT_EDGE) somePlayer->x = RIGHT_EDGE - somePlayer->width;
    SPR_setPosition(somePlayer->paddle, somePlayer->x, somePlayer->y);
}
