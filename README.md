# soal-shift-sisop-modul-3-F11-2021
Repository soal shift sisop modul 3 kelompok 11 kelas F

## Soal 1

## Soal 2

## Soal 3
Seorang mahasiswa bernama Alex sedang mengalami masa gabut, dan ia memikirkan untuk merapikan sejumlah file yang ada di laptopnya. Karena jumlah filenya terlalu banyak, Alex meminta saran ke Ayub. Ayub menyarankan untuk membuat sebuah program C agar file-file dapat dikategorikan. Program ini akan memindahkan file ke dalam folder sesuai ekstensinya yang folder hasilnya terdapat di working directory ketika program kategori tersebut dijalankan.

### Bagian a
Program menerima opsi -f dimana pengguna bisa menambahkan argumen file yang bisa dikategorikan sebanyak yang diinginkan oleh pengguna. Dengan contoh output sebagai berikut.

**File 1 : Berhasil dikategorikan (jika berhasil)**

**File 2 : Sad, gagal:( (jika gagal)**

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
Dan berikut fungsi untuk opsi -f :
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

**Note :** File ke 5 yakni 66.jpg gagal dikategorikan karena tidak ada file dengan nama tersebut, yang ada yakni 6.jpg, maka akan keluar output yakni File 5 : Sad, gagal:(

Berikut adalah folder yang terbentuk setelah menjalankan opsi -f :
<img width="587" alt="Screen Shot 2021-05-23 at 10 58 24" src="https://user-images.githubusercontent.com/74484044/119247693-1246bd80-bbb6-11eb-889d-eaf2c62dc3cb.png">

Dibawah ini adalah isi di dalam masing-masing folder yang sudah terkategorikan :

<img width="157" alt="Screen Shot 2021-05-23 at 11 11 15" src="https://user-images.githubusercontent.com/74484044/119247996-47540f80-bbb8-11eb-970a-42c2ffc8c9ed.png"> <img width="181" alt="Screen Shot 2021-05-23 at 11 11 21" src="https://user-images.githubusercontent.com/74484044/119247999-491dd300-bbb8-11eb-8883-b4206bf30448.png"> <img width="158" alt="Screen Shot 2021-05-23 at 11 11 27" src="https://user-images.githubusercontent.com/74484044/119248000-49b66980-bbb8-11eb-80bf-7328cc421d42.png"> <img width="159" alt="Screen Shot 2021-05-23 at 11 11 34" src="https://user-images.githubusercontent.com/74484044/119248003-4ae79680-bbb8-11eb-9518-bd02d699d9c9.png">

### Bagian b
Program juga dapat menerima opsi -d untuk melakukan pengkategorian pada suatu directory. Namun pada opsi -d ini, user hanya bisa memasukkan input 1 directory saja, tidak seperti file yang bebas menginput file sebanyak mungkin. Lalu hasilnya akan disimpan di working directory dimana program C tersebut berjalan (hasil kategori filenya bukan di /path/to/directory). Berikut contoh outputnya :

**"Direktori sukses disimpan!" (jika berhasil)**

**"Yah, gagal disimpan :(" (jika gagal)**
