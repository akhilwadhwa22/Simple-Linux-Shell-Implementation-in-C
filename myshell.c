#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <unistd.h>

#define CM_size 64 //Max size for the user input
#define PARSED_SIZE 12 //Max char size for individual command
#define DELIM " " //Delimiter for separating commands
#define NUMBUILT 5 //Number of builtin capabilities implemented
#define BUFSIZE 25 //Buffer size for storing history

char* Builtins[]={ "cd","help","history","clear","exit"}; //Implemented builtin functions 
char* buffer[BUFSIZE]; //Initialization for the history buffer

//********************Check for duplicates********************
bool searchDuplicate(char command[]){   
 	for(int i=0;i<BUFSIZE;i++){
		if(buffer[i]!=NULL){
			if(!strcmp(command,buffer[i])){
				return true;
			}
		}	
	}
	return false;
}

//********************Add commands to the history buffer********************
void add_history(char command[]){ 
	static int index=0;
	if(strlen(command)!=0){ //Make sure an empty command in not added in history
		if(!searchDuplicate(command)){ //Search for duplicate commands
			buffer[index]=strdup(command); //A circular buffer to store history
			index=(index+1)%BUFSIZE;
		}
	}
	else return;
}

//******************** Display history********************
void display_history(){ 
	int index=0;
	printf("Unique commands entered:\n");
	while(buffer[index]!=NULL){
		printf("%s\n",buffer[index]);
		index++;
	}
}

//********************Start up page of the shell********************
void shellCover(){ 	

	system("clear"); //Clear the output window
	printf("\n\n\n ****************************Welcome to MYSHELL**************************************");
	printf("\n           *******************PLAY AT YOUR RISK*******************");
	//**** Get Environment Variables ****
	printf(" \n\t\t\tCurrent User: %s\n",getenv("USER"));
	printf(" \n\t\t\tHome: %s\n",getenv("HOME"));
	printf(" \n\t\t\tPresent Directory: %s\n",getenv("PWD"));

	printf("\n ************************************************************************************\n\n");
	sleep(2); //Sleep for 2 seconds
	system("clear"); //Clear the output window
}

//********************Read the user input********************
char *readCommand(){

	int index=0; // To keep track of the index in the char array
	int size=0; //Size of the input enetered 
	char ch; 
	char* command= (char*)malloc(sizeof(char)*CM_size); //char array to store the entered string 

	while(1){

		ch=getchar(); //Read the next character entered
		if(ch=='\n'){ //If user pressed the enter key
			command[index]='\0'; //Insert a NULL character at the end of the string.
			add_history(command); //add the command to history
			break;
		}
		else if(ch==EOF) 	//If user pressed ctrl + c
			EXIT_SUCCESS;
		else 				//Fill in the string with characters
			command[index]=ch;
		//Increments for size and index in each iteration
		size++;	
		index++;

		if(size>=CM_size) //if size is more than defined CM_size, reallocate with the new size
 			command= realloc(command,size++);
	}
	return command;
}

//********************Help Function********************
void helper(){

	puts("\n***********************Welcome to the SHELL HELPER***********************" 
		"\n\n\n To execute, type command and arguments, and hit enter.\n"
		"Builtin commands:\n"
		"cd\n"
		"help\n"
		"history\n"
		"clear\n"
		"exit\n\n\n\n"
		"Use the man command for information on other programs.\n"
		"**************************************************************************");
}

//********************Selecting the builtin function********************
bool myBuiltIns(char **parsed,int index){
	switch(index){
		case 0: //cd function
			if(parsed[1]==NULL)
				fprintf(stderr,"Missing arguments for cd");
				chdir(parsed[1]);
				return true;
		case 1: //help function
			helper();
			return true;
		case 2: //history function
			display_history();
			return true;
		case 3: //clear screen function
			system("clear");
			return true;	
		case 4: //Exit
			printf("Exiting Shell! Thank you!\n\n");
			exit(0);
		default:
			break;			
	}

	return 0;
}

//********************Processing the user input********************
bool processed(char** parsed){
	int i=0;
	pid_t pid;
  	int status;
  	int index=0;

	if(parsed[0]==NULL) //If no input from user
		return true; 

	for(int i=0;i<NUMBUILT;i++){
		if(strcmp(parsed[0],Builtins[i])==0){ //If commands present as builtins
			index=i;
			return myBuiltIns(parsed, index);
		}
	}

	pid = fork(); //Fork a child process

	if (pid < 0) { //Check if fork successful
  		printf("Fork Failed");
 	}

  	if (pid == 0) { //Child process
  		if(execvp(parsed[0],parsed)==-1) //replace the current process image with a new process
//The first argument is the name of the command
//The second argument consists of the name of the command and the arguments passed to the command itself. 
//It must also be terminated by NULL.
  			perror("Shell Error");

  		exit(EXIT_FAILURE);
    }

	else {// Parent process
		
		do
			waitpid(pid, &status, WUNTRACED); //Wait for the child to finish execution
		while (!WIFEXITED(status)); //Checks if child finishes successfully
	}

	return true;
}

//********************Parsing the user input********************
char **parseCommand(char* command){
	int index=0;
	int size=0;
	char** parsed=(char**)malloc(sizeof(char*)*PARSED_SIZE);
	char* str;

	str=strtok(command,DELIM); //Parse the first command
	parsed[index]=str;
	index++;

	while(str!=NULL){
		str=strtok(NULL,DELIM); //Store the consecutive strings
		parsed[index]=str;
		index++;
		size++;

		if(size>=PARSED_SIZE) //If size is greater than the limit, allocate more memory as and when required
			parsed=realloc(parsed,PARSED_SIZE * sizeof(char*));
	}
	parsed[index++]="\0";
	return parsed;
}

//********************Main function********************
int main(){
	char *command;
	char **parsed;
	bool status;

	shellCover(); //Intialize shell
	do{
		printf("\n>> ");
		command= readCommand(); //read input 

		parsed=parseCommand(command); //parse the commands
		status=processed(parsed); //process the commands
	
	} while(status); //Take in infinite inputs unless EOF or exit is encountered

	//Free allocated Memory
	free(command);
	free(parsed);
	return EXIT_SUCCESS;
}