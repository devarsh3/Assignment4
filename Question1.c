/*
Authors:
Devarsh Patwa - 190417940
Mahek Shah - 190734000

Github:
Devarsh Patwa - devarsh3
Mahek SHah - mahekshah28
Repository - https://github.com/devarsh3/Assignment4
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <pthread.h>


//customer structure to keep things organized
typedef struct customer {
	int customers;
	int record_size;
} Customer;

int *available;
int **allocated;
int **need;

Customer *customerTemp;

#define MAXINP 200



//declare methods
int readFile(char *file);
void currentResources();
int bankerAlgo(int p, int q, int **allocated, int maximum[q][p], int availablem[p], int **need, int array[q]);
void needResources(int size, int customers, int **allocated, int maximum[customers][size], int **needed);
void *runThread(void *t);

/*
------------------------------------------------------------
Main Method
------------------------------------------------------------
*/
int main(int argc, char *argv[]){

if(argc<2)
	{
		printf("ERROR: Resources Data Missing\n");
		return -1;
	}


int availableSize = argc - 1;

//get the no. of cutomers from file
int numberOfCustomers = readFile("sample4_in.txt") + 1;
printf("Number of Customers: %d\n", numberOfCustomers);

//array initiallization
customerTemp = (Customer*)malloc(sizeof(Customer));
customerTemp->record_size = availableSize;
customerTemp->customers = numberOfCustomers;

//allocation of  2d array
available = (int*)malloc(availableSize * sizeof(int));
	
// conditional statement if == to Null THEN	
if (available == NULL){
	// formatted print message 
    printf("MALLOC ERROR\n");
	//returns-1
    return -1;
}

int i; //for loop variable

//argv values in array
for(i = 0; i<availableSize;i++)
    available[i] = atoi(argv[i+1]);

//allocated 2d array
allocated = (int **)malloc(numberOfCustomers * sizeof(int *));

for(i=0;i<numberOfCustomers;i++)
    allocated[i] = (int *)malloc(availableSize * sizeof(int));

//initialization to 0
for(int j=0; j<numberOfCustomers;j++){
    for(int x=0; x<availableSize;x++){
        allocated[j][x] = 0;
    }
}

//need 2d array
need = (int **)malloc(numberOfCustomers * sizeof(int *));

//for loop to iterate through array 
for(i = 0; i< numberOfCustomers;i++){
    need[i] = (int *)malloc(availableSize * sizeof(int));
}

//initializing array 
int tempArr[numberOfCustomers];

int maximum[numberOfCustomers][availableSize];

//initialization to 0
for(int j=0; j<numberOfCustomers;j++){
    for(int x=0; x<availableSize;x++){
        maximum[j][x] = 0;
    }
}

//open file to read contents from txr 
FILE *input = fopen("sample4_in.txt", "r");

//conditional statement doesnt equal to it 
if(!input){
	// formatted print 
    printf("FILE ERROR");
	//returns -1
    return -1;
}


struct stat stt;
fstat(fileno(input), &stt);
char* content = (char*)malloc(((int) stt.st_size + 1)* sizeof(char));
content[0]='\0';

while(!feof(input))
{
    char line[100];
    if(fgets(line, 100, input) != NULL)
        strncat(content, line, strlen(line));
}

fclose(input); //close the file


char* lines[numberOfCustomers];
char *command = NULL;
i = 0;
command = strtok(content,"\r\n");
//content to array
while(command!=NULL)
{
    lines[i] = malloc(sizeof(command)*sizeof(char));
    strcpy(lines[i],command);
    i++;
    command = strtok(NULL,"\r\n");
}
// for loop for iteration 
for(int k = 0; k < numberOfCustomers; k++)
{
    //initiaizng varables setting them equal to null and - 
    char* token = NULL;
    int x = 0;
    token =  strtok(lines[k],",");

    //while iterates as long as token is not empty 
    while(token!=NULL)
    {
        maximum[k][x] = atoi(token);
	//increment 
        x++;
        token = strtok(NULL,",");
    }
}

//print available Res
printf("Currently Available resources:");
for(i=0; i<availableSize;i++)
    printf(" %d", available[i]);
printf("\n");

//print Maximum Res
for(i=0; i<numberOfCustomers;i++){
    for(int k = 0;k<availableSize;k++){
        
        printf("%d", maximum[i][k]);
        if(k!= availableSize-1)
            printf(",");
    }
    printf("\n");
}


//command Strings
char str[MAXINP];
char comm[MAXINP];
char request[MAXINP] = "RQ"; 
char release[MAXINP] = "RL";
char status[MAXINP] = "Status\n";
char run[MAXINP] = "Run\n";
char exitStr[MAXINP] = "exit\n";


//ask for command
printf("Enter Command: ");
fgets(str, MAXINP, stdin);
strcpy(comm, str);
	
//initializinf variable work = to0 
int work = 0;

//while loop checking parameter doesnt = to 0 
while(strcmp(comm,exitStr)!=0){

    // conditional statement checking work >0 
    if(work>0){
	// prints if work >0
        printf("Enter Command: ");
        fgets(str, sizeof str, stdin);
        strcpy(comm, str);
    }

    //split
    int total = 0;
    //for loop to iterate through 
    for(int x = 0; comm[x]!='\0';x++){
	// checking if com has certain conditiosn in them 
        if(comm[x] == ' ' || comm[x] == '\n' || comm[x] == '\t')
	    //increment 
            total++;
    }

    char *tokn = strtok(comm, " ");
	char *inputStr[MAXINP];

    // intialize and set amount eqaul to 
    i = 0;
    
    // conditional statement checking total is always great or = to 2 to pass the next step 
    if(total>=2){
        while (tokn != NULL && i <= numberOfCustomers){
	   //if input at index i = to req tokn
            inputStr[i] = tokn;
			tokn = strtok(NULL, " ");
			//increment i at end next index
			i += 1;
        }
    }
    else{
        strcpy(inputStr[0], comm);
    }

    int length = i;
    i=0;



    //REQUEST RESOURCE========================

    if (strcmp(inputStr[0], request) == 0) {
        if (atoi (inputStr[1]) >= numberOfCustomers) { // Idiot Proofing the RQ command
            printf("Allocated index cant be bigger then Maximum No. of Customers\n");
        }

        else {
            for (int m = 2; m < (length); m++) // Store requested resource data
                allocated[atoi(inputStr[1])][m-2] = atoi(inputStr[m]); // Convert inputted allocated resource information from string to integer

            printf("State is safe, and request is satisfied\n");
        }
    }

    //RELEASE RESOURCE====================

    else if (strcmp(inputStr[0], release) == 0){

        int quit; // is Requested Release is unsatisfied

        if (atoi (inputStr[1]) >= numberOfCustomers) { // Idiot Proofing the RQ command
            printf("Allocated index cant be bigger then Maximum No. of Customers\n");
        }
        else{

            for (int m = 2; m < (length); m++)
            {
                int releaseValue;
                releaseValue = allocated[atoi(inputStr[1])][m - 2] - atoi(inputStr[m]); //calculate release value

                if (releaseValue < 0) { 
                    printf("Release is unsatisfied\n");
                    quit= 1; 
                    break;
                }
                
                else
                    allocated[atoi(inputStr[1])][m - 2] = releaseValue;

                if (m = length - 1)
                    printf("The resources have been released successfully\n");
            }

            if (quit == 1)
                continue;
        }
    }

    //Print Details=======================

    else if (strcmp(inputStr[0], status) == 0){

        printf("Available Resources: ");
        currentResources();

        for (i = 0; i < availableSize; i++)
            printf("%d ", available[i]);
        printf("\n\n");

        //max res
        printf("Maximum Resources: \n");
			
        for (i = 0; i < numberOfCustomers; i++) {
            for (int x = 0; x < availableSize; x++)
                printf("%d ", maximum[i][x]);

            printf("\n");
        }
        printf("\n");

        // display Alloc.Resources=========================
        printf("Allocated Resources: \n");

        for (i = 0; i < numberOfCustomers; i++) {
            for (int x = 0; x < availableSize; x++)
                printf("%d ", allocated[i][x]);
            
            printf("\n");
        }
        printf("\n");


        //display Need Resources========================
        printf("Needed Resources: \n");
        
        needResources(availableSize, numberOfCustomers, allocated, maximum, need);

        for (i = 0; i < numberOfCustomers; i++) {
            for (int x = 0; x < availableSize; x++)
                printf("%d ", need[i][x]);

            printf("\n");
        }
        printf("\n");

    }


    //Run command===================
    else if (strcmp(inputStr[0], run) == 0) {
        
        needResources(availableSize, numberOfCustomers, allocated, maximum, need);
        int res = bankerAlgo(availableSize, numberOfCustomers, allocated, maximum, available, need, tempArr);
        
        if (res != 0) {
            printf("Safe sequence not found... :(\n");
            return -1;
        }

        for (int x = 0; x < numberOfCustomers; x++) {
            int index = tempArr[x];
            printf("-->\tCustomer/Thread %d\n", index);
            
            // Running the thread
            pthread_t tid;
            pthread_create(&tid, NULL, runThread, &index);
            pthread_join(tid, NULL);
        }
    }
    
    work += 1;

}//end while

//free
free(available);
free(allocated);
free(need);
 
return 0;


} //end main




