#include <stdlib.h>

#include "resourceManagement.h"

int const STRING_LENGTH = 31;

pqType createPQType( Department *d, double totalSpent );
double minDouble( double x, double y );

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
    printf("Peter Akinwunmi\n\n");
}

/* TODO:
 * ResourceManagement
 * input: char* fileNames[], double budget
 * output: none
 *
 * Simulates the algorithm from the pdf to determine what items are purchased
 * for the given budget and department item lists.
 * Print a summary of what each department received and did not receive.
 */
void ResourceManagement( char* fileNames[], int testDataSize, double budget )
{
    int i;
    PriorityQueue* quee = createPQ();
    Department* temp;
    char buffer[STRING_LENGTH];
    char stringTemp[STRING_LENGTH];
    FILE *f = NULL;
    pqType tempInput;
    double totalSpent = 0.0;
    double num = 0.0;
    double ogBudget = budget;
    Item* buff;
    Item* exe;
    Item* scholar;
    bool noItem = false;
    
    pqType priCheck;
    
    
    /* Create a department for each file listed in fileNames */
    for(i = 0; i < testDataSize; i++){
    
    
    /*open file*/
    f = fopen(fileNames[i], "r");
    
    if( f == NULL){
      printf("ERROR - Invalid file %s\n", fileNames[i]);
      exit(-1);
    }
    temp = (Department *)malloc( sizeof(Department) );
    temp->name = (char*)malloc(sizeof(char)*STRING_LENGTH);
    /*department making*/
    fgets(buffer, STRING_LENGTH, f);
    sscanf(buffer, "%s", &stringTemp);
    strcpy(temp->name, stringTemp);
    temp->totalSpent = 0;
    temp->itemsDesired = createQueue();
    temp->itemsReceived = createQueue();
    temp->itemsRemoved = createQueue();
    /*item queue input*/
    
    while(fgets(buffer, STRING_LENGTH, f) != NULL){
    buff = (Item *)malloc( sizeof(Item) );
    buff->name = (char*)malloc(sizeof(char)*STRING_LENGTH);
    if(fgets(buffer, STRING_LENGTH, f) != NULL)
      sscanf(buffer, "%s", &stringTemp);
    if(fgets(buffer, STRING_LENGTH, f)!= NULL)
      sscanf(buffer, "%lf", &num);
      
    strcpy(buff->name, stringTemp);
    buff->price = num;
    enqueue(temp->itemsDesired, buff);
    }
    tempInput = createPQType(temp,totalSpent);
    temp = tempInput.info;
    
    insertPQ(quee, tempInput);
    fclose(f);
    }
    printf("ITEMS PURCHASED\n----------------------------\n");
     /*Simulate the algorithm for picking the items to purchase */
      
    while(budget > 0){
      priCheck = removePQ(quee);
      temp = priCheck.info;
      if(isEmpty(temp->itemsDesired) == false)
        buff = dequeue(temp->itemsDesired);
      else
        noItem = true;
      if(noItem == true){
        noItem = false;
        scholar = (Item *)malloc( sizeof(Item) );
        scholar->name = (char*)malloc(sizeof(char)*STRING_LENGTH);
        strcpy(scholar->name, "Scholarship");
        scholar->price = 1000;
    
        if(budget >= 1000){
        enqueue(temp->itemsReceived, scholar);
        budget -= 1000;
        priCheck = createPQType(priCheck.info, priCheck.priority + 1000);
        insertPQ(quee, priCheck);
        }
        else{
          scholar->price = budget;
          enqueue(temp->itemsReceived, scholar);
          budget = 0;
          priCheck = createPQType(priCheck.info, priCheck.priority + scholar->price);
          insertPQ(quee, priCheck);
          }
        sprintf(stringTemp, "$%.2lf", buff->price);
        printf("Department of %-30s - %-30s - %20s\n", temp->name, scholar->name, stringTemp);
      }      
      else if(buff->price <= budget){
        enqueue(temp->itemsReceived, buff);
        sprintf(stringTemp, "$%.2lf", buff->price);
        printf("Department of %-30s - %-30s - %20s\n", temp->name, buff->name, stringTemp);
        budget -= buff->price;
        priCheck = createPQType(priCheck.info, priCheck.priority + buff->price);
        insertPQ(quee, priCheck);
      }
      else{
      enqueue(temp->itemsRemoved, buff);
      priCheck = createPQType(priCheck.info, priCheck.priority);
      insertPQ(quee, priCheck);
      }
      
    }
	/* Print the information for each department (including which items were received and which were not) */
 
  while(isEmptyPQ(quee) == false){
  num = 0.0;
  priCheck = removePQ(quee);
  temp = priCheck.info;
  while(isEmpty(temp->itemsDesired) == false){
    exe = dequeue(temp->itemsDesired);
    enqueue(temp->itemsRemoved, exe);
  }
  num = (priCheck.priority / ogBudget) * 100;
  
  printf("\nDepartment of %s\nTotal Spent       = $%.2lf\nPercent of Budget = %.2lf\n----------------------------\n", temp->name, priCheck.priority, num);
  printf("ITEMS RECEIVED\n");
  while(isEmpty(temp->itemsReceived) == false){
    exe = dequeue(temp->itemsReceived);
    sprintf(stringTemp, "$%.2lf", exe->price);
    printf("%-30s - %20s\n", exe->name, stringTemp);
    free(exe->name);
    free(exe);
  }
  printf("\nITEMS NOT RECEIVED\n");
  while(isEmpty(temp->itemsRemoved) == false){
    exe = dequeue(temp->itemsRemoved);
    sprintf(stringTemp, "$%.2lf", exe->price);
    printf("%-30s - %20s\n", exe->name, stringTemp);
    free(exe->name);
    free(exe);
  }
  printf("\n\n");
  freeQueue(temp->itemsReceived);
  freeQueue(temp->itemsRemoved);
  freeQueue(temp->itemsDesired);
  free(temp->name);
  free(temp);
  }
  freePQ(quee);
} 

/* createPQType
 * input: Department *d, double totalSpent
 * output: pqType
 *
 * Creates and returns a pqType struct (i.e the type in the priority queue)
 * from the given department and totalSpent. 
 */
pqType createPQType( Department *d, double totalSpent ){
	pqType pt;
	pt.info = d;
	pt.priority = totalSpent;

	return pt;
}

/* minDouble
 * input: double x, double y 
 * output: the smaller of x and y
 */
double minDouble( double x, double y ){
	if( x<=y )
		return x;
	return y;
}