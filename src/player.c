#include <gb/gb.h>
#include <stdio.h>
#include <stdlib.h>

struct Player {
    UINT16 x;
    UINT16 y;
    UINT16 newX;
    UINT16 newY;
    UINT16 velocityDesc;
    UINT16 velocityAsc;
    UBYTE state;
    UBYTE frame;
};

struct Player *newPlayer(){
    struct Player *p = malloc(sizeof(struct Player));
    p->x = 0;
    p->y = 0;
    p->newX = p->x;
    p->newY = p->y;
    p->velocityDesc = 0;
    p->velocityAsc = 0;
    p->state = 0;
    p->frame = 0;
}

void deletePlayer(struct Player *p){
    /*
    if(p != null){
        free(p);
    }
    */
}

UINT16 getPlayerX(struct Player *p){
    return p->x;
}
UINT16 getPlayerY(struct Player *p){
    return p->y;
}
UINT16 getPlayerVelocityAsc(struct Player *p){
    return p->velocityAsc;
}
UINT16 getPlayerVelocityDesc(struct Player *p){
    return p->velocityDesc;
}
UINT16 getPlayerFrame(struct Player *p){
    return p->frame;
}
UINT16 getPlayerState(struct Player *p){
    return p->state;
}
void setPlayerX(struct Player *p, UINT16 newX){
    p->x = newX;
}
void setPlayerY(struct Player *p, UINT16 newY){
    p->y = newY;
}
void setPlayerVelocityAsc(struct Player *p, UINT16 newVelocityAsc){
    p->velocityAsc = newVelocityAsc;
}
void setPlayerVelocityDesc(struct Player *p, UINT16 newVelocityDesc){
    p->velocityDesc = newVelocityDesc;
}
void setPlayerFrame(struct Player *p, UBYTE newFrame){
    p->frame = newFrame;
}
void setPlayerState(struct Player *p, UBYTE newState){
    p->state = newState;
}

