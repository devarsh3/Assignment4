/*
Authors:
Devarsh Patwa - 190417940
Mahek Shah - 190734000
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <pthread.h>
#include <semaphore.h>

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

sem_t sem_even; 
sem_t sem_odd;

//declare methods
int read(char *file);
void currentResources();

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

sem_init(&sem_even, 0, 0);
sem_init(&sem_odd, 0 ,0);

int availableSize = argc - 1;

//get the no. of cutomers from file
int numberOfCustomers = read("sample4_in.txt") + 1;
printf("Number of Customers: %d\n", numberOfCustomers);

//array initiallization
customerTemp = (Customer*)malloc(sizeof(Customer));
customerTemp->record_size = availableSize;
customerTemp->customers = numberOfCustomers;

available = (int*)malloc(availableSize * sizeof(int));
if (available == NULL){
    printf("MALLOC ERROR\n");
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

for(i = 0; i< numberOfCustomers;i++){
    needed[i] = (int *)malloc(availableSize * sizeof(int));
}

int tempArr[numberOfCustomers];

int maximum[numberOfCustomers][availableSize];

//initialization to 0
for(int j=0; j<numberOfCustomers;j++){
    for(int x=0; x<availableSize;x++){
        maximum[j][x] = 0;
    }
}

FILE *input = fopen("sample4_in.txt", "r");

if(!input){
    printf("FILE ERROR");
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

for(int k = 0; k < customers; k++)
{
    char* token = NULL;
    int x = 0;
    token =  strtok(lines[k],",");

    while(token!=NULL)
    {
        maximum[k][x] = atoi(token);
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
char command[MAXINP];
char request[MAXINP] = "RQ"; 
char release[MAXINP] = "RL";
char newLine[MAXINP] = "\n";
char run[MAXINP] = "run\n";
char exitStr[MAXINP] = "exit\n";


//ask for command
printf("Enter Command: ");
fgets(str, MAXINP, stdin);
strcpy(command, str);
int work = 0;

while(strcmp(command,exitStr)!=0){

    if(work>0){
        printf("Enter Command: ");
        fgets(str, sizeof str, stdin);
        strcpy(command, str);
    }

    //split
    int total = 0;
    for(int x = 0; command[x]!='\0';x++){
        if(command[x] == ' ' || command[x] == '\n' || command[x] == '\t')
            total++;
    }

    char *tokn = strtok(command, " ");
	char *inputStr[MAXINP];

    i = 0;
    if(total>=2){
        while (tokn != NULL && i <= numberOfCustomers){
            inputStr[i] = tokn;
			tokn = strtok(NULL, " ");
			i += 1;
        }
    }
    else{
        strcpy(inputStr[0], command);
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
                allocated[atoi(input_str[1])][m-2] = atoi(inputStr[m]); // Convert inputted allocated resource information from string to integer

            printf("Request is satisfied\n");
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
                    printf("Release is satisfied\n");
            }

            if (quit == 1)
                continue;
        }
    }

    //Print Details=======================

    else if (strcmp(inputStr[0], newLine) == 0){
        printf("Showing current state of arrays:\n\n");
        printf("Currently Available Resources: ");
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

        // display Alloc.Resources
        printf("Allocated Resources: \n");

        for (i = 0; i < numberOfCustomers; i++) {
            for (int x = 0; x < availableSize; x++)
                printf("%d ", allocated[i][x]);
            
            printf("\n");
        }
        printf("\n");


        // Print Needed Resources
        printf("Needed Resources: \n");
        
        needResources(availableSize, numberOfCustomers, allocated, maximum, need);

        for (i = 0; i < numberOfCustomers; i++) {
            for (int x = 0; x < availableSize; x++)
                printf("%d ", need[i][x]);

            printf("\n");
        }
        printf("\n");

    }

}//end while



} //end main


/*
----------------------------------------------------------------
Method to count the number of customers from file
----------------------------------------------------------------
*/

int read(char *file){
    FILE *fileptr;
    int lines = 0;
    char x;
 
    fileptr = fopen(file, "r");

    x = getc(fileptr); 
    
	while (x != EOF)
    {
        if (x == '\n')
            lines++;

        x = getc(fileptr);
    }

    fclose(fileptr); // Close file

    return lines;
}

void currentResources(){
    int temp1;
	int temp2;

	for (int m = 0; m < customerTemp->record_size; m++) {
		temp1 = 0;

		for (int n = 0; n < customerTemp->customers; n++)
			temp1 = temp1 + allocated[n][m];
		
		temp2 = available[m] - temp1;
		available[m] = temp2;
	}
}

void needResources(int size, int customers, int **allocated, int maximum[j][i], int **needed) {

	for (int j = 0; j < customers; j++) {
		for (int x = 0; x < size; x++)
			needed[j][x] = maximum[j][x] - allocated[j][x];
	}
}