#ifndef __PLAYER_H__
#define __PLAYER_H__


struct Player *newPlayer();

void deletePlayer(struct Player *p);

UINT16 getPlayerX(struct Player *p);

UINT16 getPlayerY(struct Player *p);

UINT16 getPlayerVelocityAsc(struct Player *p);

UINT16 getPlayerVelocityDesc(struct Player *p);

UINT16 getPlayerFrame(struct Player *p);

UINT16 getPlayerState(struct Player *p);

void setPlayerX(struct Player *p, UINT16 newX);

void setPlayerY(struct Player *p, UINT16 newY);

void setPlayerVelocityAsc(struct Player *p, UINT16 newVelocityAsc);

void setPlayerVelocityDesc(struct Player *p, UINT16 newVelocityDesc);

void setPlayerFrame(struct Player *p, UBYTE newFrame);

void setPlayerState(struct Player *p, UBYTE newState);
