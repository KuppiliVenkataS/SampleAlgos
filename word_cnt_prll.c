# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "mpi.h"

char file[2][20];

main(int argc, char* argv[])
{

//variables specific to MPI
int my_rank, source, dest=0, tag=0;
MPI_Status status;

int i,p,nwords,total =0;


int word_count(int i);



MPI_Init(&argc, &argv);
MPI_Comm_rank(MPI_COMM_WORLD, & my_rank);

MPI_Comm_size(MPI_COMM_WORLD, &p); // p -> number of processors
printf("give file names\n");
gets(file[my_rank]);
nwords = word_count(my_rank);
if (my_rank == 0)
{
	total = nwords;
printf("nwords = %d\n",nwords);
	for (source=1;source<p;source++)
	{
		MPI_Recv(&nwords, 1, MPI_INT, source, tag, MPI_COMM_WORLD, &status);
		total = total + nwords;
	}//for

}//if the aggregation

else {
	MPI_Send(&nwords, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
}//else

/* print result */

if (my_rank == 0)
{
	printf("hi\n");

	printf("total no_of words= %d\n",total);

}//if

//shut down MPI
MPI_Finalize();
}//main

//------------------------------------------------------------------------
int word_count(int i)
{

FILE *fp,*fopen();
int nwords =0;
char ch;

fp = fopen(file[i],"r");

ch = getc(fp);

while (ch != EOF)
	{
	if (ch == ' ')
	nwords++;

	ch = getc(fp);
	}//while eof
printf("words in the file %s are %d\n",file[i],nwords+1);
return nwords+1;

}//function word_count
