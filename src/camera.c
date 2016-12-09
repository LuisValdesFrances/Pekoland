#include <stdlib.h>
#include <stdio.h>
#include <gb/gb.h>
#include "camera.h"

struct Camera {
    UINT16 x;
    UINT16 y;
    UINT16 lastX;
};

struct Camera *newCamera(){
    struct Camera *c = malloc(sizeof(struct Camera));
    c->x = 0;
    c->y = 0;
    c->lastX = c->x;
}

void deleteCamera(struct Camera *c){
    /*
    if(p != null){
        free(p);
    }
    */
}

UINT16 getCameraX(struct Camera *c){
    return c->x;
}
UINT16 getCameraY(struct Camera *c){
    return c->y;
}
UINT16 getCameraLastX(struct Camera *c){
    return c->lastX;
}
void setCameraX(struct Camera *c, UINT16 newX){
    c->x = newX;
}
void setCameraY(struct Camera *c, UINT16 newY){
    c->y = newY;
}
void setCameraLastX(struct Camera *c, UINT16 newLastX){
    c->lastX = newLastX;
}

UINT16 getScrollX(UINT16 objX, UINT16 levelWidth, UINT16 screenWidth) {

    if(objX > (screenWidth>>1)){
        if(objX < levelWidth - (screenWidth>>1)){
            return objX - (screenWidth>>1);
        }else{
            return levelWidth - screenWidth;
        }
    }else{
        return 0;
    }
}


