#include <gb/gb.h>
#include <stdio.h>
#include <time.h>
#include <gb/drawing.h>
#include "collision.h"

#define TRUE 1
#define FALSE 0

#define SPRITES_MODE 2 //8X8 = 1. 8X16 = 2
#define PRECISION_BITS 6
#define INC_BITS <<PRECISION_BITS
#define DEC_BITS >>PRECISION_BITS

#define GRAVITY 4

//CHARACTERS
#define MAX_ENEMY_NUMBER 1//Limite esta en 4
//Payer
#define PLAYER_WIDTH 8
#define PLAYER_HEIGHT 16
#define PLAYER_WEIGHT 5
#define PLAYER_JUMP 2
#define PLAYER_SPEED 1
//Enemy Popo
#define ENEMY_POPO_WIDTH 8
#define ENEMY_POPO_HEIGHT 16
#define ENEMY_POPO_WEIGHT 4
#define ENEMY_POPO_SPEED 1



#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 144
#define SCREEN_WIDTH2 (SCREEN_WIDTH >> 1)
#define SCREEN_HEIGHT2 (SCREEN_HEIGHT >> 1)
#define SCREEN_WIDTH4 (SCREEN_WIDTH >> 2)
#define SCREEN_HEIGHT4 (SCREEN_HEIGHT >> 2)
#define SCREEN_WIDTH8 (SCREEN_WIDTH >> 3)
#define SCREEN_HEIGHT8 (SCREEN_HEIGHT >> 3)

//Player states
#define STATE_IDLE 0
#define STATE_RUN 1
#define STATE_JUMP 2

/**
Datos del mapa maps.c Los arrays estan implementados en maps.c
*/
#define MAP_SIZE_X 64
#define MAP_SIZE_Y 18
#define LEVEL_WIDTH (MAP_SIZE_X * 8)
#define LEVEL_HEIGHT (MAP_SIZE_Y * 8)

/**
Posicion de los tiles. El array esta implementado en spriteTiles.c
*/
#define TILE_0 0
#define TILE_1 (TILE_0 + SPRITES_MODE)
#define TILE_2 (TILE_1 + SPRITES_MODE)
#define TILE_3 (TILE_2 + SPRITES_MODE)
#define TILE_4 (TILE_3 + SPRITES_MODE)
#define TILE_5 (TILE_4 + SPRITES_MODE)
#define TILE_6 (TILE_5 + SPRITES_MODE)
#define TILE_7 (TILE_6 + SPRITES_MODE)
#define TILE_8 (TILE_7 + SPRITES_MODE)
#define TILE_9 (TILE_8 + SPRITES_MODE)
#define TILE_PLAYER_IDLE_F1 20
#define TILE_PLAYER_IDLE_F2 22
#define TILE_PLAYER_RUN_F1 24
#define TILE_PLAYER_RUN_F2 26
#define TILE_PLAYER_RUN_F3 28
#define TILE_PLAYER_JUMP_UP 30
#define TILE_PLAYER_JUMP_DOWN 32
#define TILE_ENEMY_POPO_IDLE_F1 34
#define TILE_ENEMY_POPO_IDLE_F2 36
#define TILE_BLANK 38
#define TOTAL_TILES 40 //Total + 2

//Sprites (Maximo 39)
#define SPRITE_DIGIT_1A 0
#define SPRITE_DIGIT_2A SPRITE_DIGIT_1A + 1
#define SPRITE_DIGIT_3A SPRITE_DIGIT_2A + 1
#define SPRITE_DIGIT_4A SPRITE_DIGIT_3A + 1
#define SPRITE_DIGIT_5A SPRITE_DIGIT_4A + 1
//Digits
#define SPRITE_DIGIT_1B SPRITE_DIGIT_5A + 1
#define SPRITE_DIGIT_2B SPRITE_DIGIT_1B + 1
#define SPRITE_DIGIT_3B SPRITE_DIGIT_2B + 1
#define SPRITE_DIGIT_4B SPRITE_DIGIT_3B + 1
#define SPRITE_DIGIT_5B SPRITE_DIGIT_4B + 1
//Player animations
#define SPRITE_PLAYER SPRITE_DIGIT_5B + 1 //32
//Enemy Popo animations
#define SPRITE_ENEMY_1 SPRITE_PLAYER + 1
#define SPRITE_ENEMY_2 SPRITE_ENEMY_1 + 1
#define SPRITE_ENEMY_3 SPRITE_ENEMY_2 + 1
#define SPRITE_ENEMY_4 SPRITE_ENEMY_3 + 1
#define SPRITE_ENEMY_5 SPRITE_ENEMY_4 + 1
#define SPRITE_ENEMY_6 SPRITE_ENEMY_5 + 1
#define SPRITE_ENEMY_7 SPRITE_ENEMY_6 + 1
#define SPRITE_ENEMY_8 SPRITE_ENEMY_7 + 1

