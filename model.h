

#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> 


void print_job();
void time_the_process(bool exit_flag);
void add_jobs(char *analyze_command[], int count, pid_t pid);
void readthefile(char* fileInputName);
void execute_inputoutputpipe(char *analyze_command[], int count, char* fileInputName, char* fileOutputName, bool background);
void execute_inputpipe(char *analyze_command[], int count, char* fileInputName, bool background);
void execute_outputpipe(char *analyze_command[], int count, char *fileOutputName, bool background);
void execute_normal(char *analyze_command[], int count, bool background);

void execute_exit();

void execute_jobs();

void execute_kill(pid_t pid_kill);

void execute_resume(pid_t pid_resume);

void execute_sleep(int sleep_time);

void execute_suspend(pid_t pid_suspend);

void execute_wait(pid_t pid_wait);

void find_execution(char *analyze_command[], int count, bool fileInput, char *fileInputName, bool fileOutput, char *fileOutputName, bool background);

void evaluate_nonspecial(char *analyze_command[], int count);