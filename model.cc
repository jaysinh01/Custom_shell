
#include <unistd.h>
#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> 
#include <sys/times.h>
#include <vector>
#include <chrono>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include "model.h"

struct jobs_description{
	pid_t pid;
	char state;
    int count;
	clock_t time_process;
	char* job_command[7];
};

std::vector<jobs_description> jobs_list;


void print_job(){
	//check if the process is still going on. If it is then find the end time and subtract
	// it with the time_process.
	//clock_t end = clock();
    struct tms team;
    clock_t end = times(&team);
    int tics_per_second;
    tics_per_second = sysconf(_SC_CLK_TCK);
	printf("Running processes:\n");
	printf("  #     PID  S  SEC  COMMAND\n");
	int process_number = 0;
    printf("The size: %d\n", (int)jobs_list.size());
    	for (auto current_job = std::begin(jobs_list); current_job != std::end(jobs_list); ++current_job){
    		int status;
    		pid_t ChildPID = current_job->pid;
    		pid_t result = waitpid(ChildPID, &status, WNOHANG);
    		if (result == 0) {
    		  // Child still alive
                
                clock_t total_t = ((double)(end - current_job->time_process))/tics_per_second;
               
    			printf("  %d   %d    ", process_number, ChildPID);
                printf("%c  %lu    \n", current_job->state,
                    total_t);
    			fflush(stdout);
                
    			for(int index = 0; index < current_job->count; index++){
    				printf("%s ", current_job->job_command[index]);
    			}
                fflush(stdout);
    			process_number++;
                
    		} else if (result == -1) {
    		  // Error 
                printf("There has been error in the PID line 65\n");
    		} else {
    		  // Child exited
                printf("I got here 51 \n");
    			jobs_list.erase(current_job);
                --current_job;
    		}
    	}
    
    printf("gets here 55\n");
	time_the_process(false);

}

void time_the_process(bool exit_flag){
	struct tms t;
	clock_t dub;

  	int tics_per_second;
    tics_per_second = sysconf(_SC_CLK_TCK);

  	if ((dub = times(&t)) == -1){
    	perror("times() error");
  	}
  	else if (!exit_flag){
    	printf("Completed Processes\n");
    	//printf("process was dubbed %f seconds ago.\n\n",
        //   ((double) dub)/tics_per_second);
    	//printf("            utime           stime\n");
    	/*printf("parent:    %f        %f\n",
           ((double) t.tms_utime)/tics_per_second,
           ((double) t.tms_stime)/tics_per_second);
    	printf("child:     %f        %f\n",
           ((double) t.tms_cutime)/tics_per_second,
           ((double) t.tms_cstime)/tics_per_second);*/
	} else{
		printf("Resources Used\n");
	}
	printf("User time =     %f\n", ((double) t.tms_utime)/tics_per_second);
    printf("System time =     %f\n", ((double) t.tms_stime)/tics_per_second);

}

void add_jobs(char *analyze_command[], int count, pid_t pid){
	//https://stackoverflow.com/questions/22387586/measuring-execution-time-of-a-function-in-c
	//clock_t start = clock();
    
    struct tms nice;
    clock_t start = times(&nice);
	jobs_description job;
	job.time_process = start;
	job.pid = pid;
    job.count = count;
	job.state = 'R';
	//seems probalamatic
	for (int i=0; i<count; i++){
		job.job_command[i] = analyze_command[i];
	}
	jobs_list.push_back(job);
    

}

void readthefile(char* fileInputName){
    FILE *fptr; 
  
    char c; 
  
  
    // Open file 
    fptr = fopen(fileInputName, "r"); 
    if (fptr == NULL) 
    { 
        printf("Cannot open file \n"); 
        exit(0); 
    } 
  
    // Read contents from file 
    c = fgetc(fptr); 
    while (c != EOF) 
    { 
        printf ("%c", c); 
        c = fgetc(fptr); 
    } 
  
    fclose(fptr); 

}

void execute_inputoutputpipe(char *analyze_command[], int count, char* fileInputName, char* fileOutputName, bool background){
	printf("I'm in execute_inputoutputpipe\n");
	 int des_p[2];
        int this_p[2];
        if(pipe(des_p) == -1) {
          perror("Pipe failed");
          exit(1);
        }

        if(fork() == 0)            //first fork
        {
            close(STDOUT_FILENO);  //closing stdout
            dup(des_p[1]);         //replacing stdout with pipe write 
            close(des_p[0]);       //closing pipe read
            close(des_p[1]);

            //read the input file
            readthefile(fileInputName);
            //perror("execvp of ls failed");
            exit(1);
        }
        pid_t pid = fork();
        printf("%d\n", pid);
        if(pid == 0)            //creating 2nd child
        {

            close(STDIN_FILENO);   //closing stdin
            dup(des_p[0]);         //replacing stdin with pipe read
            close(des_p[1]);       //closing pipe write
            close(des_p[0]);

            //write to file instead of the stdoutput
            freopen (fileOutputName,"w",stdout);

            execvp(analyze_command[0], analyze_command);
            perror("execvp of wc failed");
            exit(1);

            
            fclose (stdout);
            
        }

        close(des_p[0]);
        close(des_p[1]);
        if (!background){
        	wait(0);
        	wait(0);
    	}else{
    		add_jobs(analyze_command, count, pid);
    	}
}
	
