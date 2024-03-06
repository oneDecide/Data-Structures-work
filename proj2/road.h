#ifndef _road_h
#define _road_h
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "car.h"
#include "queue.h"



typedef struct RoadData
{
    /* TODO - Add data associated with road.  Some suggested data: */
    /* length of this road */
    int length;
    /* information used to record/update whether the light at the end of this road is green or red */
    int whenGreen;
    int whenRed;
    int whenReset;
    int trafficLight;
    /* intersections this road starts from and moves to */
    int startsFrom;
    int endsFrom;
    /* Each road should have an array to track cars currently driving on them.  The type of this array can be either `Car**` 
     * (array of car pointers) or `Car*` (array of cars) but the former may be easier to use since locations on the road can 
     * be set to `NULL` if no car is present.
     */
    Car** cars;
    /* For each road it is recommended to have a `queue` of cars waiting to enter the simulated road system (see `queue.h` for 
     * the queues functions which have already been implemented for you).  Cars will be put into this queue only when an add 
     * car event occurs for this road.
     */
    Queue* carQueue;

}  RoadData;

RoadData* createRoad(int len, int green, int red, int cycle, int starts, int ends);
void freeRoad(RoadData* road);
void moveCarsFoward(RoadData* road);
void moveCarToNextRoad(RoadData* road, int timing);
void enqueueCarToRoad(RoadData* road, Car* car);
void updateTrafficLights(RoadData* road, int time);

void printRoad(RoadData* road, int timing);
void printCar(Car* car, int timing);
void printCarDestinationReached(Car* car, int timing);

#endif

