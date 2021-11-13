#ifndef __ROBOT__C___
#define __ROBOT__C___

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "missile.h"
#include "robot.h"
#include "config.c"
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

//Initialize robots
void create_robots(robot robots[]){
  int i,posX,posY;

  for (i = 0; i< NUMBER_ROBOTS; i++){
    //creating random position x and y
    posX = rand()%WIDTH;
    posY = rand()%HEIGHT;

    //At begin the origine position and the current position are the same
    robots[i].posX = posX;
    robots[i].posY = posY;
    robots[i].nextX = posX;
    robots[i].nextY = posY;

    //Initialiazing missiles
    //The missiles had the same position as the robot
    for (int j = 0; j < NUMBER_MISSILES; j++){
      memset(&robots[i].missiles[j],0,sizeof(missile));
      robots[i].missiles[j].state = INACTIVE;
    }

    //Initialize the basic variables
    robots[i].robline = 0;
    robots[i].robdone = 0;
    robots[i].id = i;
    robots[i].speed = 0;
    robots[i].state = ALIVE;
    robots[i].life = POINT_LIFE;
    robots[i].memory = malloc(1000*sizeof(int));
  }
}

//Function for damages in percentage
void damages(robot * r , int d){
  if(r->state == ALIVE){
    r->life = (r->life - d);    

    if(r->life <= 0){
      r->life = 0;
      r->state = DEAD;
      r->speed = 0;
    }
  }
}


//Verify if the robot is dead or no
int is_dead(robot * r){
  return (r->state == DEAD || r->life <= 0);
}

//return the square of an integer
double sqr(double x){
  return x*x;
}

//Return random number
int random_number(int max){
  srand((unsigned) time(0));
  return rand() % max;
}

//Return the distance between 2 points
int distance(double x1,double y1,double x2,double y2){
  return (int)sqrt(sqr(y2-y1)+sqr(x2-x1));
}


//Return the angle between 2 points
int angle(double x1,double y1,double x2,double y2){
  double res = atan2(y2-y1,x2-x1); 
  return (int)(res*180)/M_PI;
}

//Function ENGINE angle speed
void engine(robot * r,int angle, int speed){
  r->speed = (int)(((double)speed/100)*SPEED_MAX);
  r->angle = angle;
}

//Function WAIT delay
void wait(robot * r, int delay){
  r->wait = delay;
}

//Function SELF
int self(robot * r){
  return r->id;
}

//Function SPEED
int speed(robot * r){
  return r->speed;
}

//Function STATE num
int state(robot r[], int id){
  return r[id].life;
}

//Function GPSX num
int gpsx(robot r[], int id){
  return r[id].posX;
}

//Function GPSY num
int gpsy(robot r[], int id){
  return r[id].posY;
}

//Function CARDINAL
int cardinal(){
  return NUMBER_ROBOTS;
}

//Function POKE address value
void poke(robot * r2, int adress, int value){
  r2->memory[adress] = value;
}

//Function PEEK address
int peek(robot * r2, int adress){
  return r2->memory[adress];
}

//Function TARGETX x1 angle length
int targetx(int x1, int angle, int length){
  return  (int)( x1 + ( length * cos(angle) ));
}

//Function TARGETY y1 angle length
int targety(int y1, int angle, int length){
  return  (int)( y1 + ( length * sin(angle) ));
}

//Function SHOOT angle distance
void shoot(robot * r1, int angle, int distance){
  //Verify if there is a missile ready to be used   
  for(int i = 0 ;i < NUMBER_MISSILES ; i++){
    if(r1->missiles[i].state == INACTIVE){
      r1->missiles[i].state = ACTIVE;
      r1->missiles[i].angle = angle;
      r1->missiles[i].range = distance;
      r1->missiles[i].nextX = r1->posX;
      r1->missiles[i].posX = r1->posX;
      r1->missiles[i].nextY = r1->posY;
      r1->missiles[i].posY = r1->posY;
      r1->missiles[i].originX = r1->posX;
      r1->missiles[i].originY = r1->posY;
      break;
    }
  }
}

//Function that explode the missile and apply damages on the robots near the explosion
void explode(robot r[],missile * m){
  if(m->state == ACTIVE){
    m->state = INACTIVE;
    for(int i = 0;i < NUMBER_ROBOTS;i++){
      int dist = distance(m->posX,m->posY,r[i].posX,r[i].posY);
      if(dist <= 50){
        damages(&r[i],10);
      } else if(dist <= 200){
        damages(&r[i],5);
      } else if(dist <= 400){
        damages(&r[i],3);
      }
    }
  }
}

//Return 1 if positions are good, 0 else
int isNotOver(robot r[]){
  int loop = 0;
  for(int i = 0; i < 4; i++){
    //Robot position
    if(r[i].posX != r[i].nextX || r[i].posY != r[i].nextY){
      loop = 1;
    }

    //Missile position
    for(int y = 0; i < 2; i++){
      if(r[i].missiles[y].posX != r[i].missiles[y].nextX || r[i].missiles[y].posY != r[i].missiles[y].nextY){
        loop = 1;
      }
    }
  }
  return loop;
}

