#ifndef __ROBOT__H___
#define __ROBOT__H___

#define POINT_LIFE 100
#define SPEED_MAX 10

#define DAMAGES_SUFFERED 0
#define DAMAGES_MAX 100

#define NUMBER_MISSILES 2

#define ALIVE 1
#define DEAD 0

#define MEMORY 128

typedef struct robot{
  int robline, robdone;
  int wait;
  int state;
  int id;
  int posX,posY;
  int nextX, nextY;
  int speed, angle;
  int life;
  missile missiles[NUMBER_MISSILES]; 
  int * memory;
} robot ;

#endif