int bankerAlgo(int p, int q, int **allocated, int maximum[q][p], int availablem[p], int **need, int array[q]){
    int i, j, k;
    int safety = 1;

    int f[q], ind = 0;
    for (i = 0; i < q; i++)
        f[i] = 0;

    int y = 0;
    for (k = 0; k < p; k++)
    {
        for (i = 0; i < q; i++)
        {
            if (f[i] == 0)
            {

                int flag = 0;
                for (j = 0; j < p; j++)
                {
                    if (need[i][j] > available[j])
                    {
                        flag = 1;
                        break;
                    }
                }

                if (flag == 0)
                {
                    array[ind++] = i;

                    for (y = 0; y < p; y++)
                        available[y] += allocated[i][y];

                    f[i] = 1;
                }
            }
        }
    }

    for (k = 0; k < q; k++)
    {
        if (f[k] == 0)
            safety = 0;
    }

    if (safety == 0)
        return -1;

    printf("Safe Sequence is: <");

    for (k = 0; k < q - 1; k++)
        printf("%d ", array[k]);

    printf("%d>\n", array[q - 1]);

    return 0;
}

/*
----------------------------------------------------------------
Method to count the number of customers from file
----------------------------------------------------------------
*/

//read file function 
int readFile(char *file){
    //connecting file 
    FILE *fileptr;
    // initalizing variable = to 0 
    int lines = 0;
    char x;
 
    // open file to read 
    fileptr = fopen(file, "r");

    //get data from file 
    x = getc(fileptr); 
    
    // while loop iteartes onlywhen it doenst = to parameter
	while (x != EOF)
    {
	// if statemetn check if its empty/ white space incremet if it is 
        if (x == '\n')
            lines++;

        x = getc(fileptr);
    }

    fclose(fileptr); // Close file

// returns lines 
    return lines;
}