extern const unsigned char MAP_TILES[];//Informacion de los tiles (imagenes)
extern const unsigned char SPRITE_TILES[];//Informacion de los tiles (imagenes)
extern const unsigned char LEVEL1[];//Colisiones y distribucion

UINT8 getNumberDigits(UINT16 number, UINT8 c);
UINT8 getDigit(UINT16 number, UINT8 digit);
void drawPoints(UINT16 points, UINT8 digits, UINT8 positionX, UINT8 positionY);
void drawNumbers(UINT8 digits, UINT16 number, UINT8 positionX, UINT8 positionY);

UBYTE keyPressUP(UBYTE key) {
    if( key & (J_UP)) {
        return 1;
    } else {
        return 0;
    }
}
UBYTE keyPressDOWN(UBYTE key) {
    if( key & (J_DOWN)) {
        return 1;
    } else {
        return 0;
    }
}
UBYTE keyPressLEFT(UBYTE key) {
    if( key & (J_LEFT)) {
        return 1;
    } else {
        return 0;
    }
}
UBYTE keyPressRIGHT(UBYTE key) {
    if( key & (J_RIGHT)) {
        return 1;
    } else {
        return 0;
    }
}
UBYTE keyPressA(UBYTE key) {
    if( key & (J_A)) {
        return 1;
    } else {
        return 0;
    }
}
UBYTE keyPressB(UBYTE key) {
    if( key & (J_B)) {
        return 1;
    } else {
        return 0;
    }
}
UBYTE keyPressSTART(UBYTE key) {
    if( key & (J_START)) {
        return 1;
    } else {
        return 0;
    }
}
UBYTE keyPressSELECT(UBYTE key) {

    if( key & (J_SELECT)) {
        return 1;
    } else {
        return 0;
    }
}

UINT8 getNumberDigits(UINT16 number, UINT8 c) {
    c++;
    if(number / 10 == 0) {
        return c;
    } else {
        number /= 10;
        return getNumberDigits(number, c);
    }
}

UINT8 getDigit(UINT16 number, UINT8 digit) {

    if(digit >= getNumberDigits(number, 0)) {
        return 0;
    } else {
        int d = number%10;
        if(digit == 0) {
            return d;
        } else {
            int n = number/10;
            digit--;
            getDigit(n, digit);
        }
    }
}

void drawPointsA(UINT16 points, UINT8 digits, UINT8 positionX, UINT8 positionY) {

    UINT8 i;
    UINT8 digit;
    for(i = 0; i < digits; i++) {
        //UINT8 tile = points[i] - 48;//Cast chat to int
        digit = getDigit(points, (digits - (i+1)));
        set_sprite_tile(i, digit * SPRITES_MODE);
        move_sprite(i, positionX + (i * 8), positionY);
    }
}

void drawPointsB(UINT16 points, UINT8 digits, UINT8 positionX, UINT8 positionY) {

    UINT8 i;
    UINT8 digit;
    for(i = 0; i < digits; i++) {
        //UINT8 tile = points[i] - 48;//Cast chat to int
        digit = getDigit(points, (digits - (i+1)));
        set_sprite_tile(i+digits, digit * SPRITES_MODE);
        move_sprite(i+digits, positionX + (i * 8), positionY);
    }
}

void drawNumbers(UINT8 digits, UINT16 number, UINT8 positionX, UINT8 positionY) {

    UINT8 digit = (number % 10);

    set_sprite_tile(digits-1, digit * SPRITES_MODE);
    move_sprite(digits-1, positionX + ((digits-1) * 8), positionY);

    if(number / 10 == 0) {
        return;
    } else {
        digits--;
        drawNumbers(digits, number / 10, positionX, positionY);
    }
}

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

struct EnemyPopo {
    UINT16 x;
    UINT16 y;
    UINT16 newX;
    UINT16 newY;
    UBYTE state;
    UBYTE frame;
    UBYTE active;
};

