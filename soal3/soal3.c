#include <math.h>
#include <time.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include <syslog.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <string.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#define EOF (-1)

int start(const char *P) // cek file atau folder
{
    struct stat P_stat;
    stat(P, &P_stat);
    return S_ISREG(P_stat.st_mode);
}

int cekFile(const char * filename) // untuk cek file ada apa engga
{
    FILE *file;
    file = fopen(filename, "rb");
    if(file != NULL)
    {
        printf("%s\n", filename);
        fclose(file);
        if(start(filename))
            return 1; // cek file
        else
            return 0;
    }
    return 0;
}

void cekExt(char* namafile, char *ext1) // cek extension
{
    char *ext2 = strchr(namafile, '.'); // kalau ada 2 ext -> ambil paling depan
    if(ext2 == namafile)
    {
        strcpy(ext1, "Hidden");
    }
    else if(ext2 == NULL)
    {
        strcpy(ext1, "Unknown");
    }
    else
    {
        strcpy(ext1, ext2 + 1);
        for(int x = 0; x < strlen(ext1); x++) // untuk ubah jadi lowercase
        {
            ext1[x] = tolower(ext1[x]);
        }
    }
}

pthread_t T[100];
char direktori[1024];
char save[1000][1000];
int indeks = 0;

int listFilesRecursively(char *basePath)
{
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(basePath);

    if(!dir)
        return 0;

    while((dp = readdir(dir)) != NULL)
    {
        if(strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            char destDir[1000];
            strcpy(destDir, basePath);
            strcat(destDir, "/");
            strcat(destDir, dp->d_name);
            printf("%s\n", destDir);
           
            if(cekFile(destDir))
            {
                strcpy(save[indeks], destDir);
                indeks += 1;
            }
            
            strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, dp->d_name);

            listFilesRecursively(path);
        }
    }
    closedir(dir);
    return 1;
}

void *moveFile(void* argc)
{
    char* src = (char*) argc;

    //pindah file
    char file[1000];
    strcpy(file,src);
       
    char *ndir, *dir;
    char x = '/'; // path
    char y = '.';
    dir = strrchr(file, x); // yang pertama kali dijumpai
    ndir = strchr(file, y);
    char ext[1000];

    if(dir)
    {
        if(cekFile(file)) // ngecek file/dir ada apa ngga
        {
            cekExt(dir + 1, ext);
        }
        else
        {
            return 0;
        }
    }
    mkdir(ext, 0777); // extension dir
    
    //src
    char path[1000];
    strcpy(path, (char*) argc);
    
    //dst
    char fileCat[1000];
    getcwd(fileCat, sizeof(path));
    strcat(fileCat, "/");
    strcat(fileCat, ext);
    strcat(fileCat, "/");
    strcat(fileCat, dir + 1);
    printf("%s\n%s\n", path, fileCat);
    rename(path, fileCat);
    
    return(void *) 1;
    pthread_exit(0);
}

int main(int argc, char* argv[])
{
    int x;
    char src[100];
    // get dir
    getcwd(direktori, sizeof(direktori));
    strcpy(src, direktori);
    char dir[100];
    
    if(strcmp(argv[1], "-f") == 0)
    {
        for(x = (1 + 1); x < argc; x++)
        {
            if(start(argv[x]))
            {
                pthread_create(&(T[x - 2]), 0, moveFile, (void *)argv[x]); // untuk thread
                printf("File %d : Berhasil Dikategorikan\n", x - 1);
            }
            else
            {
                printf("File %d : Sad, Gagal :(\n", x - 1);
            }
        }
        for(x = 0; x < argc - 2; x++)
        {
            pthread_join(T[x], 0);
        }
    }
    else if(strcmp(argv[1], "-d") == 0)
    {
        if(argc == 3) // bisa menerima 1 path
        {
            strcpy(dir, argv[2]);
        }
    }
    else if(strcmp(argv[1], "*") == 0)
    {
        strcpy(dir, src);
    }
    if(!listFilesRecursively(dir))
    {
        printf("Yah, gagal disimpan :(\n");
    }
    
    pthread_t tid2[indeks];
    int i = 0, j = 0;
    while(i < indeks)
    {
        printf("%s\n", save[i]);
        pthread_create(&tid2[i], NULL, moveFile, (void *)save[i]);
        i++;
    }
    while(j < indeks)
    {
        void *ptr;
        pthread_join(tid2[j], &ptr);
        j++;
    }
    
    if(strcmp(argv[1], "-d") == 0)
        printf("Direktori sukses disimpan!\n");
    return 0;
}
