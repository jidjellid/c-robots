#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "robot.c"
#include "robot.h"


#define LINE_SIZE 128
#define NB_LINES 50

int parseExpression(int robotNb, robot robots[]);
void deleteSpaces();

char * pos = NULL;
size_t len = 0;

int parseDigit(int robotNb, robot robots[]){
  if(*pos >= '0' && *pos <= '9'){
    return *pos++ - '0';
  } else {
    robots[robotNb].robdone = 2;
    return -1;
  }
  return 0;
}

int parseNumber(int robotNb, robot robots[]){
  int number = 0;

  while(*pos >= '0' && *pos <= '9'){
    number = number * 10;
    number += parseDigit(robotNb,robots);
  }

  return number;
}

int parseInteger(int robotNb, robot robots[]){
  if(*pos == '-'){
    pos++;
    int val = parseNumber(robotNb,robots);
    return -val;
  } else {
    int val = parseNumber(robotNb,robots);
    return val;
  }
}

char * parseWord(int robotNb, robot robots[]){
  deleteSpaces();
  char * val = pos;
  char * buf = malloc(25);
  int p = 0;

  while(1){
    if(*val == ' ' || *val == '\n' || strcmp(val,"EOF")==0 || p > 19){
      break;
    } else {
      buf[p] = *val;
    }
    p++;
    val++;
  }

  pos = val;
  deleteSpaces();
  return buf;
}

void deleteSpaces(){
  while(*pos == ' '){
    pos++;
  }
}

int parseOperator(int robotNb, robot robots[]){
  deleteSpaces();
  int x = parseExpression(robotNb, robots);
  deleteSpaces();
  if(*pos == '+'){
    pos++;
    deleteSpaces();
    int y = parseExpression(robotNb, robots);
    deleteSpaces();
    return x + y;
  } else if(*pos == '-'){
    pos++;
    deleteSpaces();
    int y = parseExpression(robotNb, robots);
    deleteSpaces();
    return x - y;
  } else if(*pos == '*'){
    pos++;
    deleteSpaces();
    int y = parseExpression(robotNb, robots);
    deleteSpaces();
    return x * y;
  } else if(*pos == '/'){
    pos++;
    deleteSpaces();
    int y = parseExpression(robotNb, robots);
    deleteSpaces();
    return x / y;
  } else if(*pos == '%'){
    pos++;
    deleteSpaces();
    int y = parseExpression(robotNb, robots);
    deleteSpaces();
    return x % y;
  } else {
    robots[robotNb].robdone = 2;
    return -1;
  }
}

int parseExpression(int robotNb, robot robots []){
  deleteSpaces();
  if(*pos >= '0' && *pos <= '9'){
    int val = parseInteger(robotNb,robots);
    return val;
  } else if(*pos == '(') {
    pos++; //Consume (
    int val = parseOperator(robotNb, robots);
    pos++; //Consume )
    return val;
  } else {
    char * val = parseWord(robotNb,robots);
    if(strcmp(val,"PEEK") == 0){
      int adress = parseExpression(robotNb, robots);
      if(robots[robotNb].robdone != 2){
        return peek(&robots[robotNb],adress);
      }
    } else if(strcmp(val,"RAND") == 0){
      int max = parseExpression(robotNb, robots);
      return random_number(max);
    } else if(strcmp(val,"CARDINAL") == 0){
      return cardinal();
    } else if(strcmp(val,"SELF") == 0){
      return self(&robots[robotNb]);
    } else if(strcmp(val,"SPEED") == 0){
      return speed(&robots[robotNb]);
    } else if(strcmp(val,"STATE") == 0){
      int id = parseExpression(robotNb, robots);
      return state(robots, id);
    } else if(strcmp(val,"GPSX") == 0){
      int id = parseExpression(robotNb, robots);
      return gpsx(robots, id);
    } else if(strcmp(val,"GPSY") == 0){
      int id = parseExpression(robotNb, robots);
      return gpsy(robots, id);
    } else if(strcmp(val,"ANGLE") == 0){
      int x = parseExpression(robotNb, robots);
      int y = parseExpression(robotNb, robots);
      int a = parseExpression(robotNb, robots);
      int b = parseExpression(robotNb, robots);
      int val = angle(x,y,a,b);
      return val;
    } else if(strcmp(val,"TARGETX") == 0){
      int x = parseExpression(robotNb, robots);
      int y = parseExpression(robotNb, robots);
      int z = parseExpression(robotNb, robots);
      int val = targetx(x,y,z);
      return val;
    } else if(strcmp(val,"TARGETY") == 0){
      int x = parseExpression(robotNb, robots);
      int y = parseExpression(robotNb, robots);
      int z = parseExpression(robotNb, robots);
      int val = targety(x,y,z);
      return val;
    } else if(strcmp(val,"DISTANCE") == 0){
      int x = parseExpression(robotNb, robots);
      int y = parseExpression(robotNb, robots);
      int a = parseExpression(robotNb, robots);
      int b = parseExpression(robotNb, robots);
      int val = angle(x,y,a,b);
      return val;
    }
    robots[robotNb].robdone = 2;
    return -1;
  }
}