struct Camera {
    UINT16 x;
    UINT16 y;
    UINT16 lastX;
};

UINT16 getCameraX(UINT16 objX) {

    if(objX > SCREEN_WIDTH2){
        if(objX < LEVEL_WIDTH - SCREEN_WIDTH2){
            return objX - SCREEN_WIDTH2;
        }else{
            return LEVEL_WIDTH - SCREEN_WIDTH;
        }
    }else{
        return 0;
    }
}
UBYTE isInScreen(UINT16 scrollX, UINT16 x, UINT16 width){
    if(x < scrollX + SCREEN_WIDTH && x + width > scrollX){
        return TRUE;
    }else{
        return TRUE;
    }
}

UBYTE getPlayerFrameIdle(UBYTE gameFrame, UBYTE playerFrame) {
    if(gameFrame%30 == 0) {
        playerFrame += SPRITES_MODE;
        if(playerFrame > TILE_PLAYER_IDLE_F2){
            playerFrame = TILE_PLAYER_IDLE_F1;
        }
    }
    return playerFrame;
}

UBYTE getPlayerFrameRun(UBYTE gameFrame, UBYTE playerFrame) {
    if(gameFrame%4 == 0) {
        playerFrame += SPRITES_MODE;
        if(playerFrame > TILE_PLAYER_RUN_F3){
            playerFrame = TILE_PLAYER_RUN_F1;
        }
    }
    return playerFrame;
}

UBYTE getEnemyPopoFrameIdle(UBYTE gameFrame, UBYTE enemyFrame) {
    if(gameFrame%30 == 0) {
        enemyFrame += SPRITES_MODE;
        if(enemyFrame > TILE_ENEMY_POPO_IDLE_F2){
            enemyFrame = TILE_ENEMY_POPO_IDLE_F1;
        }
    }
    return enemyFrame;
}

UINT16 getGravitySpeed(UINT16 gravityForce, UINT16 weight){
    UINT16 newGravityForce;
    UINT16 newWeight;
    /*
    Regla de tres
    (Las pruebas se hicieron con desplazamientos de 8 (3 bits), de esta manera, si se cambia esto, la fisica se adapta)
    8               = gravityForce
    PRECISION_BITS  = newGravityForce
    8               = weight
    PRECISION_BITS  = newWeight
    */
    newGravityForce = (PRECISION_BITS * gravityForce) >> 3;
    newWeight = (PRECISION_BITS * weight) >> 3;
    return newGravityForce * newWeight;
}

UINT16 getStrongJump(UINT16 strongJump){
    /*
    Regla de tres INVERSA
    (Las pruebas se hicieron con desplazamientos de 8 (3 bits), de esta manera, si se cambia esto, la fisica se adapta)
    8               = strongJump
    PRECISION_BITS  = newStrongJump
    */
    return (8 * strongJump) / PRECISION_BITS;
}

