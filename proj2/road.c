#include "road.h"

/* Suggested TODO - Write a function to create road.
 */
RoadData* createRoad(int len, int green, int red, int cycle, int starts, int ends){
  int i;
    RoadData* newRoad = (RoadData *)malloc( sizeof(RoadData) );
    newRoad->length = len;
    newRoad->whenGreen = green;
    newRoad->whenRed = red;
    newRoad->whenReset = cycle;
    newRoad->startsFrom = starts;
    newRoad->endsFrom = ends;
    newRoad->cars = (Car**)malloc(sizeof(Car*) * len);
    newRoad->trafficLight = 0;
    for(i = 0; i < len; i++){
      newRoad->cars[i] = NULL;
    }
    newRoad->carQueue = createQueue();
    return newRoad;
}
/* Suggested TODO - Write a function to free a road.
 */
void freeRoad(RoadData* road){
  free(road->cars);
  freeQueue(road->carQueue);
  free(road);
}
/* Suggested TODO - Write a function that attempts to move the cars on a given road forward (if space ahead of them was empty).
 */
void moveCarsFoward(RoadData* road) {
  int i;
  bool newInsert = false;
  
  if(road->cars[road->length-1] == NULL)
    newInsert = true;
    
  for (i = road->length-1; i > 0; i--) {
       if (road->cars[i] != NULL && road->cars[i-1] == NULL && road->cars[i]->moved == false) {
           road->cars[i]->moved = true;
           road->cars[i-1] = road->cars[i];
           road->cars[i] = NULL;
           
       }
   }
   if(newInsert == true){
     road->cars[road->length-1] = dequeue(road->carQueue);
     }
     for (i = road->length-1; i > 0; i--) {
       if(road->cars[i] != NULL)
           road->cars[i]->moved = false;
       }
}
/* Suggested TODO - Write a function to attempt to move the car at the first position on a given road
 * to the last position of a another given road.
 */
void moveCarToNextRoad(RoadData* road, int timing){
  /*int nextRoad;
  Car* car = road->cars[0];
  road->cars[0] = NULL;
  if(car->destination == road->endsFrom){
    printCarDestinationReached(car, timing);
    freeCar(car);
    return 0;
  }
  else{
    return -1;
  }*/
    
}
/* Suggested TODO - Write a function to attempt to move a car from the add car queue onto the end of this road.
 */
void enqueueCarToRoad(RoadData* road, Car* car){
  enqueue(road->carQueue, car);
}
/* Suggested TODO - Write a function to update the current state of the traffic light based on the current cycle.
 */
void updateTrafficLights(RoadData* road, int time){
  int cycleTime = time % road->whenReset;
  road->trafficLight = (cycleTime >= road->whenGreen && cycleTime < road->whenRed) ? 1 : 0;
}
/* Suggested TODO - Write a function to print the destinations of the cars on the road
 */
void printRoad(RoadData* road, int timing){
  int u;
    printf("Cars on the road from %d to %d: \n", road->startsFrom, road->endsFrom);
    for(u = 0; u < road->length; u++){
      if(road->cars[u] == NULL)
        printf("- ");
      else
        printf("%d ", road->cars[u]->destination);
    }
    if(road->trafficLight == 0)
      printf("(GREEN light)\n");
    else
      printf("(RED light)\n");
}

void printCar(Car* car, int timing){
printf("STEP %d - ADD_CAR_EVENT - Cars enqueued on road from %d to %d\n", timing, car->origin, car->next);
}

void printCarDestinationReached(Car* car, int timing){
  printf("STEP %d - Car successfully traveled from %d to %d in %d time steps\n", timing, car->origin, car->destination, timing+1);
}

