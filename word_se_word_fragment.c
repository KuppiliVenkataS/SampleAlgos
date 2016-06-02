//this algorithm is to search a pattern in a file where the pattern is fragmented and given to the 
//processors sequentially.
//vectors for each of the processors are maintained and the higher ranked processors are dependent on 
//the processors with lower rank.

#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include <string.h>
#include "vect.h"

char fragment[15][50];//each processor fills this array with the fragments they need to search
//int vector[15][100] ;//vector to store occurances in 15 processors

main(int argc, char* argv[])
{
//variables specific to MPI

int my_rank, source, dest=0, tag=0;
MPI_Status status;

//variables general

 int i,j,p,frag_length;
 int noccurances = 0;
 char ch;
 FILE *fopen(),*fp1,*fp2;

char word[2000]="GOBBLEDEGOOKGOOKGOOKGOOKGOBBLEDEGOBBLEDEGOOKGOOKGOOKGOOKGOBBLEDEGOBBLEDEGOOKGOOKGOOKGOOKGOBBLEDEGOBBLEDEGOOKGOOKGOOKGOOKGOBBLEDE";
char searchfile[40]="/home/usr071/lata/mpiprog/Titanic.dat";

int fragmentation (FILE*,char*,int,int);//filepointer,word,my_rank,frag_length
int search_file(FILE*,int);


fp2 = fopen(searchfile,"r");


 
//Initialize MPI
MPI_Init(&argc,&argv);
MPI_Comm_rank(MPI_COMM_WORLD,& my_rank);

MPI_Comm_size(MPI_COMM_WORLD, &p);

i = strlen(word)+1;
//the word should be fragmented
frag_length = i/p;
i =fragmentation(fp2,word,my_rank,frag_length);
	
j = search_file(fp2,my_rank);	

if (my_rank == 0)
	{
printf(" no.of occurances of zeroth fragment is %d \n",j);

		for (source=1;source<p;source++)
		{
		MPI_Recv(&j,1,MPI_INT, source, tag, MPI_COMM_WORLD, &status);
		
		printf(" working %d \n", j);
		
		}//for
	}

else {

	MPI_Send(&j,1,MPI_INT,  dest, tag, MPI_COMM_WORLD);
}//else


printf(" fragment is %s\n",fragment[my_rank]);
//shut down MPI
MPI_Finalize();
fclose(fp2);

}//main
 

int fragmentation(FILE *fp2,char word[],int my_rank,int p)
{
	int i=0,j=0,location=0,match,noccurances=0;
	char ch;
	int vector_position=0;
// which word you want?
	
	i = my_rank * (p);
	ch = word[i];//i is for counting word[] characters

	while ((ch != '\0') && (j < p))
		{
			fragment[my_rank][j] = ch;
			j++;
			ch = word[++i];
			
		fragment[my_rank][j] = '\0';
		}//while


return 1;
}//end of function

int search_file(FILE *fp2,int my_rank)
{
int i=0,j=0,location=0,match,noccurances=0,k=0;
int file_pos;
	
	char ch = getc(fp2);;
	int vector_position=0,frag_length=strlen(fragment[my_rank]);
//search in the file 

if (my_rank == 0)
	{
	i=0;
	//ch = getc(fp2);
	while (ch != EOF)
		{
		location++;

		if (ch ==fragment[my_rank][i])
			{ match = location;

				while (ch == fragment[my_rank][i])
				{
				ch = getc(fp2);
				location++;
				i++;
			
				}//inner while
				if (fragment[my_rank][i] == '\0')
				{
			
				noccurances++;location--;
				//vector[my_rank][vector_position] = match;
				k = fill_vector(my_rank,vector_position,match);// k is filled dummy
				vector_position++;
				
				}
			i=0;
			}
		else 
			{
			ch = getc(fp2);
			}

		}//while 
	}//if
else //my_rank > 0
	{
	i=0;

	//while (vector[my_rank-1][i] != 0)
 	file_pos = get_vector(my_rank-1,i);
	while ( file_pos != 0)
		{
		printf("inside my_rank >0\n");

		k=0;
		j = fseek(fp2,file_pos,SEEK_SET);
		j = ftell(fp2);
	
		ch = getc(fp2);
//printf("%c",ch);	
		while ((ch == fragment[my_rank][k]) && (k <= frag_length))
			{
			k++;ch = getc(fp2);
printf("%c",ch);
			}
			if (k> frag_length)
			{
			//vector[my_rank][vector_position++] = j + frag_length;
			fill_vector(my_rank, vector_position,j+frag_length);
			vector_position++;

			noccurances++;
			}
		
		i++;		
		}//while all the entries in the vector
	}//else

//  *********change this return value************************************************
return noccurances;


}//end of the function





