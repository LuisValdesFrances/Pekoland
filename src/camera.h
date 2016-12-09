#ifndef __CAMERA_H__
#define __CAMERA_H__

struct Camera *newCamera();

void deleteCamera(struct Camera *c);

UINT16 getCameraX(struct Camera *c);
UINT16 getCameraY(struct Camera *c);
UINT16 getCameraLastX(struct Camera *c);
void setCameraX(struct Camera *c, UINT16 newX);
void setCameraY(struct Camera *c, UINT16 newY);
void setCameraLastX(struct Camera *c, UINT16 newLastX);
UINT16 getScrollX(UINT16 objX, UINT16 levelWidth, UINT16 screenWidth);
