# soal-shift-sisop-modul-3-F11-2021
Repository soal shift sisop modul 3 kelompok 11 kelas F

## Soal 1
## soal a (login atau register)
soal meminta untuk client bisa login atau register dengan multiple connection tetapi ketika ada 2 client yang terhubung dengan server maka client yang kedua akan menunggu client pertama. berikut adalah untuk pemasukan data akun . program akan meminta inputan antara register atau login. Kemudian dilanjutkan dengan  untuk multiple connection belum bisa terimplementasikan karena masih kesusahan dalam membuat kodingannya.
```C
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
```

Jika `register` memasukkan string input_akun dari client ke file akun.txt

Jika `login` server mengecek apakah string input_akun tersebut ada dalam list akun di file akun.txt
berikut adalah kodingannya dalam file server.c
```C
 
    valread = read( new_socket , pilihan1, 1024);
    
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
```

tampilan jika berhasil login dan berhasil register

![punya taq](https://github.com/EEB12/Alarm-Pendeteksi-gas/blob/main/Arduino/register.PNG?raw=true )

![punya taq](https://github.com/EEB12/Alarm-Pendeteksi-gas/blob/main/Arduino/login.PNG?raw=true )

untuk multi connection saat ini belum bisa hingga seperti berikut

![punya taq](https://github.com/EEB12/Alarm-Pendeteksi-gas/blob/main/Arduino/logingagal.PNG?raw=true )


## Soal b
diminta untuk membuat  database yang bernama files.tsv. Isi dari files.tsv ini adalah path file saat berada di server, publisher, dan tahun publikasi. Setiap penambahan dan penghapusan file pada folder file yang bernama  FILES pada server akan memengaruhi isi dari files.tsv. Folder FILES otomatis dibuat saat server dijalankan
client mengirim string input_file yang berisi (nama file, publisher, tahun publikasi, ekstensi, filepath) ke server. untuk kodingannya digabung dengan soal c

## Soal C
untuk soal C diminta untuk menambahkan perintah `add` untuk penambahan file pada server dan setiap kali pemasukan file terdapat perubahan pada files.tsv.
```C
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
            strcat(input_file, ",");
            strcat(input_file, publisher);
            strcat(input_file, ",");
            strcat(input_file, tahun);
            strcat(input_file, ",");
            strcat(input_file, ekstensi);
            strcat(input_file, ",");
            strcat(input_file, filepath);
            input_file[strlen(input_file)] = '\0';
            send(sock , input_file, strlen(input_file) , 0 );
        }
```

server menerima string input_file lalu memasukkan ke dalam file files.tsv lalu mengambil bagian filepath untuk mengcopy tiap karakter yang ada pada file tersebut ke sebuah file baru di Server/FILES/namafile.ekstensi kemudian mengisi file running.log dengan fungsi yang telah dibuat
```C
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
  }
```
berikut adalah hasil screenshot isi FILES dan files.tsv setelah `add` 

![punya taq](https://github.com/EEB12/Alarm-Pendeteksi-gas/blob/main/Arduino/addF.PNG?raw=true )

![punya taq](https://github.com/EEB12/Alarm-Pendeteksi-gas/blob/main/Arduino/addtsv.PNG?raw=true )


## Soal D
client dapat mendownload file yang telah ada dalam folder FILES di server, sehingga sistem harus dapat mengirim file ke client. Server harus melihat dari files.tsv untuk melakukan pengecekan apakah file tersebut valid. Jika tidak valid, maka mengirimkan pesan error balik ke client. Jika berhasil, file akan dikirim dan akan diterima ke client di folder client tersebut.
untuk soal ini kita terdapat kesalahan dalam penulisan program. pada soal diminta untuk mendowload file misalnya client dan server adalah 2 device yang berbeda sedangkan pada program kita hanya mengandalkan copy address.

berikut pada client akan meminta input nama untuk nama file lalu dikirim pada server.

``` C
if(pilihan[0]=='d' && pilihan[1]=='o'){
            char nama[100];
            char download[1024] = {0};
            scanf("%s", nama);
            
            //kirim nama file yang ingin di download
            send(sock , nama, strlen(nama) , 0 );
	    
```

server menerima nama file yang ingin di download dari client kemudian mengirim filepath file yang diinginkan diambil dari `files.tsv`
```C
if(pilihan[0]=='d' && pilihan[1]=='o'){
            char line[1024];
            int ada=0;
            char file[1024] = {0};
            
            valread = read( new_socket , file, 1024);
            
           
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
```

lalu client menerima filepath dari file yang ingin didownload, lalu mengcopy tiap karakter dari file tersebut ke file baru yang dibuat di direktori Client 
```C
if(pilihan[0]=='d' && pilihan[1]=='o'){
            char nama[100];
            char download[1024] = {0};
            scanf("%s", nama);
            
            
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

             
                fptr1 = fopen(path1, "r");
                if(!fptr1)
                    printf("fptr1 eror\n");

                
                fptr2 = fopen(nama, "a");
                if(!fptr2)
                    printf("fptr2 eror\n");    

                
                char c = fgetc(fptr1);
                while (c != EOF)
                {
                    fputc(c, fptr2);
                    c = fgetc(fptr1);
                }
            }

        }
```
berikut adalah hasil download
![punya taq](https://github.com/EEB12/Alarm-Pendeteksi-gas/blob/main/Arduino/download.PNG?raw=true )

## Soal E
pada soal E diminta untuk membuat perintah `Delete`. ketika perintah dijalankan file yang ingin didelete tetap ada hanya saja diganti nama filenya menjadi `old-NamaFile.ekstensi`. tetapi pada file.tsv row yang menunjuk ke nama file tersebut dihapus.

Berikut client mengirim nama file yang akan di hapus ke server
```C
 if(pilihan[0]=='d' && pilihan[1]=='e'){
            char nama[100];
            scanf("%s", nama);

            //mengirim nama file yang ingin didelete
            send(sock , nama, strlen(nama) , 0 );
        }
```

server menerima nama file kemudian menghapus baris yang berisi nama tersebut pada `files.tsv` dan mengganti nama file pada direktori `Server/FILES/namafile.ekestensi` menjadi `old-namafile.ekstensi`. Server menambah baris pada running.log dengan memanggul fungsi yang telah dibuat

```C
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
```
berikut hasil setelah menjalankan perintah `delete`
![punya taq](https://github.com/EEB12/Alarm-Pendeteksi-gas/blob/main/Arduino/filedelete.PNG?raw=true )
![punya taq](https://github.com/EEB12/Alarm-Pendeteksi-gas/blob/main/Arduino/tsvdelete.PNG?raw=true )

## Soal F
Client dapat melihat semua isi files.tsv dengan memanggil suatu perintah yang bernama see. untuk kodingan akan ada di gabung dengan soal G

## Soal G
Aplikasi client juga dapat melakukan pencarian dengan memberikan suatu string. Hasilnya adalah semua nama file yang mengandung string tersebut. Format output seperti format output f. aplikasi membuat perintah `find` untuk mencari file tersebut.

### perintah `see` dan `find`
perintah `see` server mengirim semua isi dari `files.tsv` sebagai sebuah string ke client

perintah `find` server mengirim isi dari `files.tsv` yang tiap barisnya berisi kata yang dicari dari client

berikut adalah kodingan untuk `find` dan `see`

```C
if(pilihan[0]=='s'){
            char line[1024], data_file[1024] ;

            
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
```

client mencetak string yang telah dikirim server.
```C

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
```
berikut adalah hasil run perintah `see` dan `find`


![punya taq](https://github.com/EEB12/Alarm-Pendeteksi-gas/blob/main/Arduino/see.PNG?raw=true )
![punya taq](https://github.com/EEB12/Alarm-Pendeteksi-gas/blob/main/Arduino/find.PNG?raw=true )

## soal H
membuat log untuk tiap pertambahan dan penghapusan file di server dengan log bernama running.log. 
 
fungsi addRunning() dengan tiga argumen yaitu :
- `isi1` yaitu string yang berisi tambah atau hapus.
- `isi2` yaitu string yang berisi nama file yang ditambah atau dihapus.
- `isi3` yaitu string yang berisi tid dan password client yang menambah atau menghapus.

```
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
```
## Soal 2
Crypto (kamu) adalah teman Loba. Pada suatu pagi, Crypto melihat Loba yang sedang kewalahan mengerjakan tugas dari bosnya. Karena Crypto adalah orang yang sangat menyukai tantangan, dia ingin membantu Loba mengerjakan tugasnya. Detail dari tugas tersebut adalah :

**a.** Membuat program perkalian matriks (4 × 3 dengan 3 × 6) dan menampilkan hasilnya. Matriks nantinya akan berisi angka 1-20

**b.** Membuat program dengan menggunakan matriks output dari program sebelumnya (program soal 2a.c) (Catatan : gunakan shared memory). Kemudian matriks tersebut akan dilakukan perhitungan dengan matriks baru (input user) sebagai berikut contoh perhitungan untuk matriks yang ada. Perhitungannya adalah setiap sel yang berasal dari matriks A menjadi angka untuk faktorial, lalu sel dari matriks B menjadi batas maksimal faktorialnya (dari paling besar ke paling kecil) (Catatan : gunakan thread untuk perhitungan di setiap sel).

**c.** Karena takut lag dalam pengerjaannya membantu Loba, Crypto juga membuat program (soal2c.c) untuk mengecek 5 proses teratas apa saja yang memakan resource komputernya dengan command “ps aux | sort -nrk 3,3 | head -5” (Catatan : harus menggunakan IPC Pipes)

Dari soal di atas, dapat diketahui bahwa kita diminta untuk :
1. Membuat program perkalian matriks (4 × 3 dengan 3 × 6) dan menampilkan hasilnya.
2. Menyimpan hasil perkalian program perkalian matriks tersebut ke dalam shared memory.
3. Membuat program yang menerima input berupa matriks 4 × 6 untuk kemudian dioperasikan dengan matriks hasil perkalian program pertama.
4. Membuat program untuk mengecek 5 proses teratas yang  memakan resource komputer menggunakan IPC Pipes dan command “ps aux | sort -nrk 3,3 | head -5”

Untuk menyelesaikannya, dibuat program seperti di bawah ini.
Untuk menyelesaikan nomor 1, kita menggunakan :  
```C
for(c = 0; c < 4; c++)
{
	for(d = 0; d < 6; d++)
        {
            for(k = 0; k < 3; k++)
            {
                sum = sum + first[c][k] * second[k][d];
            }
            value[c][d] = sum;
            sum = 0;
        }
}
```
Program ini akan menghitung perkalian matriks 4 × 3 dengan 3 × 6 yang telah diinputkan dan menyimpan hasilnya di array value. Array 4 × 3 dengan 3 × 6 sendiri disimpan menggunakan array.

Seperti yang kita ketahui, program kedua meminta kita untuk mengoperasikan matriks baru dengna matriks hasil perkalian 4 × 3 dengan 3 × 6 yang kita dapat dari program ini. Untuk itu, kita perlu menyimpan matriks hasil perkalian program ini ke dalam shared memory. Syntax untuk shared memory adalah :
```C
key_t key = 1234;
    int (*value)[6];
    int shmid = shmget(key, sizeof(int[4][6]), IPC_CREAT | 0666);
    value = shmat(shmid, NULL, 0);
```
Operasi yang diminta dalam soal dapat dituliskan seperti di bawah
```C
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
```
Ketiga fungsi di atas digunakan dalam perhitungan faktorial seperti yang diminta dalam soal. Fungsi selisih mengomputasi terlebih dahulu berapa selisih elemen dari kedua matriks yang menempati posisi yang sama, fungsi ini juga sekaligus mengecek apakah ada salah satu di antara kedua elemen yang bernilai 0, apabila ada, fungsi akan mengembalikan nilai 0 juga.  Hasil perhitungan selisih kemudian digunaka pula untuk mengklasifikasikan apakah elemen matriks A lebih besar atau lebih kecil dari elemen matriks B dari posisi yang sama. Apabila lebih besar, elemen tersebut akan difaktorialkan kemudian dibagi dengan hasil faktorial selisih elemen bilangan A dengan B. Apabila lebih kecil, fungsi akan mengembalikan hasil faktorial dari elemen A.
Kita perlu mengakses shared memory dan membuat thread untuk soal kali ini.
```C
key_t key = 1234;
int (*value)[6];
int shmid = shmget(key, sizeof(int[4][6]), IPC_CREAT | 0666);
value = shmat(shmid, NULL, 0);

pthread_t tid[24];
Setelah menginisiasi thread dan shared memorynya, fungsi berikut akan mengakses shared memory tersebut dan membuat thread untuk setiap proses komputasinya.
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
```
Kita kemudian menggabungkan semua thread yang sudah selesai menggunakan : 
```C
for(int i = 0; i < index; i++)
{ 
    pthread_join(tid[i], NULL);
}
```
Untuk subsoal terakhir, kita diminta untuk mengecek 5 proses teratas yang memakan resource komputer menggunakan command “ps aux | sort -nrk 3,3 | head -5” dan IPC Pipes :
```C
int pid;
int pipe1[2];
int pipe2[2];

void exec1() 
{
  // input from stdin (already done)
  // output to pipe1
  dup2(pipe1[1], 1);
  // close fds
  close(pipe1[0]);
  close(pipe1[1]);
  // exec
  execlp("ps", "ps", "aux", NULL);
  _exit(1);
}

void exec2() 
{
  // input from pipe1
  dup2(pipe1[0], 0);
  // output to pipe2
  dup2(pipe2[1], 1);
  // close fds
  close(pipe1[0]);
  close(pipe1[1]);
  close(pipe2[0]);
  close(pipe2[1]);
  // exec
  execlp("sort", "sort", "-nrk", "3.3", NULL);

  _exit(1);
}

void exec3() 
{
  // input from pipe2
  dup2(pipe2[0], 0);
  // output to stdout (already done)
  // close fds
  close(pipe2[0]);
  close(pipe2[1]);
  // exec
  execlp("head", "head", "-5", NULL);
 
  _exit(1);
}
```
Hasil :

![VirtualBox_Ubuntu 2 0_23_05_2021_17_03_22](https://user-images.githubusercontent.com/68385532/119256948-55705300-bbed-11eb-9d84-41cc233c92c3.png)
![VirtualBox_Ubuntu 2 0_23_05_2021_17_04_06](https://user-images.githubusercontent.com/68385532/119256944-530df900-bbed-11eb-812b-7d0852b444bc.png)

## Soal 3
Seorang mahasiswa bernama Alex sedang mengalami masa gabut, dan ia memikirkan untuk merapikan sejumlah file yang ada di laptopnya. Karena jumlah filenya terlalu banyak, Alex meminta saran ke Ayub. Ayub menyarankan untuk membuat sebuah program C agar file-file dapat dikategorikan. Program ini akan memindahkan file ke dalam folder sesuai ekstensinya yang folder hasilnya terdapat di working directory ketika program kategori tersebut dijalankan.

### Bagian a
Program menerima opsi -f dimana pengguna bisa menambahkan argumen file yang bisa dikategorikan sebanyak yang diinginkan oleh pengguna. Dengan output yakni *File 1 : Berhasil dikategorikan* jika berhasil, *File 2 : Sad, gagal:(* jika gagal.

Berikut adalah fungsi ```moveFile2``` untuk memindahkan dan mengkategorikan file pada opsi -f :
```C
void* moveFile2(void *arg) // untuk opsi -f
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
```
Menerima argumen -f :
```C
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
```
Dibawah ini adalah file dengan kategori berbeda yang terdapat dalam satu direktori yang sama.
<img width="593" alt="Screen Shot 2021-05-23 at 10 55 11" src="https://user-images.githubusercontent.com/74484044/119247691-0fe46380-bbb6-11eb-93bb-c6052cf36eea.png">

Lalu kita coba jalankan opsi -f dan saya akan mengkategorikan file 3.zip, 4.png, 8.log, 7.heic, serta 66.jpg
<img width="565" alt="Screen Shot 2021-05-23 at 10 57 14" src="https://user-images.githubusercontent.com/74484044/119247692-11159080-bbb6-11eb-9e34-0b0a245eaa74.png">

**Note :** File ke 5 yakni 66.jpg gagal dikategorikan karena tidak ada file dengan nama tersebut, yang ada yakni 6.jpg, maka akan keluar output File 5 : Sad, gagal:(

Berikut adalah folder yang terbentuk setelah menjalankan opsi -f :
<img width="587" alt="Screen Shot 2021-05-23 at 10 58 24" src="https://user-images.githubusercontent.com/74484044/119247693-1246bd80-bbb6-11eb-889d-eaf2c62dc3cb.png">

Dibawah ini adalah isi di dalam masing-masing folder yang sudah terkategorikan :

<img width="157" alt="Screen Shot 2021-05-23 at 11 11 15" src="https://user-images.githubusercontent.com/74484044/119247996-47540f80-bbb8-11eb-970a-42c2ffc8c9ed.png"> <img width="181" alt="Screen Shot 2021-05-23 at 11 11 21" src="https://user-images.githubusercontent.com/74484044/119247999-491dd300-bbb8-11eb-8883-b4206bf30448.png"> <img width="158" alt="Screen Shot 2021-05-23 at 11 11 27" src="https://user-images.githubusercontent.com/74484044/119248000-49b66980-bbb8-11eb-80bf-7328cc421d42.png"> <img width="159" alt="Screen Shot 2021-05-23 at 11 11 34" src="https://user-images.githubusercontent.com/74484044/119248003-4ae79680-bbb8-11eb-9518-bd02d699d9c9.png">

Fungsi ```moveFile``` untuk opsi -d dan *
```C
char *ndir, *dir;
char x = '/'; // path
char y = '.';
dir = strrchr(file, x); // yang pertama kali dijumpai
ndir = strchr(file, y);
```
```strrchr``` digunakan untuk mendapatkan string yang dipisahkan dengan tanda / dengan mengecek dari string bagian belakang. Sedangkan, ```strchr``` digunakan untuk mendapatkan string yang dipisahkan dengan tanda . dengan mengecek dari string bagian depan.

```C
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
```
Jika path yang dicek merupakan direktori atau file, maka akan mengecek apakah path file tersebut memang ada dengan fungsi ```cekFile```. Jika ada, maka akan memisahkan extension dari nama file tersebut dengan fungsi ```cekExt``` yang akan dijelaskan pada bagian 3d.
```C
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
```
Untuk membuat direktori berupa kategori dari path yang diinputkan dengan ```mkdir```. Kemudian, direktori tersebut disimpan pada current working directory dengan melakukan rename pada path awal dari direktori tersebut.

### Bagian b
Program juga dapat menerima opsi -d untuk melakukan pengkategorian pada suatu directory. Namun pada opsi -d ini, user hanya bisa memasukkan input 1 directory saja, tidak seperti file yang bebas menginput file sebanyak mungkin. Lalu hasilnya akan disimpan di working directory dimana program C tersebut berjalan (hasil kategori filenya bukan di /path/to/directory). Dengan output yakni *"Direktori sukses disimpan!"* jika berhasil, dan *"Yah, gagal disimpan :("* jika gagal.

- Menerima argumen -d :
```C
else if(strcmp(argv[1], "-d") == 0)
{
    if(argc == 3) // bisa menerima 1 path
    {
        strcpy(dir, argv[2]);
    }
}
```
Menggunakan ```strcmp``` untuk membandingkan argumen input apakah sesuai dengan argumen input yang diminta yakni -d. Command -d akan dilaksanakan jika hanya memasukkan 1 path saja, maka menggunakan argc == 3. Path tersebut akan disimpan di sebuah char dir.

- Hasil yang ditampilkan :
```C
if(strcmp(argv[1], "-d") == 0)
    printf("Direktori sukses disimpan!\n"); // jika berhasil
```
```C
if(!fungsiRekursif(dir))
    printf("Yah, gagal disimpan :(\n"); // jika gagal
```
- Output :
Kita akan mengkategorikan file yang berada pada direktori soal3 seperti berikut.
<img width="568" alt="Screen Shot 2021-05-23 at 15 21 19" src="https://user-images.githubusercontent.com/74484044/119253265-a0cd3600-bbda-11eb-8af8-c17289f48e1f.png">
<img width="565" alt="Screen Shot 2021-05-23 at 15 21 33" src="https://user-images.githubusercontent.com/74484044/119253266-a1fe6300-bbda-11eb-9b1e-d9e012b6ee50.png">
Dan akan dipindahkan di current working directory dimana tempat kodingan C berada.
<img width="692" alt="Screen Shot 2021-05-23 at 15 20 41" src="https://user-images.githubusercontent.com/74484044/119253268-a32f9000-bbda-11eb-92c9-5d89b65b02f7.png">

### Bagian c
Selain menerima opsi-opsi di atas, program ini menerima opsi * yaitu mengkategorikan seluruh file yang ada di working directory ketika menjalankan program C tersebut.
- Menerima argumen *
```C
else if(strcmp(argv[1], "*") == 0)
{
    strcpy(dir, src);
}
```
Menggunakan ```strcmp``` untuk membandingkan argumen input, apakah sesuai dengan argumen input yang diminta yakni * dan path tersebut akan disimpan di sebuah char dir.
- Output :
Sebelum menjalankan argumen *
<img width="672" alt="Screen Shot 2021-05-23 at 15 35 38" src="https://user-images.githubusercontent.com/74484044/119253727-c65b3f00-bbdc-11eb-9e84-106e0aa58e48.png">
Sesudah menjalankan argumen * maka seluruh file yang ada akan masuk ke dalam folder sesuai dengan kategorinya masing-masing.
<img width="697" alt="Screen Shot 2021-05-23 at 15 40 20" src="https://user-images.githubusercontent.com/74484044/119253824-3b2e7900-bbdd-11eb-8736-27638bbaa92a.png">

### Bagian d
Semua file harus berada di dalam folder, jika terdapat file yang tidak memiliki ekstensi, file disimpan dalam folder “Unknown”. Jika file hidden, masuk folder “Hidden”.
```C
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
```
File Hidden adalah file yang memiliki awalan . pada nama file, sehingga jika file diawali dengan . maka akan dikategorikan sebagai Hidden, dan file yang tidak memiliki extension dibelakanganya akan dikategorikan sebagai Unknown. Sedangkan untuk nama extension yang ditulis dengan uppercase dapat diubah menjadi lowercase dengan menggunakan tolower seperti diatas.
- Output :
Untuk folder Hidden dan Unknown sudah terbentuk dan bisa dilihat pada gambar diatas, folder Hidden tidak berisikan apa-apa, dan untuk folder Unknown berikut isinya.
<img width="683" alt="Screen Shot 2021-05-23 at 15 47 56" src="https://user-images.githubusercontent.com/74484044/119254045-5483f500-bbde-11eb-8480-424138ac3ee7.png">

### Bagian e
Setiap 1 file yang dikategorikan dioperasikan oleh 1 thread agar bisa berjalan secara paralel sehingga proses pengkategorian bisa berjalan lebih cepat.
```C
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
```
Thread diatas adalah untuk command -d dan * ```pthread_create``` dan ```pthread_join``` akan dibuat sebanyak kategori yang ingin dibuat dari file yang ada pada direktori tersebut.
