//---------------------------------------------------------------------------

#pragma hdrstop

//---------------------------------------------------------------------------

#pragma argsused

/*
*  Generate a precision/recall graphic script
*  argv[1] = input filename
*  argv[2] = total number of columns of file, including the transaction id and category
*  argv[3] = string in the format [+i1,i2,...,ik]
*  with the indexes of columns to be cosidered
*  the first attribute of the feature vector is 1 (one).
*  ps: the output file is default out.plt
**/
#include "Reader.h"
#include "DistanceCalculus.h"
#include "PrecRecCalculus.h"
#include <conio.h>

int main(int argc, char * argv[]){
	char * filename = argv[1];
	int nrocols= atoi(argv[2]);
	char * format = argv[3];
	printf("\n%s",filename);
	printf("\n%d",nrocols);
	printf("\n%s",format);
	Reader * reader = new Reader(filename,nrocols,format);
	//See if database was read correctly
        FILE * fa = fopen("datasel.txt","w");
	reader->firstTransaction();
	do{
		reader->showCurrent(fa);
	}while(reader->nextLine()!=-1);
        fclose(fa);
       	/*//See if DistanceCalculus is correctly working
	DistanceCalculus * cq = new DistanceCalculus(4);
	//test2: Distance Function ok
	//vector has id, categories, features
	float c1[] = {0,0,7,4}, c2[] = {0,0,4,8};
	printf("\n****\n %.4f\n****",cq->getDistance(c1,c2)); */
	
	PrecRecCalculus * prc = new PrecRecCalculus(reader);
        printf("\nProgram ended, press any key to continue...");
        getch();
}

