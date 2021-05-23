#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#define PORT 8080

char addRunning(char* isi1, char* isi2, char* isi3){
    char isi[100];
    FILE *frunning;
    frunning= fopen("running.log","a");

    strcpy(isi, isi1);
    strcat(isi, " : ");
    strcat(isi, isi2);
    strcat(isi, " (");
    strcat(isi, isi3);
    strcat(isi, ")");

    fputs(isi, frunning);
    fputs("\n", frunning);
    fflush(frunning);
    fclose(frunning);

}

int main(int argc, char const *argv[]) {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char id[1024] = {0};
    char pass[1024] = {0};
    char pilihan1[1024] = {0};
    char input_akun[1024] = {0};

    char *path = "/home/rayhan12/cmpt/soal1fix/soal1";
      
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
      
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
      
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    FILE *fakun, *ffiles, *fptr1, *fptr2;

    //ambil pilihan register atau login dari client
    valread = read( new_socket , pilihan1, 1024);
    //ambil id dan pass
    valread = read( new_socket , input_akun, 1024);
    if(pilihan1[0]=='r'){
        fakun= fopen("akun.txt","a");

        //input id dan pass akun baru
        fputs(input_akun, fakun);
        fputs("\n", fakun);
        fflush(fakun);
        fclose(fakun);
    }
    else{
        char line[1024];
        int ada=0;

        //cek id dan pass sama 
        FILE* srcFile = fopen("akun.txt", "r");
        while (fgets(line , sizeof(line) , srcFile )!= NULL)
        {   
            if (strstr(line , input_akun) != NULL)
                ada=1;
        }

        if(ada)
            send(new_socket , "berhasil", 8 , 0 );
        else{
            send(new_socket , "gagal", 5 , 0 );
            return 0;
        }
            
    }

    while(1){
        char pilihan[1024] = {0};
        //ambil perintah dari client
        valread = read( new_socket , pilihan, 1024);

        if(pilihan[0]=='a'){
            //ambil input nama, publisher, tahun publikasi, ekstensi, filepath
            char input_file[1024] = {0};
            valread = read( new_socket , input_file, 1024);

            //tambah file di files.tsv
            ffiles= fopen("files.tsv","a");
            fputs(input_file, ffiles);
            fputs("\n", ffiles);
            printf("%s\n", input_file);
            fflush(ffiles);
            fclose(ffiles);

            //ambil tiap variabel yang dipisahin '\t'
            char filepath2[100], filepath[100], nama[100];
            char* hasil = strchr(input_file, '\t');
            long int posisi, posisi1;
            posisi1 = hasil - input_file+1;
            while(hasil != NULL){
                posisi = hasil - input_file+1;
                hasil = strchr(hasil+1, '\t');
            }
    
            // mendapat variabel nama dalam input_file
            int ii=0;
            for(int i = 0; i<posisi1-1; i++){
                nama[ii++] = input_file[i];

            }
            nama[ii]='\0';

            // mendapat variabel filepath dalam input file
            ii=0;
            for(int i = posisi; i<strlen(input_file); i++){
                filepath[ii++] = input_file[i];

            }
            filepath[ii]='\0';

            //cek udah ada folder FILES blm
            DIR* dir = opendir("FILES");
            if (!dir){
                int check = mkdir("FILES",0777);
            }

            //open file mula-mula
            fptr1 = fopen(filepath, "r");
            if(!fptr1)
                printf("fptr1 eror\n");

            //open file tujuan
            strcpy(filepath2, "FILES/");
            strcat(filepath2, nama);

            fptr2 = fopen(filepath2, "a");
            if(!fptr2)
                printf("fptr2 eror\n");    

            //copy
            char c = fgetc(fptr1);
            while (c != EOF)
            {
                fputc(c, fptr2);
                c = fgetc(fptr1);
            }
            //tambah running.log
            addRunning("Tambah", nama, input_akun);
        }
        if(pilihan[0]=='d' && pilihan[1]=='e'){
            char filepath[100], filepath2[100];
            char nama[1024] = {0};

            //ambil nama file yang mau didelete dari client
            valread = read( new_socket , nama, 1024);

            //hapus line
            char line[1024], data_file[1024] ;
            FILE* srcFile = fopen("files.tsv", "r");
            FILE* tempFile = fopen("delete-line.tmp", "w");
            while (fgets(line , sizeof(line) , srcFile )!= NULL)
            {   
                if (strstr(line , nama )!= NULL)
                    continue;  
                fputs(line, tempFile);
            }
            fclose(srcFile);
            fclose(tempFile);
            remove("files.tsv");
            rename("delete-line.tmp", "files.tsv");

            //ganti nama file jadi old
            strcpy(filepath, "FILES/");
            strcat(filepath, nama);
            strcpy(filepath2, "FILES/old-");
            strcat(filepath2, nama);
            rename(filepath, filepath2);
            
            //tambah running.log
            addRunning("Hapus", nama, input_akun);
        }
        if(pilihan[0]=='s'){
            char line[1024], data_file[1024] ;

            //mengirim file files.tsv ke client
            FILE* srcFile = fopen("files.tsv", "r");
            while (fgets(line , sizeof(line) , srcFile )!= NULL)
            {   
                line[strlen(line)]='\0';
                send(new_socket , line , strlen(line) , 0 );
            }
        }
        if(pilihan[0]=='f'){
            char word[1024] = {0};
            char line[1024];
            //ambil kata yang di cari client
            valread = read( new_socket , word, 1024);

            //mencari baris yang ada word nya trus kirim ke client
            FILE* srcFile = fopen("files.tsv", "r");
            while (fgets(line , sizeof(line) , srcFile )!= NULL)
            {   
                if (strstr(line , word)!= NULL){
                    line[strlen(line)]='\0';
                    send(new_socket , line , strlen(line) , 0 );
                }
            }
        }
        if(pilihan[0]=='d' && pilihan[1]=='o'){
            char line[1024];
            int ada=0;
            char file[1024] = {0};
            //ambil nama file yang ingin didownload client
            valread = read( new_socket , file, 1024);
            
            //cek apakah ada file ituuu
            FILE* srcFile = fopen("files.tsv", "r");
            while (fgets(line , sizeof(line) , srcFile )!= NULL)
            {   
                if (strstr(line , file)!= NULL && !ada ){
                    line[strlen(line)]='\0';
                    ada=1;
                    send(new_socket , path , strlen(path) , 0 );
                    break;
                }
            }
            if(!ada){
                char *pesan = "File tidak ada";
                send(new_socket , pesan, strlen(pesan) , 0 );
            }
        }
        if(pilihan[0]=='e')
            break;
    }
    return 0;
}