int parseCondition(int robotNb, robot robots []){
  deleteSpaces();
  int val1 = parseExpression(robotNb, robots);
  char * comparator = parseWord(robotNb,robots);
  int val2 = parseExpression(robotNb, robots);
  if(strcmp(comparator,"<") == 0){
    return val1 < val2;
  } else if(strcmp(comparator,"<=") == 0){
    return val1 <= val2;
  } else if(strcmp(comparator,"=") == 0){
    return val1 == val2;
  } else if(strcmp(comparator,"<>") == 0){
    return val1 != val2;
  } else if(strcmp(comparator,">=") == 0){
    return val1 >= val2;
  } else if(strcmp(comparator,">") == 0){
    return val1 > val2; 
  } else {
    robots[robotNb].robdone = 2;
    return -1;
  }
}

void parseCommand(int robotNb, robot robots []){
  char * val = parseWord(robotNb,robots);
  if(strcmp(val,"WAIT") == 0){
    int delay = parseExpression(robotNb, robots);
    if(robots[robotNb].robdone != 2){
      wait(&robots[robotNb],delay);
      robots[robotNb].robdone = 1;
    }
  } else if(strcmp(val,"POKE") == 0){
    int adress = parseExpression(robotNb, robots);
    int value = parseExpression(robotNb, robots);
    if(robots[robotNb].robdone != 2){
      poke(&robots[robotNb],adress,value);
      robots[robotNb].robdone = 1;
    }
  } else if(strcmp(val,"GOTO") == 0){
    int nb = parseNumber(robotNb,robots);
    robots[robotNb].robline = nb;
  } else if(strcmp(val,"IF") == 0){
    int b = parseCondition(robotNb, robots);
    if(b == 1){
      parseWord(robotNb,robots);//THEN
      int nb = parseNumber(robotNb,robots);
      robots[robotNb].robline = nb;
    } else {
      robots[robotNb].robline++;
    }
  } else if(strcmp(val,"ENGINE") == 0){
    int angle = parseExpression(robotNb, robots);
    int speed = parseExpression(robotNb, robots);
    if(robots[robotNb].robdone != 2){
      engine(&robots[robotNb], angle, speed);
      robots[robotNb].robdone = 1;
      //printf("Engine parse(%i,%i,%i)\n",robots[robotNb].id,angle,speed); 
    }
    
  } else if(strcmp(val,"SHOOT") == 0){
    int angle = parseExpression(robotNb, robots);
    int distance = parseExpression(robotNb, robots);
    if(robots[robotNb].robdone != 2){
      shoot(&robots[robotNb], angle, distance);
      robots[robotNb].robdone = 1;
      //printf("Shoot parse(%i,%i,%i)\n",robotNb,angle,distance);
    }
  } else {
    robots[robotNb].robdone = 2;
  }
}

void parseProgram(FILE * pointer, int robotNb, robot robots []){
  rewind(pointer);
  int checked = 0;
  pos = NULL;
  while((getline(&pos, &len, pointer)) != -1){
    if(strcmp(pos,"//") != 0){
      int x = parseNumber(robotNb,robots);
      //printf("Checking line (%i vs %i) with robot n°%i\n",x,robots[robotNb].robline,robotNb);
      if(x == robots[robotNb].robline){
        checked = 1;
        //printf("The line %i (vs %i) is the correct one !  = %i\n",x,robots[robotNb].robline);
        parseCommand(robotNb, robots);
        if(robots[robotNb].robdone == 1){
          robots[robotNb].robline++;
        }
        break;
      }
    }
  }
  if(checked == 0){
    robots[robotNb].robdone = 2;//Robot is done permanently
  }
}

//Start a new cycle for every robot
void cycleRobot(FILE * rob0, FILE * rob1, FILE * rob2, FILE * rob3, robot robots []){
  
  for(int i = 0; i < 4; i++){//Reset the robots
    //printf("robdone[%i] = %i\n",i,robdone[i]);
    if(robots[i].robdone != 2){
      robots[i].robdone = 0;
    }
  }

  while(robots[0].robdone == 0){
    if(robots[0].wait != 0){
      robots[0].wait--;
      robots[0].robdone = 1;
    } else {
      parseProgram(rob0, 0, robots);
    }
  }

  while(robots[1].robdone== 0){
    if(robots[1].wait != 0){
      robots[1].wait--;
      robots[1].robdone = 1;
    } else {
      parseProgram(rob1, 1, robots);
    }
  }

  while(robots[2].robdone == 0){
    if(robots[2].wait != 0){
      robots[2].wait--;
      robots[2].robdone = 1;
    } else {
      parseProgram(rob2, 2, robots);
    }
  }

  while(robots[3].robdone == 0){
    if(robots[3].wait != 0){
      robots[3].wait--;
      robots[3].robdone = 1;
    } else {
      parseProgram(rob3, 3, robots);
    }
  }
  
  //printf("Entire cycle is over !\n\n");
}
