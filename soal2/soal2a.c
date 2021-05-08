#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

int main()
{
	key_t key = 1234;
	int (*value);
	int shmid = shmget(key, sizeof(int[4][6]), IPC_CREAT | 0666);
	value = shmat(shmid, NULL, 0);
	
	int c, d,k, sum = 0;
	int first[4][3], second[3][6], multiply[4][6];
	
	printf("Masukkan elemen matriks pertama (4 X 3):\n");
	
	for(c = 0; c < 4; c ++)
	{
		for(d = 0; d < 3; d++)
		{
			scanf("%d", &first[c][d]);
		}
	}
	
	printf("Masukkan elemen mtriks kedua (3 x 6):\n");
	
	for(c = 0; c < 3; c++)
	{
		for(d = 0; d < 6; d++)
		{
			scanf("%d", &second[c][d]);
		}
	}
	
	for(c = 0; c < 4; c++)
	{
		for(d = 0; d < 6; d++)
		{
			for(k = 0; k < 3; k++)
			{
				sum = sum + first[c][k] * second[k][d];
			}
			
			multiply[c][d] = sum;
			sum = 0;
		}
	}
	
	printf("Hasil perkalian matriks: \n");
	
	for(c = 0; c < 4; c++)
	{
		for(d = 0; d < 6; d++)
		{
			printf("%d ", multiply[c][d]);
		}
		
		printf("\n");
	}
	
	for(c = 0; c < 4; c++)
	{
		for(d = 0; d < 6; d++)
		{
			*value = multiply[c][d];
			sleep(1);
		}
	}
	
	shmdt(value);
	return 0;
}
