#include <iostream>
#include <stdlib.h>
#include "naivebayesian.h"
#include "bayesiannetwork.h"
#include <string>
using namespace std;

int main(  ){

int method=1;


/*
char* zhuanhuan(string src)
{
	char *dst = new char[src.length() + 1];
	for (int i = 0; i < src.length() + 1; i + +)
		dst[i] = src[i];
	return dst;
}
*/
string str1("breast-cancer-wisconsin400_data.txt");
string str2("breast-cancer-wisconsin299_test.txt");
string str3("breast-cancer-wisconsin_cfg.txt");

char *train = new char[str1.length() + 1];
for (int i = 0; i < str1.length() + 1; i++)
	train[i] = str1[i];

char* input = new char[str2.length() + 1];
for (int i = 0; i < str2.length() + 1; i++)
	input[i] = str2[i];

char* cfg = new char[str3.length() + 1];
for (int i = 0; i < str3.length() + 1; i++)
	cfg[i] = str3[i];
/*
if( argc >= 5 ){
	method = atoi(argv[4]);
	train = argv[1];
	input = argv[2];
	cfg = argv[3];
}else if( argc == 4 ){
	train = argv[1];
	input = argv[2];
	cfg = argv[3];
	std::cout<<" use default NaiveBayesian method"<<std::endl;
}else {
	std::cout<<" You need to provide training data, test data, and configuration for prediction. Please read README"<<std::endl;
}
*/
if( method == 0 ){
	baysian::naivebayesian naive(train, input, cfg);
}else if( method == 1 ){
	baysian::bayesiannetwork bnetwork(train, input, cfg);
}
while (1);
return 0;
}


