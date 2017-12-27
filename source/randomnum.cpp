#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <random>
#include <vector>
#include <chrono>
#include <algorithm>	//for shuffle
#include <iterator>

#define MAXSIZE 64
int main(){
	int i;
	FILE *fp;
	fp=fopen("input.txt","w+");
	printf("Making some random numbers...\n");

	std::vector <int> v;
	for (i=1;i<=MAXSIZE;++i) v.push_back(i);	//v[i]=i;

	std::random_device rd;
    std::mt19937 g(rd());
	//随机排列
	std::shuffle(v.begin(),v.end(),g);


	std::vector<int>::iterator p;
	p=v.begin();
	for (i=1;i<=MAXSIZE;++i){
		fprintf(fp,"%d",*p);
		fprintf(fp,"\n");
		p++;		
	}
	printf("Success!\n");
	fclose(fp);
}
