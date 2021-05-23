#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

int second[4][6], dikurangkan, hasil = 1;

int selisih(int n, int m)
{
    if(n == 0 || m == 0)
    {
    	hasil = 0;
    }
    return n - m;
}

long long jumlah(long long n)
{
    if(n != 0)
    {
        return  n * jumlah(n - 1);
    }
    else
    {
        return 1;
    }
}

void *faktorial(void* n)
{
    long long angka = *(long long*)n;
    if(hasil == 0)
    {
	printf("0\t");
    }
    else if(dikurangkan >= 1) // a >= b
    {
        printf("%lld\t", jumlah(angka)/jumlah(dikurangkan));
        
    }
    else if(dikurangkan < 1) //b > a
    {
        printf("%lld\t", jumlah(angka));
      
    }
}

int main()
{
    key_t key = 1234;
    int (*value)[6];
    int shmid = shmget(key, sizeof(int[4][6]), IPC_CREAT | 0666);
    value = shmat(shmid, NULL, 0);
    
    int c, d, k, index = 0, sum = 0;
    pthread_t tid[24];
    int first[4][6];
    
    printf("Masukkan elemen matriks:\n");
    
    for(c = 0; c < 4; c ++)
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
            long long *k = malloc(sizeof(long long[4][6]));
            *k = value[c][d];
            //printf("%d\n", value[c][d]);
            hasil = 1;
            dikurangkan = selisih(value[c][d], second[c][d]);
            pthread_create(&tid[index], NULL, &faktorial, k);
           // printf("\t");
            index++;
            sleep(1);
        }
        
        printf("\n");
    }
    
    for(int i = 0; i < index; i++)
    {
        pthread_join(tid[i], NULL);
    }
    
    shmdt(value);
    shmctl(shmid, IPC_RMID, NULL);
    
}