void goToNextPos(robot r[]){
  //Distance between actual pos and the pos to reach
  int distArrayRobot [4];
  int distArrayMissile [8];

  for(int i = 0; i < 4; i++){
    distArrayRobot[i] = distance(r[i].posX,r[i].posY,r[i].nextX,r[i].nextY);
    for(int y = 0; y < 2; y++){
      distArrayMissile[i*2+y] = distance(r[i].missiles[y].posX,r[i].missiles[y].posY,r[i].missiles[y].nextX,r[i].missiles[y].nextY);
    }
  }

  while(isNotOver(r)){

    //Update the position of every robot and missile step by step to check for collisions
    for(int i = 0; i < 4; i++){

      //Robot position
      if(distArrayRobot[i] > 0){
        if(r[i].posX != r[i].nextX || r[i].posY != r[i].nextY){
          r[i].posX += (int)(1*cos((double)r[i].angle*(M_PI / 180.0)));
          r[i].posY += (int)(1*sin((double)r[i].angle*(M_PI / 180.0)));
          distArrayRobot[i] -= 1;
        }

        if(r[i].posX < 0){
          r[i].speed = 0;
          r[i].posX = 0;
          r[i].nextX = 0;
          r[i].nextY = r[i].posY;
          damages(&r[i],3);
        }

        if(r[i].posX > 10000){
          r[i].speed = 0;
          r[i].posX = 10000;
          r[i].nextX = r[i].posX;
          r[i].nextY = r[i].posY;
          damages(&r[i],3);
        }

        if(r[i].posY < 0){
          r[i].speed = 0;
          r[i].posY = 0;
          r[i].nextX = r[i].posX;
          r[i].nextY = r[i].posY;
          damages(&r[i],3);
        }

        if(r[i].posY > 10000){
          r[i].speed = 0;
          r[i].posY = 10000;
          r[i].nextX = r[i].posX;
          r[i].nextY = r[i].posY;
          damages(&r[i],3);
        }

      } else {
        r[i].posX = r[i].nextX;
        r[i].posY = r[i].nextY;
      }

      //Missile position
      for(int y = 0; y < 2; y++){

        //Update missile position step by step
        if(distArrayMissile[i*2+y] > 0 && r[i].missiles[y].state == ACTIVE && (r[i].missiles[y].posX != r[i].missiles[y].nextX || r[i].missiles[y].posY != r[i].missiles[y].nextY)){
          r[i].missiles[y].posX += (int)(5*cos((double)r[i].missiles[y].angle*(M_PI / 180.0)));
          r[i].missiles[y].posY += (int)(5*sin((double)r[i].missiles[y].angle*(M_PI / 180.0)));
          distArrayMissile[i*2+y] -= 5;

          //Check if the missile got further than its max range
          int traveled = distance(r[i].missiles[y].posX,r[i].missiles[y].posY,r[i].missiles[y].originX,r[i].missiles[y].originY);
          if(traveled > r[i].missiles[y].range || traveled > 7000 || r[i].missiles[y].posX > 10000 || r[i].missiles[y].posX < 0 || r[i].missiles[y].posY > 10000 || r[i].missiles[y].posY < 0){
            explode(r,&r[i].missiles[y]);
          }

        } else {
          r[i].missiles[y].posX = r[i].missiles[y].nextX;
          r[i].missiles[y].posY = r[i].missiles[y].nextY;
        }
      }

      //Check for collisions
      for(int k = 0; k < 4; k++){
        if(i != k){
          //If collides with another robot
          if(distance(r[i].posX,r[i].posY,r[k].posX,r[k].posY) <= 10 && r[k].state != 0){
            r[i].speed = 0;
            r[k].speed = 0;
            
            r[i].posX += -1*(int)(10*cos((double)r[i].angle*(3.14159265 / 180.0)));
            r[i].posY += -1*(int)(10*sin((double)r[i].angle*(3.14159265 / 180.0)));

            r[i].nextX = r[i].posX;
            r[i].nextY = r[i].posY;

            r[k].nextX = r[k].posX;
            r[k].nextY = r[k].posY;

            damages(&r[i],3);
            damages(&r[k],3);
          }

          //If collides with a missile
          for(int m = 0; m < 2; m++){
            if(r[i].missiles[m].state == ACTIVE && distance(r[i].missiles[m].posX,r[i].missiles[m].posY,r[k].posX,r[k].posY) <= 10 && r[k].state == ALIVE){
              explode(&r[i],&r[i].missiles[m]);
            }
          }
        }
      }
    }
  }
}

//Update the robot position every cycle
void update_robots(robot r[]){
  
  //Update every robots
  for (int i = 0; i<NUMBER_ROBOTS; i++){
    if(r[i].robdone == 2){
      r[i].speed = 0;
      r[i].state = DEAD;
    }
    
    r[i].nextX = r[i].posX + (int)(r[i].speed*cos((double)r[i].angle*(M_PI / 180.0)));
    r[i].nextY = r[i].posY + (int)(r[i].speed*sin((double)r[i].angle*(M_PI / 180.0)));

    //Update missiles for that robot
    for(int y = 0 ;y < NUMBER_MISSILES ; y++){
      if(r[i].missiles[y].state == ACTIVE){
        r[i].missiles[y].nextX = r[i].missiles[y].posX + (int)(500*cos((double)r[i].missiles[y].angle*(M_PI / 180.0)));
        r[i].missiles[y].nextY = r[i].missiles[y].posY + (int)(500*sin((double)r[i].missiles[y].angle*(M_PI / 180.0)));
      }
    }
  }

  goToNextPos(r);
}

//Print the current state of a robot and its missiles
void to_string(robot r){
  printf("Le robot %d a %d points de vie possede une vitesse de %d son etat est %d, possede  2 missiles  : missile 1 : %d , missile 2 : %d\n",r.id,r.life,r.speed,r.state,r.missiles[0].state,r.missiles[1].state);
}

#endif