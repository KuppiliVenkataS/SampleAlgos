//A program to search for a given pattern  across the input file 
// fragments the string and searches within the file in parallel
// ensure file has read-only access

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mpi.h"


main(int argc, char* argv[])
{

//variables specific to MPI

int my_rank, source, dest=0, tag=0;
MPI_Status status;

int i,j,p, noccurances=0,total=0,location=0,match,frag_length=0;
char word[700],search_word[200][110];
char inputfile[40]="/home/usr071/input.dat"; // contains the fragment to be searched
char searchfile[40]="/home/usr071/SearchFile.dat"; //file to be searched

char ch;

FILE *fopen(), *fp1,*fp2;
FILE *fp[5];

int fragmentation (FILE*,char*,int,int);

fp1 = fopen(inputfile,"r");
fp2 = fopen(searchfile,"r");

ch = getc(fp1);

while (ch != EOF)
		{
		word[i++] = ch;
		ch = getc(fp1);
		}
	word[i] = '\0';

//Initialize MPI
MPI_Init(&argc,&argv); 
MPI_Comm_rank(MPI_COMM_WORLD,& my_rank); // each processor searches for the given string rankwise

MPI_Comm_size(MPI_COMM_WORLD, &p);// provide no.of processors you want

//the word should be fragmented
frag_length = i/p;
	
i =fragmentation(fp2,word,my_rank,frag_length);
	
	

if (my_rank == 0)
	{
printf(" no.of occurances of zeroth fragment is %d \n",i);
		for (source=1;source<p;source++)
		{
		MPI_Recv(&i,1,MPI_INT, source, tag, MPI_COMM_WORLD, &status);
		
		printf(" no.of occurances  is %d \n",i);
		
		}//for
	}

else {
	MPI_Send(&i,1,MPI_INT,  dest, tag, MPI_COMM_WORLD);
}//else

/* print result */

if (my_rank == 4)
{
	
	printf(" ########no.of occurances of 4 is %d \n",i);
	

}//if

//shut down MPI
MPI_Finalize();
fclose(fp1);

}//main

int fragmentation(FILE *fp2,char word[],int my_rank,int p)
{
	int i=0,j=0,k=0,location=0,match,numccurances=0;
	char ch;
	char fragment[251]; // default fragment length = 251 char

// which word you want?
	
	i = my_rank * (p);
	ch = word[i];//i is for counting word[] characters

	while ((ch != '\0') && (j < p))
		{
			fragment[j] = ch;
			j++;
			ch = word[++i];
			
		fragment[j] = '\0';
		}//outer while
printf("the fragment in %d is %s\n",my_rank,fragment);
//search the fragment in the file

i=0;
	ch = getc(fp2);
	while (ch != EOF)
		{
		location++;

		if (ch ==fragment[i])
			{ match = location;
				while (ch == fragment[i])
				{
				ch = getc(fp2);
				location++;
				i++;
			
				}//inner while
			if (fragment[i] == '\0')
			{
			
			numccurances++;location--;
			//printf("%s present at %d location\n",fragment,match);
			
			}
		i=0;
			}
		else 
			{
			ch = getc(fp2);
			}

		}//while 

//return variable
return numccurances;

}//end of the function
