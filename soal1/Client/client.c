#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8080
  
int main(int argc, char const *argv[]) {
    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;

    char buffer[1024] = {0};
    char login_status[1024] = {0};

    char pilihan[100];

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
      
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
  
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    char id[20], pass[20], input_akun[100];
    
    printf("Register atau Login? ");
    scanf("%s", pilihan);

    //kirim perintah register atau login ke server
    send(sock , pilihan , strlen(pilihan) , 0 );

    //masukkan id dan pass lalu kirim ke server
    printf("Masukkan id : ");
    scanf("%s", id);
    printf("Masukkan pass : ");
    scanf("%s", pass);
    strcpy(input_akun, id);
    strcat(input_akun, ":");
    strcat(input_akun, pass);
    input_akun[strlen(input_akun)] = '\0';
    send(sock , input_akun , strlen(input_akun) , 0 );

    if(pilihan[0]=='r'){
       printf("Register berhasil\n");
    }
    else if(pilihan[0]=='l'){
        valread = read( sock , login_status, 1024);
        if(login_status[0]=='b'){
            printf("Login berhasil\n");
        }
        else{
            printf("Login gagal!!\n");
            return 0;
        }   
    }

    while(1){
        printf("\nMasukkan perintah : ");
        scanf("%s", pilihan);
        // kirim comand ke
        send(sock , pilihan, strlen(pilihan) , 0 );

        if(pilihan[0]=='a'){
            char input_file[200];
            char publisher[100], tahun[100], filepath[100], nama[100], ekstensi[100];
            long int posisi;
            //ambil variabel2 yang mai diinput
            printf("Publisher : ");
            scanf("%s", publisher);
            printf("Tahun Publikasi: ");
            scanf("%s", tahun);
            printf("Filepath : ");
            scanf("%s", filepath);

            //mengambil nama dan ekstensi dari pathfile
            char* hasil = strchr(filepath, '/');
            while(hasil != NULL){
                posisi = hasil - filepath+1;
                hasil = strchr(hasil+1, '/');
            }
            int ii=0, jj=0, eks=0;
            for(int i= posisi; i<strlen(filepath); i++){
                nama[ii++] = filepath[i];
                if(eks)
                    ekstensi[jj++]=filepath[i];
                if (filepath[i]=='.')
                    eks=1;
            }
            nama[ii]='\0';
            ekstensi[jj]='\0';

            //menyusun input_file lalu kirim ke server
            strcpy(input_file, nama);
            strcat(input_file, "\t");
            strcat(input_file, publisher);
            strcat(input_file, "\t");
            strcat(input_file, tahun);
            strcat(input_file, "\t");
            strcat(input_file, ekstensi);
            strcat(input_file, "\t");
            strcat(input_file, filepath);
            input_file[strlen(input_file)] = '\0';
            send(sock , input_file, strlen(input_file) , 0 );
        }
        if(pilihan[0]=='d' && pilihan[1]=='e'){
            char nama[100];
            scanf("%s", nama);

            //mengirim nama file yang ingin didelete
            send(sock , nama, strlen(nama) , 0 );
        }
        if(pilihan[0]=='s' || pilihan[0]=='f'){
            if(pilihan[0]=='f'){
                char word[100];
                scanf("%s", word);

                //mengirim kata yang ingin dicari dalam files.tsv
                send(sock ,word, strlen(word) , 0 );
            }

            char line[1024] = {0};
            char baris[1024];
            //mengambil isi file files.tsv dari server
            valread = recv( sock , line, 1024, 0);
            
            // menulis sesuai keinginan soall ehehe
            int n = strlen(line);
            int i=0;
            while(1){
                printf("\nNama: ");
                while(line[i] != '\t'){
                    printf("%c", line[i++]);
                }
                i++;
                printf("\nPublisher: ");
                while(line[i] != '\t'){
                    printf("%c", line[i++]);
                }
                i++;
                printf("\nTahun Publishing: ");
                while(line[i] != '\t'){
                    printf("%c", line[i++]);
                }
                i++;
                printf("\nEkstensi File: ");
                while(line[i] != '\t'){
                    printf("%c", line[i++]);
                }
                i++;
                printf("\nFilepath: ");
                while((int)line[i] != 10){
                    printf("%c", line[i++]);
                }
                i++;
                printf("\n");
                if(i==strlen(line))
                    break;
            }
            printf("\n");
        }
        if(pilihan[0]=='d' && pilihan[1]=='o'){
            char nama[100];
            char download[1024] = {0};
            scanf("%s", nama);
            
            //kirim nama file yang ingin di download
            send(sock , nama, strlen(nama) , 0 );
            valread = read( sock , download, 1024);
            if(strcmp(download,"File tidak ada")==0){
                printf("%s\n", download);
            }
            else{
                FILE *fptr1, *fptr2;
                char path1[100];
                strcpy(path1, download);
                strcat(path1, "/Server/FILES/");
                strcat(path1, nama);
                printf("%s\n", path1);

                //open file mula-mula
                fptr1 = fopen(path1, "r");
                if(!fptr1)
                    printf("fptr1 eror\n");

                //open file tujuan
                fptr2 = fopen(nama, "a");
                if(!fptr2)
                    printf("fptr2 eror\n");    

                //copy
                char c = fgetc(fptr1);
                while (c != EOF)
                {
                    fputc(c, fptr2);
                    c = fgetc(fptr1);
                }
            }

        }
        if(pilihan[0]=='e')
            break;
    }
    return 0;
}