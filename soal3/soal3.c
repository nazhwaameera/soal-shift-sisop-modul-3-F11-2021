#include <stdio.h>
#include <ctype.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>

pthread_t T[100];
char direktori[1024];
char save[1000][1000];
int indeks = 0;

int start(const char *p) // cek file atau folder
{
    struct stat p_stat;
    stat(p, &p_stat);
    return S_ISREG(p_stat.st_mode);
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
    char *ext2 = strchr(namafile, '.'); // kalau ada 2 ext, ambil yang paling depan
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

int fungsiRekursif(char *basePath)
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
            fungsiRekursif(path);
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
    strcpy(file, src);
        
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

void* moveFile2(void *arg)
{
    char *P;
    char extension[100];
    char dest[100]; // destinasi
    char asal[100]; // source
    char split[100];
    char fileName[100];
    char cwd[1024]; // cwd
    char *sep1, *sep2; // separate
    char *a[5];
    char *an[5];
    int x = 0;

    P = (char*) arg;

    strcpy(asal, arg);
    strcpy(split, arg);
    sep1 = strtok(P, "."); // split path

    while(sep1 != 0)
    {
        a[x] = sep1; x++;
        sep1 = strtok(NULL, ".");
    }
    if(x == 1)
    {
        strcpy(extension, "Unknown");
    }
    else
    {
        int y;
        for(y = 0; y < strlen(a[x - 1]); y++)
        {
            extension[y] = tolower(a[x - 1][y]); // untuk menyimpan seluruh ekstensi
        }
    }
    sep2 = strtok(split, "/"); // split direktori
    while(sep2 != 0)
    {
        an[x] = sep2;x++;
        sep2 = strtok(0, "/");
    }

    strcpy(fileName, an[x-1]);

    getcwd(cwd, sizeof(cwd)); // mendapatkan direktori yang sedang dibuka
    strcpy(dest, cwd);
    strcat(dest, "/");
    strcat(dest, extension);
    __builtin___memset_chk (extension, 0,100, __darwin_obsz0 (extension));
    mkdir(dest,0777);

    FILE *p1;
    FILE *p2;

    strcat(dest, "/");
    strcat(dest, fileName);
    
    p1 = fopen(asal, "r");
    p2 = fopen(dest, "w");

    int ch;
    if(!p1)
    {
        fclose(p2);
    }
    if(!p2)
    {
        
    }
    while((ch = fgetc(p1)) != -1)
    {
        fputc(ch, p2); // pindah ke file lama
    }
    remove(asal); // menghapus file lama
    return 0;
}

int main(int argc, char* argv[])
{
    int x;
    char src[1000];
    
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
                pthread_create(&(T[x - 2]), 0, moveFile2, (void *)argv[x]); // untuk thread
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
    if(strcmp(argv[1], "-d") == 0)
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
    if(!fungsiRekursif(dir))
    {
        printf("Yah, gagal disimpan :(\n");
    }
    
    pthread_t tid2[indeks];
    int i = 0, j = 0;
    while(i < indeks)
    {
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