void currentResources(){
    //intializing variables 
    int temp1;
	//intializing variables 
	int temp2;

	//for loop to iterate setting a range for m 
	for (int m = 0; m < customerTemp->record_size; m++) {
		//var is = 0 
		temp1 = 0;
		
		//for loop to iterate setting a range for n
		for (int n = 0; n < customerTemp->customers; n++)
			//array allocations and n and m indexes and is incremented
			temp1 = temp1 + allocated[n][m];
		
		//array allocations and n and m indexes and is decremented one less of it 
		temp2 = available[m] - temp1;
		available[m] = temp2;
	}
}

void needResources(int size, int customers, int **allocated, int maximum[customers][size], int **needed) {
	//for loop to iterate setting a range for j
	for (int j = 0; j < customers; j++) {
		//for loop to iterate setting a range for x
		// double for loop 
		for (int x = 0; x < size; x++)
			//array allocation at indexes j and x and setting req aray equal to it 
			needed[j][x] = maximum[j][x] - allocated[j][x];
	}
}


void *runThread(void *t){

    // initializing 
    int *person = (int *)t;

    //print statement indent tab
    printf("\tAllocated Resources:\t");
    
    //for loop to iterate through with range for x 
    for(int x = 0; x<customerTemp->record_size; x++)
	//prints the array 
        printf("%d ", allocated[*person][x]);
    //print new line 
    printf("\n");

    //print statement indent tab
    printf("\tNeeded:\t");
    //for loop to iterate through with range for x 
    for (int x=0; x< customerTemp->record_size; x++){
	//prints the array 
        printf("%d ", need[*person][x]);
    }
    //print new line 
    printf("\n");

    //print statement indent tab
    printf("\tAvailable:\t");
    //for loop to iterate through with range for x 
    for (int x=0; x< customerTemp->record_size; x++){
	//prints the array 
        printf("%d ", available[x]);
    }
    //print new line 
    printf("\n");

   //required print statemnts end with a new line 
    printf("\tThread has started\n");
    printf("\tThread has finished\n");
    printf("\tThread is realeasing resources\n");
    

	printf("\tNew Available:\t");

    // for loop to iterate through the array range of x 
    for (int x=0; x< customerTemp->record_size; x++){
	//array indeexes and allocation of locations and setting equal to it 
        available[x] =  available[x] + allocated[*person][x];
	//prints 
        printf("%d ", available[x]);
    }

    //prints new line 
    printf("\n");
	
	//returns null 
	return NULL;

}