void main() {
    //Mierda temporal
    //printf("%d",3);
    //printf("Inicio del programa");

    //Declaracion
    UINT16 frame;
    UINT16 temp;
    UINT16 temp2;
    UWORD count;

    UINT8 screenCountX;

    //Maneja el contador de numero de enemigos en el nivel
    UINT8 numberEnemiesPopo;


    UBYTE isInGround;
    UBYTE gameKeyPressed;
    UINT16 tileIndex;



    UBYTE keys;



    char digitPoints[5];
    struct EnemyPopo enemyPopoList[MAX_ENEMY_NUMBER];
    struct Player player;
    struct Camera camera;

    //Inicialización
    temp = 0;
    temp2 = 0;
    count = 0;
    screenCountX = 0;

    keys = 0;

    digitPoints[0] = 0;
    digitPoints[1] = 0;
    digitPoints[2] = 0;
    digitPoints[3] = 0;
    digitPoints[4] = 0;

    DISPLAY_OFF;//Desactiva la pantalla
    disable_interrupts();
    HIDE_SPRITES;
    HIDE_BKG;
    HIDE_WIN;

    //Tiles del fondo
    SWITCH_ROM_MBC1(2);//Banco de memoria 3
    //posicion inicial, numero y tiles
    set_bkg_data(0, 3, MAP_TILES);

    //Sprites
    //Carga en la VRAM los tiles para los sprites
    SPRITES_8x16;
    SWITCH_ROM_MBC1(3);//Salto al banco de memoria 2
    //posicion de memoria, cantidad de tiles, tiles
    set_sprite_data(0, TOTAL_TILES, SPRITE_TILES);
    //Asigna a un sprite un tile
    //numero del sprite (0-39), posicion del tile
    set_sprite_tile(SPRITE_DIGIT_1A, TILE_0);
    set_sprite_tile(SPRITE_DIGIT_2A, TILE_0);
    set_sprite_tile(SPRITE_DIGIT_3A, TILE_0);
    set_sprite_tile(SPRITE_DIGIT_4A, TILE_0);

    set_sprite_tile(SPRITE_DIGIT_1B, TILE_0);
    set_sprite_tile(SPRITE_DIGIT_2B, TILE_0);
    set_sprite_tile(SPRITE_DIGIT_3B, TILE_0);
    set_sprite_tile(SPRITE_DIGIT_4B, TILE_0);

    //set_bkg_tiles(indexX(8 pixelex), indexY(8 pixelex), ancho, alto y mapa)
    //Viejo sistema, hasta 32 tiles me vale...
    //set_bkg_tiles(0, 0, MAP_SIZE_X, MAP_SIZE_Y, LEVEL1);


    /**
    El bucle rellena la pantalla de tiles de arriba a abajo (Se recorre en Y),
    haciendo una fila completa de 22 tiles (los que entran en la pantalla)
    Al final de cada iteracion mueve el puntero 64 tiles (El tamaño en X del mapa)
    para que en la siguiente iteracion apunte a la nueva fila de la matriz
    */

    SWITCH_ROM_MBC1(4);//Banco de memoria 4
    //Seria 32 pero solo quiero llenar la parte visible de la pantalla (144 pixels = 18 tiles)
    for(count = 0; count != 18; count++ ){//18 son los tiles que entran en la pantalla en alto

        //El mapa maximo en memoria es 32 x 32
        //22 son los tiles que entran en la pantalla en ancho (160 pixels = 20 tiles)
        set_bkg_tiles(0, count, 22, 1, &(LEVEL1+temp));

         //Incrementa temp con el tamaño X del mapa para que se cargue la proxima fila
		temp = temp + MAP_SIZE_X;

    }

    SHOW_BKG;//Muestra el fondo
    SHOW_SPRITES;//Muestra los sprites
    DISPLAY_ON;
    enable_interrupts();







    //Inicializacion del player y los enemigos
    player.x = 0;
    player.y = (SCREEN_HEIGHT - PLAYER_HEIGHT - 8) INC_BITS;
    player.newX = player.x;
    player.newY = player.y;
    player.frame = TILE_PLAYER_IDLE_F1;
    player.state = STATE_IDLE;


    for(count = 0; count != MAX_ENEMY_NUMBER; count++){
        enemyPopoList[count].active = FALSE;
    }
    //Se debe de reastrar el nivel en busca de posiciones (O definirlas previamete)
    numberEnemiesPopo = 1;

    for(count = 0; count != numberEnemiesPopo; count++){
        enemyPopoList[count].x = (50 + (count * 12))INC_BITS;
        enemyPopoList[count].y = (SCREEN_HEIGHT - ENEMY_POPO_HEIGHT - 8) INC_BITS;
        enemyPopoList[count].newX = enemyPopoList[count].x;
        enemyPopoList[count].newY = enemyPopoList[count].y;
        enemyPopoList[count].frame = TILE_ENEMY_POPO_IDLE_F1;
        enemyPopoList[count].state = STATE_IDLE;
        enemyPopoList[count].active = TRUE;
    }
    frame = 0;
    while(TRUE) {

        keys = joypad();//Lee el pad

        //Set limits screen
        if(player.x < 2 INC_BITS){
            player.x = 1 INC_BITS;
        }
        else if((player.x DEC_BITS) +8 >= LEVEL_WIDTH){
            player.x = (LEVEL_WIDTH-8) INC_BITS;
        }

        //Player controls
        player.newX = player.x;
        player.newY = player.y;

        //Physics
        if(player.velocityAsc > 1 INC_BITS){
            player.velocityDesc = 0;
            player.newY -= player.velocityAsc;
            player.velocityAsc -= getGravitySpeed(GRAVITY, PLAYER_WEIGHT);
        }else{
            player.velocityAsc = 0;
            player.velocityDesc += getGravitySpeed(GRAVITY, PLAYER_WEIGHT);
            player.newY += player.velocityDesc;
            //Si los pies no tocan el suelo, continuo cayendo
            if(!isCollisonDown(player.x DEC_BITS, (player.y + (1 INC_BITS)) DEC_BITS, PLAYER_WIDTH, PLAYER_HEIGHT, MAP_SIZE_X, LEVEL1)){
                player.frame = TILE_PLAYER_JUMP_DOWN;
                player.state = STATE_JUMP;
            }
        }
        isInGround = isCollisonDown(player.x DEC_BITS, (player.y + (1 INC_BITS)) DEC_BITS, PLAYER_WIDTH, PLAYER_HEIGHT, MAP_SIZE_X, LEVEL1);

        //Inputs
        if(keyPressSTART(keys) > 0){}
        if(keyPressSELECT(keys) > 0){}
        gameKeyPressed = FALSE;
        if(keyPressUP(keys) > 0) {
        }
        else if(keyPressDOWN(keys) > 0) {
        }
        if(keyPressLEFT(keys) > 0) {
            gameKeyPressed = TRUE;
            /* this will OR the flip x bit of sprite 0 to the value 1, making the sprite flip horizontally */
            set_sprite_prop(SPRITE_PLAYER, S_FLIPX);
            player.newX -=  PLAYER_SPEED INC_BITS;
            if(player.state == STATE_IDLE || (isInGround && player.state == STATE_JUMP)){
                player.frame = TILE_PLAYER_RUN_F1;
                player.state = STATE_RUN;
            }
        }
        else if(keyPressRIGHT(keys) > 0) {
            gameKeyPressed = TRUE;
            set_sprite_prop(SPRITE_PLAYER, 0);
            player.newX +=  PLAYER_SPEED INC_BITS;
            if(player.state == STATE_IDLE || (isInGround && player.state == STATE_JUMP)){
                player.frame = TILE_PLAYER_RUN_F1;
                player.state = STATE_RUN;
            }
        }
        if(keyPressA(keys) > 0) {
        }
        if(keyPressB(keys) > 0) {
            if(isInGround){
                gameKeyPressed = TRUE;
                player.velocityAsc = getStrongJump(PLAYER_JUMP INC_BITS);
                player.frame = TILE_PLAYER_JUMP_UP;
                player.state = STATE_JUMP;

            }
        }

        //Condiciones para volver a idle
        if(player.state != STATE_IDLE && isInGround && !gameKeyPressed){
                player.frame = TILE_PLAYER_IDLE_F1;
                player.state = STATE_IDLE;
        }

        //Colisiones
        tileIndex = isCollisonDown(player.x DEC_BITS, player.newY DEC_BITS, PLAYER_WIDTH, PLAYER_HEIGHT, MAP_SIZE_X, LEVEL1);
        if(tileIndex!=0){
            player.newY = (((tileIndex / MAP_SIZE_X) * 8) - PLAYER_HEIGHT) INC_BITS;
            player.velocityDesc = 0;
        }
        tileIndex = isCollisionUp(player.x DEC_BITS, player.newY DEC_BITS, PLAYER_WIDTH, MAP_SIZE_X, LEVEL1);
        if(tileIndex!=0){
            player.newY = (((tileIndex / MAP_SIZE_X) * 8) + 8) INC_BITS;
            player.velocityAsc = 0;
            player.velocityDesc = 0;
        }
        player.y = player.newY;
        tileIndex = isCollisionRight(player.newX DEC_BITS, player.y DEC_BITS, PLAYER_WIDTH, PLAYER_HEIGHT, MAP_SIZE_X, LEVEL1);
        if(tileIndex!=0){
            player.newX = (((tileIndex % MAP_SIZE_X) * 8)-PLAYER_WIDTH) INC_BITS;
        }
        tileIndex = isCollisionLeft(player.newX DEC_BITS, player.y DEC_BITS, PLAYER_HEIGHT, MAP_SIZE_X, LEVEL1);
        if(tileIndex!=0){
            player.newX = (((tileIndex % MAP_SIZE_X) * 8)+8) INC_BITS;
        }
        player.x = player.newX;

        //Maquina de estado del player
        switch(player.state){
            case STATE_RUN:
                player.frame = getPlayerFrameRun(frame, player.frame);
                break;
            case STATE_JUMP:
                break;
            default:
                if(isInGround){
                    player.frame = getPlayerFrameIdle(frame, player.frame);
                }
                break;
        }
        set_sprite_tile(SPRITE_PLAYER, player.frame);



        //Maquina de estado de los enemigos
        for(count = 0; count != numberEnemiesPopo; count++)
        {
            temp = (SPRITE_ENEMY_1 + count);
            if(enemyPopoList[count].active == TRUE

               //&& camera.x < (enemyPopoList[count].x DEC_BITS))

               && isInScreen(camera.x, (enemyPopoList[count].x DEC_BITS), ENEMY_POPO_WIDTH) == TRUE
            ){
                    switch(enemyPopoList[count].state){
                    case STATE_RUN:
                    case STATE_IDLE:
                        enemyPopoList[count].frame = getEnemyPopoFrameIdle(frame, enemyPopoList[count].frame);
                    break;
                }

                set_sprite_tile(temp, enemyPopoList[count].frame);
            }
            else{
                set_sprite_tile(temp, TILE_BLANK);
            }
        }





        //Actualizo los valores de la camara
        camera.lastX = camera.x;
        camera.x = getCameraX(player.x DEC_BITS);
        screenCountX += (camera.x-camera.lastX);

        //Sincroniza con el blanqueo vertical
        wait_vbl_done();

        //Movimiento de los sprites
        move_sprite(SPRITE_PLAYER, (player.x DEC_BITS) - camera.x+8, (player.y DEC_BITS) + PLAYER_HEIGHT);
        for(count = 0; count != numberEnemiesPopo; count++){
            temp = (SPRITE_ENEMY_1 + count);
            if(enemyPopoList[count].active == TRUE
               )
            {

                move_sprite(temp,
                        (enemyPopoList[count].x DEC_BITS) - camera.x+8,
                        (enemyPopoList[count].y DEC_BITS) + ENEMY_POPO_HEIGHT);
            }
        }


        //Movimiento del fondo
        //move_bkg(camera.x, bgPosY);
        scroll_bkg(camera.x-camera.lastX, 0);
        //Al avanzar un tile se actualiza el mapa
        if(screenCountX == 8){
            screenCountX = 0;

            /**
            Posiciona el indice X en la posicion del vector a rellenar
            camera.x>>3 es la suma de tiles de 8x8 avanzados: camera.x/8
            Ejemplo: el primer tile que se avance sera 1 + 21 = 22
            Como el tile que queremos rellenar de mas a la derecha se encuentra fuera de la pantalla (20 tiles)
            sumo 21
            */
            temp = (camera.x>>3)+21;
            /**
            El modulo se debe a que una vez superados los 32 tiles (32*8=256) la cuenta se reseta a 0
            (Le sigue el 0, no el 33)
            Es elmotivo de porque si el personaje esta enla posicion 257, se tilea a partir de la posición 0
            */
            temp2 = temp % 32;
            //18 son los tiles que entran en la pantalla en alto
            for(count = 0; count != 18; count++ ){

                //Solo relleno el ultimo tile por la derecha
                set_bkg_tiles(temp2, count, 1, 1, &(LEVEL1+temp));

                 //incrementa Cnt con el tamaño de X para que se cargue la proxima fila
                temp = temp + MAP_SIZE_X;
            }
        }
        else if(screenCountX == -8){
            screenCountX = 0;
            /**
            Como el tile que queremos rellenar de mas a la izquierda se encuentra fuera de la pantalla
            resto -1
            */
            temp = (camera.x>>3)-1;
            /**
            El modulo se debe a que una vez superados los 32 tiles (32*8=256) la cuenta se reseta a 0
            (Le sigue el 0, no el 33)
            Es elmotivo de porque si el personaje esta enla posicion 257, se tilea a partir de la posición 0
            */
            temp2 = temp % 32;
            //18 son los tiles que entran en la pantalla en alto
            for(count = 0; count != 18; count++ ){

                //Solo relleno el ultimo tile por la derecha
                set_bkg_tiles(temp2, count, 1, 1, &(LEVEL1+temp));

                 //incrementa Cnt con el tamaño de X para que se cargue la proxima fila
                temp = temp + MAP_SIZE_X;
            }
        }








        //drawPointsA(sys_time, 5, SCREEN_WIDTH - (8*4), 8);//hasta 256
        //drawPointsB(camera.x/8, 5, SCREEN_WIDTH - (8*4), 16);//hasta 65536


        frame++;
        frame = frame%100;
    }
    //puts("gameFrame32:");

}
