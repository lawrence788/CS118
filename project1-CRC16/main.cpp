#include <iostream>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>


using namespace std;

int main(int argc, char *argv[]){
	//G is 1000100001010001
	cout << "Hello World\n";
	int opt;
	extern int optind;
	extern char *optarg;

	//spec 5. if no argument exit with error 2
	if(argc == 1){
		//cout << "No arguments\n";
		exit(2);
	}
	
	//This loop detects the input flag, and call the relative function
	while((opt = getopt(argc, argv, "c:v:f:t:")) != -1){
		//cout << opt;
		switch(opt){
			case 'c':
				cout << "bit string + CRC\n";
				cout << optarg;
				cout <<"\n";
				break;
			case 'v':
				cout << "Validating with the bit string with\n";
				cout << optarg;
				cout <<"\n";
				break;
			case 'f':
				cout << "Undetected 4 bit errors\n";
				cout << optarg;
				cout <<"\n";
				break;
			case 't':
				cout << "fraction of Undetected 2 biterrors \n";
				cout << optarg;
				cout <<"\n";
				break;
			case '?': 
				//cout << "???? \n";
				exit(2);
				break;
		}

	}
	
	
	return 0;
}