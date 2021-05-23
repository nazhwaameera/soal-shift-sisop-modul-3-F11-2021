#include <time.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <dirent.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>

void killer(char *argv[], int pid)
{
    // 3d untuk membuat file killer.sh
    FILE *kill = fopen("killer.sh", "w");
    fprintf(kill, "#/bin/bash\n");

    // 3e
    // arg -z untuk menghentikan semua operasi
    if(strcmp(argv[1], "-z") == 0)
    {
        fprintf(kill, "killall -9 soal3\n");
    }
    // arg -x untuk kill proses utama --> pid
    else if(strcmp(argv[1], "-x") == 0)
    {
        fprintf(kill, "kill -9 %d\n", pid);
    }

    // delete folder setelah selesai di zip
    fprintf(kill, "rm killer.sh\n");
    fclose(kill);
}

int main(int argc, char** argv)
{
    pid_t pid, sid;
    pid = fork();
    
    if(pid < 0)
    {
        exit(EXIT_FAILURE);
    }
    if(pid > 0)
    {
        exit(EXIT_SUCCESS);
    }
    
    umask(0);
    sid = setsid();
    
    if(sid < 0)
    {
        exit(EXIT_FAILURE);
    }
    
    // untuk menjalankan argumen -z dan -x
    killer(argv, (int)getpid());
    
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    
    while(1)
    {
        pid_t child_id;
        int status;
        child_id = fork();
        time_t raw;
        struct tm *timeinfo;
        char tanggal[40];
        time(&raw);
        timeinfo = localtime(&raw);
        strftime(tanggal, sizeof(tanggal), "%Y-%m-%d_%H:%M:%S", timeinfo); // format penamaan folder sesuai dengan waktu saat folder tersebut berjalan
        
        if(child_id < 0)
        {
            exit(EXIT_FAILURE);
        }
        if(child_id == 0)
        {
            char *argv[4] = {"mkdir", "-p", tanggal, NULL};
            execv("/bin/mkdir", argv);
        }
        else
        {
            while((wait(&status)) > 0);
            pid_t child_id_2;
            int i = 0;
            child_id_2 = fork();
            
            if(child_id_2 == 0)
            {
                for(i = 1; i <= 10; i++)
                {
                    time_t raw2;
                    struct tm *timeinfo2;
                    char tanggal2[40];
                    time(&raw2);
                    timeinfo2 = localtime(&raw2);
                    strftime(tanggal2, sizeof(tanggal2), "%Y-%m-%d_%H:%M:%S", timeinfo2); // format penamaan file gambar yang telah di download sesuai dengan waktu pada saat mendownload
                    
                    char alamat[50];
                    sprintf(alamat, "https://picsum.photos/%ld", ((raw2 % 1000) + 50)); // link gambar beserta ukurannya
                    pid_t child_id_3;
                    child_id_3 = fork();
                    
                    if(child_id_3 == 0)
                    {
                        chdir(tanggal);
                        char *f[] = {"wget", "-bq", alamat, "-O" , tanggal2, NULL};
                        execv("/usr/bin/wget", f);
                    }
                    sleep(5); // gambar di download tiap 5 detik
                }
                
                // isi file status.txt
                char kata[50] = "Download Success";
                char word;
                      
                // caesar cipher
                for(int i = 0; i < strlen(kata); i++)
                {
                    word = kata[i];
                    if(word == ' ')
                    {
                        continue;
                    }
                    if(word >= 'a' && word <= 'z')
                    {
                        word += 5;
                        if(word > 'z')
                        {
                            word = word - 'z' + 'a' - 1;
                        }
                        kata[i] = word;
                    }
                    else if(word >= 'A' && word <= 'Z')
                    {
                        word += 5;
                        if(word > 'Z')
                        {
                            word = word - 'Z' + 'A' - 1;
                        }
                        kata[i] = word;
                    }
                }
                // untuk membuat file bernama status.txt
                char file[100];
                sprintf(file, "%s/status.txt", tanggal);
                FILE *status = fopen(file, "w");
                fputs(kata, status);
                fclose(status);
                
                // untuk mengubah folder menjadi bentuk zip sekaligus menghapus folder tersebut
                char zip[50];
                sprintf(zip, "%s.zip", tanggal);
                char *argv2[5] = {"zip", "-rm", zip, tanggal, NULL};
                execv("/usr/bin/zip", argv2);
            }
        }
        // program berjalan setiap 40 detik
        sleep(40);
    }
}
