#include <stdio.h>
#include <iostream>
#include <string>
#include <unistd.h>
int main(int argc, char* argv[]){
	std::string test = "";
	getline(std::cin,test);
	//printf("im inside me class\n");
	sleep(10);
	printf("%s\n", test.c_str());
	return 0;
}