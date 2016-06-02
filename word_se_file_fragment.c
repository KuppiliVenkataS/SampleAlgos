//A program to search for a given pattern  across the input file in parallel
//Input file is fragmented.
//observation: though the file is succesfully and segmented and searched, there is no way to search 
//for the pattern if the it is present in parts in the fragments of the file. 
// overlapping technique should be used to overcome the above problem

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mpi.h"


main(int argc, char* argv[])
{

//variables specific to MPI

int my_rank, source, dest=0, tag=0,frag_length=0;
MPI_Status status;

int i,j,p, noccurances=0,total=0,location=0,match;
long filepos,file_end,file_length,search_till,search_from;
char word[200];
char inputfile[50]="/home/usr071/lata/mpiprog/Titanic_1912.dat";
char searchfile[40]="/home/usr071/lata/mpiprog/Titanic.dat";

char ch;

FILE *fopen(), *fp1,*fp2;

int search_file_frag(FILE*,char*,long,long);

fp1 = fopen(inputfile,"r");
fp2 = fopen(searchfile,"r");

ch = getc(fp1);

while (ch != EOF)
		{
		word[i++] = ch;
		ch = getc(fp1);
		}
	word[i] = '\0';



//fragment the file

j = fseek(fp2,1,SEEK_END);
file_length = ftell(fp2);

//Initialize MPI
MPI_Init(&argc,&argv);
MPI_Comm_rank(MPI_COMM_WORLD,& my_rank);

MPI_Comm_size(MPI_COMM_WORLD, &p);

//file fragmentation
filepos= (file_length *my_rank)/p;


search_till = filepos + file_length/p ;

i =search_file_frag(fp2,word,filepos,search_till);

printf("my_rank = %d, filepos = %ld,  search_till = %ld\n",my_rank,filepos,search_till);
	

if (my_rank == 0)
	{
		total = i;
	//printf(" no.of occurances  is %d on source = 0\n",i);
		for (source=1;source<p;source++)
		{
		MPI_Recv(&i,1,MPI_INT, source, tag, MPI_COMM_WORLD, &status);
		
		//printf(" no.of occurances  is %d on source = %d\n",i,source);
		total += i;
	
		
		}//for
printf("total no. of occurances = %d\n",total);
	}

else {
	MPI_Send(&i,1,MPI_INT,  dest, tag, MPI_COMM_WORLD);
}//else


//shut down MPI
MPI_Finalize();
fclose(fp1);

}//main

// search the fragment in the file fragment
int search_file_frag(FILE *fp2,char word[],long search_from,long search_till)
{
	int i=0,j=0,noccurances=0,word_length,match;
	long l=search_from;
	
	char ch;

	word_length = strlen(word) - 1;
	j= fseek(fp2,search_from,SEEK_SET);
	
	ch = getc(fp2);
	
	while (l <= search_till)
	{
		
		while (ch == word[i])
			{	match =l;
				l++;ch = getc(fp2);
				i++;
			
			}//inner while
			
		
				if (i >= word_length)
				{
					noccurances++;l--;
					//printf("\nmatch occured at %d \n",match);
					j= fseek(fp2,l,SEEK_SET);
				}
				ch= getc(fp2);l++;i=0;
		
	}// outer while

return noccurances;			
}//end of the function
