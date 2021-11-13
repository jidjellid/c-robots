#ifndef __MISSILE__H___
#define __MISSILE__H___

#define EXPLODING 2
#define ACTIVE 1
#define INACTIVE 0
#define MAX_SPEED_MISSILE 500

typedef struct missile{
  int originX, originY;
  int nextX,nextY,posX,posY;
  int angle,range;
  int state;
} missile;

#endif