void execute_inputpipe(char *analyze_command[], int count, char* fileInputName, bool background){
	int des_p[2];
    int this_p[2];
    if(pipe(des_p) == -1) {
      perror("Pipe failed");
      exit(1);
    }

    if(fork() == 0)            //first fork
    {
        close(STDOUT_FILENO);  //closing stdout
        dup(des_p[1]);         //replacing stdout with pipe write 
        close(des_p[0]);       //closing pipe read
        close(des_p[1]);

        //read the input file
        readthefile(fileInputName);
        //perror("execvp of ls failed");
        exit(1);
    }
    pid_t pid = fork();
    printf("%d\n", pid);
    if(pid == 0)            //creating 2nd child
    {
    	
        close(STDIN_FILENO);   //closing stdin
        dup(des_p[0]);         //replacing stdin with pipe read
        close(des_p[1]);       //closing pipe write
        close(des_p[0]);

        //write to file instead of the stdoutput
        //freopen (fileOutputName,"w",stdout);
        execvp(analyze_command[0], analyze_command);

        perror("execvp of wc failed");
        
        exit(1);

    }

    close(des_p[0]);
    close(des_p[1]);
    //sleep(10);
    if (!background){
    	wait(0);
    	wait(0);
	}else{
        wait(0);
		add_jobs(analyze_command, count, pid);
	}
}

void execute_outputpipe(char *analyze_command[], int count, char *fileOutputName, bool background){
    pid_t pid = fork();
    printf("%d\n", pid);
    if(pid == 0){
        //write to file instead of the stdoutput
        freopen (fileOutputName,"w",stdout);
        execvp(analyze_command[0], analyze_command);
        perror("execvp of wc failed");
        exit(1);
        fclose (stdout);  
    }
    if (!background){
    	wait(0);
	}else{
		add_jobs(analyze_command, count, pid);
	}
}

void execute_normal(char *analyze_command[], int count, bool background){
    pid_t pid = fork();
    printf("%d\n", pid);
    if(pid == 0){
        //write to file instead of the stdoutput
        //freopen (fileOutputName,"w",stdout);
        printf("281111111\n");
        execvp(analyze_command[0], analyze_command);
        perror("execvp failed (line 157)");
        //exit(1);
        //fclose (stdout);  
    }
    if (!background){
    	wait(0);
	}else{
		add_jobs(analyze_command, count, pid);
	}

}



void execute_exit(){
	//
	for (auto current_job = std::begin(jobs_list); current_job != std::end(jobs_list); ++current_job){
		waitpid(current_job->pid, NULL, 0);
	}
	time_the_process(true);
}



void execute_jobs(){
	print_job();
}

void execute_kill(pid_t pid_kill){
	kill(pid_kill, SIGKILL);
    perror("Kill msssage: ");
}

void execute_resume(pid_t pid_resume){
    printf("pid_resume: %d\n", pid_resume);
	kill(pid_resume, SIGCONT);
	perror("Resume message: ");
}

void execute_sleep(int sleep_time){
	sleep(sleep_time);
}

void execute_suspend(pid_t pid_suspend){
	kill(pid_suspend, SIGSTOP);
    perror("Suspend message: ");
	for(auto current_job = std::begin(jobs_list); current_job != std::end(jobs_list); ++current_job){
		if (current_job->pid == pid_suspend){
			current_job->state = 'S';
		}
	}

}

void execute_wait(pid_t pid_wait){
	waitpid(pid_wait, NULL, 0);
}

void find_execution(char *analyze_command[], int count, bool fileInput, char *fileInputName, bool fileOutput, char *fileOutputName, bool background){
	printf("im in the find_execution fen\n");
	printf("%d\n", fileInput);
	if (fileInput && fileOutput){
		execute_inputoutputpipe(analyze_command, count, fileInputName, fileOutputName, background);
	}else if (fileInput){
		execute_inputpipe(analyze_command, count, fileInputName, background);
	}else if (fileOutput){
		execute_outputpipe(analyze_command, count, fileOutputName, background);
	}else{
		execute_normal(analyze_command, count, background);
	}
}

void evaluate_nonspecial(char *analyze_command[], int count){
	// you first want to check for <> and &
	bool fileInput = false;
	char *fileInputName;
	char *fileOutputName; 
	bool fileOutput = false;
	bool background = false;
	//problem in input errors
	for (int i = 0; i < count; i++){
		if (strncmp(analyze_command[i], "<", 1) == 0){
			printf("this should be printed\n");
			fileInput = true;
			fileInputName = analyze_command[i];
			fileInputName = strtok(fileInputName, "<");
		}else if (strncmp(analyze_command[i], ">", 1) == 0){
			fileOutput = true;
			fileOutputName = analyze_command[i];
			fileOutputName = strtok(fileOutputName, ">");
		}else if (strncmp(analyze_command[i], "&", 1) == 0){
			background = true;
		}
	}
	if (!fileInput){ fileInputName = NULL; }
	if (!fileOutput){ fileOutputName = NULL;}

	find_execution(analyze_command, count, fileInput, fileInputName, fileOutput, fileOutputName, background);
}








