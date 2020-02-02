
#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <sys/times.h>
#include <sys/resource.h>
#include "model.h"

void showpromt(){
	printf("type here: ");
}

void print_command(char* char_array[], int count){
	for(int i=0; i<count; i++){
		printf("%s\n", char_array[i]);
		fflush(stdout);
	}
	//showpromt();
}


int processthis(char* line, char* command[]){
	//
	int length = (int)strlen(line);
	//printf("im here at 19\n");
	char *token;
	char *pch;
	//char copy_line = *line;

	//entering gives error
	 pch = strtok (line," ");
	 int i = 0;
	 printf("this is pch when one %s\n", pch);
	  while (pch != NULL)
	  {
	  	//printf("this is pch when one %s\n", pch);
	    command[i] = pch;
	    i++;
	    pch = strtok(NULL, " ");
	    
	  }
	  //adding null last
	  command[i] = NULL;
	print_command(command, i);
	return i;
}

bool indentify_command(char *analyze_command[], int count){
	//find the length for error
	//see if it is predefined
	if (strcmp(analyze_command[0], "exit") == 0){
		execute_exit();
		return true;
	}else if(strcmp(analyze_command[0], "jobs") == 0){
		execute_jobs();
	}else if(strcmp(analyze_command[0], "kill") == 0){
		pid_t pid_kill = atoi(analyze_command[1]);
		execute_kill(pid_kill);
	}else if(strcmp(analyze_command[0], "resume") == 0){
		pid_t pid_resume = atoi(analyze_command[1]);
		printf("%d\n", pid_resume);
		execute_resume(pid_resume);
	}else if(strcmp(analyze_command[0], "sleep") == 0){
		int sleep_time = atoi(analyze_command[1]);
		execute_sleep(sleep_time);
	}else if(strcmp(analyze_command[0], "suspend") == 0){
		pid_t pid_suspend = atoi(analyze_command[1]);
		execute_suspend(pid_suspend);	
	}else if(strcmp(analyze_command[0], "wait") == 0){
		pid_t pid_wait = atoi(analyze_command[1]);
		execute_wait(pid_wait);
	}else{
		evaluate_nonspecial(analyze_command, count);
	}
	return false;
}



int main(int argc, char** argv)
{		
	showpromt();
	fflush(stdout);
	//struct rusage usage;
  	//struct timeval start, end;
  	//getrusage(RUSAGE_SELF, &usage);
  

    while (1){

    	/*
    	static clock_t st_time;
		static clock_t en_time;
		static struct tms st_cpu;
		static struct tms en_cpu;
    	st_time = times(&st_cpu);*/
    		//issue if there is not space rthen it will not be printed	
    	//start = usage.ru_stime;

    	char* command[7];
    	char* commandLine;
    	//does this cause issue with exec?
    	commandLine = (char *)malloc(100 * sizeof(char));
    	//printf("im here at 43\n");
    	fgets(commandLine, 100, stdin);
    	//getting rid of the \n delimeter
    	commandLine = strtok(commandLine, "\n");
    	//printf("im here at 45 %c\n", commandLine[20]);
    	int count = processthis(commandLine, command);
    	bool fexit = indentify_command(command, count);

    	if (fexit){break;}

    	//getrusage(RUSAGE_SELF, &usage);
  		//end = usage.ru_stime;

  		//printf("Started at: %d.%d\n", start.tv_sec, start.tv_usec);
  		//printf("Ended at: %d.%d\n", end.tv_sec, end.tv_usec);
    	/*en_time = times(&en_cpu);
        printf("Real Time: %jd, User Time %jd, System Time %jd\n",
        (intmax_t)(en_time - st_time),
        (intmax_t)(en_cpu.tms_utime - st_cpu.tms_utime),
        (intmax_t)(en_cpu.tms_stime - st_cpu.tms_stime));*/
    	showpromt();

    }
       
    return 0;
}
