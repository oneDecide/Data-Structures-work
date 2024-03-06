#include "trafficSimulator.h"

void printRoadData( int length, int from, int to, int greenStartTime, int greenEndTime, int lightCycleLength );
int max( int a, int b );

/* printNames
 * input: none
 * output: none
 *
 * Prints names of the students who worked on this solution
 * REMINDER: You may only work with one other cs2124 student
 */
void printNames( )
{
    /* TODO : Fill in you and your partner's names (or N/A if you worked individually) */
    printf("\nThis solution was completed by:\n");
    printf("Dylan Cordes\n");
    printf("Hieu Phan\n\n");
}

/* createTrafficData
 * input: char* filename of file to read
 * output: TrafficData* which stores the data of this road network
 *
 * Create a new TrafficData variable and read in the traffic data from the file whose name is stored in char* filename
 */
TrafficData* createTrafficData( char* filename )
{
   /* open the file */
    FILE *pFile = fopen( filename, "r" );
    TrafficData* toRet = (TrafficData *)malloc( sizeof(TrafficData) );
    RoadData* current = NULL;
    Event* tempEvent;
    Car* car;
    int verticies; int edges; int i; int u; int countEdges = 0;
    int incomingNum;
    int incoming; int leng;int green; int red; int resetCycle;
    int numOfCarCommands;
    int startEdge;int travelTo;int timeToPreform;
    int numOfCars;
    int printComms;
    int destination; 
    int printTime;
    
    printf("Add the roads:\n");
    /* TODO: read in all the data in pFile */
    /* HINT: use fscanf( pFile, "%d", &<your int variable here> ) to read an int from pFile */
    fscanf(pFile, "%d %d\n", &verticies, &edges);
    toRet->numOfRoads = edges;
    toRet->numOfCars = 0;
    toRet->roadSystem = createGraph(verticies);
    toRet->incrementingRoads = (RoadData**)malloc(sizeof(RoadData*) * edges);
    for(i = 0; i < verticies; i++){
      addVertex(toRet->roadSystem, i);
      }
    for(i = 0; i < verticies; i++){
      fscanf(pFile, "%d", &incomingNum);
      for(u = 0; u < incomingNum; u++){
        fscanf(pFile, "%d %d %d %d %d", &incoming, &leng, &green, &red, &resetCycle);
        current = createRoad(leng, green, red, resetCycle, incoming, i);
        setEdge(toRet->roadSystem, incoming, i, countEdges);
        toRet->incrementingRoads[countEdges] = current;
        setEdgeData( toRet->roadSystem, incoming, i, toRet->incrementingRoads[countEdges]);
        printRoadData(leng, current->startsFrom, current->endsFrom, green, red, resetCycle);
        countEdges++;
      }
    }
    
    //Create Car Commands
    fscanf(pFile,"%d", &numOfCarCommands);
    printf("Add the cars events:\n");
    toRet->eventQueue = createPQ();
    for(i = 0; i < numOfCarCommands; i++){
      fscanf(pFile, "%d %d %d",&startEdge, &travelTo, &timeToPreform);
      fscanf(pFile, "%d", &numOfCars);
      current = getEdgeData(toRet->roadSystem, startEdge, travelTo);
      tempEvent = createAddCarEvent( timeToPreform, current );
      printf("Created event for time step %d on road from %d to %d.\n", tempEvent->eventTimeStep, startEdge, travelTo);
      printf("Destinations of added cars: ");
      for(u = 0; u < numOfCars; u++){
        if(u != 0)
          printf(", ");
        fscanf(pFile, "%d", &destination);
        car = createCar( timeToPreform, startEdge, travelTo, destination );
        enqueue(tempEvent->pCarQueue, car);
        printf("%d",destination);
      }
      enqueueByPriority( toRet->eventQueue, tempEvent, tempEvent->eventTimeStep );
      
      printf("\n\n");
      
    }
    
    //Road Print Events
    printf("Add the roads events:\nTime steps: ");
    fscanf(pFile, "%d", &printComms);
    for(i = 0; i < printComms; i++){
      fscanf(pFile, "%d ", &printTime);
      enqueueByPriority( toRet->eventQueue, createPrintRoadsEvent( printTime ), printTime );
      if(i != 0)
        printf(", ");
      printf("%d", printTime);
    }
    /* HINTs:
     * Each road can be stored in a `RoadData` struct (see `road.h`).
     *
     * You'll want to store all of these `RoadData` structs in a single array.  This is important to track the order that
     * roads were added to your graph.  These are also the order they are processed in each time step.  You can store this
     * array in your TrafficData struct.
     *
     * You'll add edges to a graph to represent each road using `setEdge` (see `graph.c`).  The vertices of the graph
     * represent intersections.  The graph will do the work of finding shortest paths (see `getNextOnShortestPath` in graph.c
     * and the longest shortest path problem on Blackboard).
     *
     * It will also help to add your `RoadData` structs to your graph using `setEdgeData` (see `graph.c`).   You can then
     * retrieve that `RoadData` struct by calling `getEdgeData`.  This will be helpful for finding roads when cars pass
     * through intersections.
     *
     * Each event will be stored in an Event struct (see `event.h`).
     *
     * It's recommended to store all of the `Event` structs in a single priority queue (using time step as the priority).  This
     * allows you to quickly find the next event to execute by time step (see `enqueueByPriority`, `getFrontPriority`, and
     * `dequeuePQ`).
     *
     * Each car is stored in a Car struct (see `car.h`).
     */


    /* close file */
    fclose( pFile );

    return toRet; /* TODO: Replace this with your TrafficData pointer */

}

/* Print color coding for printRoadData (may not work outside of Linux environment) */
#define PRINT_COLOR false

/* provided code - DO NOT CHANGE
 * printRoadData
 * input: int length, int from, int to, int greenStartTime, int greenEndTime, int lightCycleLength
 * output: prints the given road
 *
 * Call this function to print out the road data you read in 
 */
void printRoadData( int length, int from, int to, int greenStartTime, int greenEndTime, int lightCycleLength )
{
	int i;
	char state='R';
	
	printf("Created road from %d to %d with length %d (green=%d; red=%d; reset=%d).\n", from, to, length, greenStartTime, greenEndTime, lightCycleLength );
	printf("Cycle number: ");
	for( i=0; i<25; i++ ){
		printf("%2d ", i );
	}
	printf("...\nLight state : ");	
	if(PRINT_COLOR) 
		printf("\033[0;31m");//Color = RED
	for( i=0; i<25; i++ ){	/* print example light cycle: */
		if( i%lightCycleLength==greenStartTime ){
			if(PRINT_COLOR) 
				printf("\033[0;32m");//Color = GREEN
			state = 'G';
		}
		if( i%lightCycleLength==greenEndTime ){
			if(PRINT_COLOR) 
				printf("\033[0;31m");//Color = RED
			state = 'R';
		}
		printf("%2c ", state );
	}
	if(PRINT_COLOR) 
		printf("\033[0m");//Reset color
	printf("...\n\n");
}

/* trafficSimulator
 * input: TrafficData that will be simulated
 * output: N/A
 *
 * Simulate the road network in the given TrafficData variable
 */
void trafficSimulator( TrafficData* pTrafficData )
{
    int i = 0; int u = 0;
    int currentTime = 0;
    bool finish = false;
    bool sameTime = true;
    bool eventActivated = false;
    int* pnext = 0;
    /* TODO: complete this function */
    Event* now = dequeuePQ( pTrafficData->eventQueue );
    Car* buffer = NULL;
    Car* dissapear = NULL;
    
    while(now != NULL){
    
      while(sameTime == true|| finish == false){
        if(now->eventTimeStep == currentTime){
          
          eventActivated = true;
          if(now->eventCode == ADD_CAR_EVENT){

            buffer = dequeue(now->pCarQueue);
            enqueueCarToRoad(now->pRoadData, buffer);
            pTrafficData->numOfCars++;
            printCar(buffer, currentTime);
            
            }
          else if(now->eventCode == PRINT_ROADS_EVENT){
           printf("STEP %d - PRINT_ROADS_EVENT - Current contents of the roads:\n", currentTime);
            for(u = 0; u < pTrafficData->numOfRoads; u++){
              printRoad(pTrafficData->incrementingRoads[u], currentTime);   //this one
            }
            printf("\n\n");
          }
          
      }
      
        
      if(eventActivated == true){
        eventActivated = false;
        if(!(isEmptyPQ(pTrafficData->eventQueue))){
          freeEvent(now);
          now = dequeuePQ( pTrafficData->eventQueue );
          if(now->eventTimeStep > currentTime)
            sameTime = false;
        }
        else
          finish = true;
    }
    else{
      for(i = 0; i < pTrafficData->numOfRoads; i++){
        if(pTrafficData->incrementingRoads[i]->cars[0] != NULL && pTrafficData->incrementingRoads[i]->trafficLight == 0){
          
          if(pTrafficData->incrementingRoads[i]->cars[0]->next == pTrafficData->incrementingRoads[i]->cars[0]->destination){
            printCarDestinationReached(pTrafficData->incrementingRoads[i]->cars[0], currentTime);
            dissapear = pTrafficData->incrementingRoads[i]->cars[0];
            pTrafficData->incrementingRoads[i]->cars[0] = NULL;
            freeCar(dissapear);
          }
        }
        
        moveCarsFoward(pTrafficData->incrementingRoads[i]);
        }
        
      currentTime++;
      }
    }
    
    if(finish == true){
      freeEvent(now);
      now = NULL;
      }
      
      
      //next time tick
      
      sameTime = true;
      for(i = 0; i < pTrafficData->numOfRoads; i++){
      
        updateTrafficLights(pTrafficData->incrementingRoads[i], currentTime);
      }
      //printf("STEP = %d - PRINT-ROADS-EVENT - Current contents of the roads:\n");
    }
    /* Loop until all events processed and either all cars reached destination or gridlock has occurred */

    /* Update the state of every traffic light */

    /* Loop on events associated with this time step */

    /* First try to move cars through the next intersection */

    /* Second move waiting cars onto the end of each road if possible */

    /* Third move cars forward on every road (only those that haven't moved yet this time step) */
    
}

/* freeTrafficData
 * input: TrafficData* pTrafficData
 * output: N/A
 *
 * Free the data in the TrafficData variable
 */
void freeTrafficData( TrafficData* pTrafficData )
{
    int i; int u;
    /* TODO: complete this function */

    freeGraph(pTrafficData->roadSystem);
    for(i = 0; i < pTrafficData->numOfRoads; i++){
      for(u = 0; u < pTrafficData->incrementingRoads[i]->length; u++){
        if(pTrafficData->incrementingRoads[i]->cars[u] != NULL)
          freeCar(pTrafficData->incrementingRoads[i]->cars[u]);
      }
      freeRoad(pTrafficData->incrementingRoads[i]);
    }
    freePQ(pTrafficData->eventQueue);
    free(pTrafficData->incrementingRoads);
    free(pTrafficData);
    
    
}

int max( int a, int b )
{
    if(a>b)
        return a;
    return b;
}

