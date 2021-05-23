#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <syslog.h>
#include <time.h>
#include <errno.h>

char *drive[] = {"https://drive.google.com/file/d/1ZG8nRBRPquhYXq_sISdsVcXx5VdEgi-J&export=download", "https://drive.google.com/file/d/1ktjGgDkL0nNpY-vT7rT7O6ZI47Ke9xcp&export=download", "https://drive.google.com/file/d/1FsrAzb9B5ixooGUs0dGiBr-rC7TS9wTD&export=download"};
char *dir[] = {"/home/nazhwaameera/Praktikum/", "/home/nazhwaameera/Praktikum/Musyik", "/home/nazhwaameera/Praktikum/Fylm", "/home/nazhwaameera/Praktikum/Pyoto", "/home/nazhwaameera/Praktikum/Lopyu_Stevany"};
char *xtrctd[] = {"/home/nazhwaameera/Praktikum/MUSIK", "/home/nazhwaameera/Praktikum/FILM", "/home/nazhwaameera/Praktikum/FOTO"};
char *nama[] = {"Musyik", "Fylm", "Pyoto", "MUSIK", "FILM", "FOTO"};

void createDir()
{
	pid_t stepcreate;
	int statuscreate;
	
	stepcreate = fork();
	
	if(stepcreate < 0) exit(EXIT_FAILURE);
	
	if(stepcreate == 0)
	{
		char *argv[] = {"mkdir", "-p", nama[0], nama[1], nama[2], NULL};
   		execv("/usr/bin/mkdir", argv);
	}
  	else
  	{
  		while((wait(&statuscreate)) > 0);
		return;
	}
}

void extractZip(char fileName[])
{
	pid_t stepextract;
	int statusextract;
	
	stepextract = fork();
	
	if(stepextract < 0) exit(EXIT_FAILURE);
	
	if(stepextract == 0)
	{
		char *argv[] = {"unzip", fileName, NULL};
    	execv("/usr/bin/unzip", argv);
	}
	else
	{
		while((wait(&statusextract)) > 0);
		return;
	}
    
}

void move(int isFolder)
{    
	pid_t stepmove;
	int statusmove;
	
	stepmove = fork();
	
	if(stepmove < 0) exit(EXIT_FAILURE);
	
	if(stepmove == 0)
	{
		if(isFolder == 1)
		{
    		char *argv[] = {"find", "/home/nazhwaameera/Praktikum/MUSIK", "-type", "f", "-exec", "mv", "{}", "/home/nazhwaameera/Praktikum/Musyik", ";", NULL};
    		execv("/usr/bin/find", argv);
		}
	    else if(isFolder == 2)
		{
			char *argv[] = {"find", "/home/nazhwaameera/Praktikum/FILM", "-type", "f", "-exec", "mv", "{}", "/home/nazhwaameera/Praktikum/Fylm", ";", NULL};
    		execv("/usr/bin/find", argv);
		}
   		else if(isFolder == 3)
    	{
    		char *argv[] = {"find", "/home/nazhwaameera/Praktikum/FOTO", "-type", "f", "-exec", "mv", "{}", "/home/nazhwaameera/Praktikum/Pyoto", ";", NULL};
    		execv("/usr/bin/find", argv);
		}
	}
	else
	{
		while((wait(&statusmove)) > 0);
		return;
	}
    
}

void Download(int download)
{
	pid_t stepdownload;
	int statusdownload;
	
	stepdownload = fork();
	
	if(stepdownload < 0) exit(EXIT_FAILURE);
				
	if(stepdownload == 0)
	{
		if(download == 1)
		{
			char *argv[] = {"wget", "-q", "--no-check-certificate", drive[0], "-O", "/home/nazhwaameera/Praktikum/mp3.zip", NULL};
			execv("/usr/bin/wget", argv);
		}
		else if(download == 2)
		{
			char *argv[] = {"wget", "-q", "--no-check-certificate", drive[1], "-O", "/home/nazhwaameera/Praktikum/mp4.zip", NULL};
			execv("/usr/bin/wget", argv);
		}
		else if(download == 3)
		{
			char *argv[] = {"wget", "-q", "--no-check-certificate", drive[2], "-O", "/home/nazhwaameera/Praktikum/jpg.zip", NULL};
			execv("/usr/bin/wget", argv);
		}
	}
	else
	{
		while((wait(&statusdownload)) > 0);
		return;
	}
	
}

void function1()
{
	pid_t function1;
	int statusfunction1;
			
	function1 = fork();
			
	if(function1 < 0) exit(EXIT_FAILURE);
			
	if(function1 == 0)
	{
		pid_t step0;
		int status0;
			
		step0 = fork();
			
		if(step0 < 0) exit(EXIT_FAILURE);
			
		if(step0 == 0)
		{
			pid_t step1;
			int status1;
				
			step1 = fork();
			
			if(step1 < 0) exit(EXIT_FAILURE);
				
			if(step1 == 0)
			{
				createDir; //buat direktori
			}
			else
			{
				while((wait(&status1)) > 0);
				pid_t step2;
				int status2;
					
				step2 = fork();
					
				if(step2 < 0) exit(EXIT_FAILURE);
					
				if(step2 == 0)
				{
					pid_t step3;
					int status3;
						
					step3 = fork();
					
					if(step3 < 0) exit(EXIT_FAILURE);
						
					if(step3 == 0)
					{
						pid_t step4;
						int status4;
							
						step4 = fork();
							
						if(step4 < 0) exit(EXIT_FAILURE);
							
						if(step4 == 0)
						{
							Download(1); //download musik
						}
						else
						{
							while((wait(&status4)) > 0);
							extractZip("mp3.zip"); //unzip musik
						}
					}
					else
					{
						while((wait(&status3)) > 0);
						pid_t step5;
						int status5;
							
						step5 = fork();
							
						if(step5 < 0) exit(EXIT_FAILURE);
							
						if(step5 == 0)
						{
							Download(2); //download film
						}
						else
						{
							while((wait(&status5)) > 0);
							extractZip("mp4.zip"); //unzip film
						}
					}
				}
				else
				{
					while((wait(&status2)) > 0);
					pid_t step6;
					int status6;
							
					step6 = fork();
							
					if(step6 < 0) exit(EXIT_FAILURE);
							
					if(step6 == 0)
					{
						Download(3); //download foto
					}
					else
					{
						while((wait(&status6)) > 0);
						extractZip("jpg.zip"); //unzip foto
					}
				}
			}
		}
		else
		{
			while((wait(&status0)) > 0);
			pid_t step7;
			int status7;
							
			step7 = fork();
						
			if(step7 < 0) exit(EXIT_FAILURE);
							
			if(step7 == 0)
			{
				pid_t step8;
				int status8;
							
				step8 = fork();
					
				if(step8 < 0) exit(EXIT_FAILURE);
					
				if(step8 == 0)
				{
					move(1);
				}
				else
				{
					while((wait(&status8)) > 0);
					move(2);
				}
			}
			else
			{
				while((wait(&status7)) > 0);
				move(3);
			}
		}	
	}
	else
	{
		while((wait(&statusfunction1)) > 0);
		return;
	}
}

void function2()
{
	pid_t function2;
	int statusfunction2;
			
	function2 = fork();
			
	if(function2 < 0) exit(EXIT_FAILURE);
			
	if(function2 == 0)
	{
		pid_t step9;
		int status9;
				
		step9 = fork();
				
		if(step9 < 0) exit(EXIT_FAILURE);
						
		if(step9 == 0) //zip folder Lopyu_Stefany
		{
			char *argv[] = {"zip", "-mr", "/home/nazhwaameera/Praktikum/Lopyu_Stevany", nama[0], nama[1], nama[2], NULL};
			execv("/usr/bin/zip", argv);
		}
		else //remove folder
		{
			while((wait(&status9)) > 0);
			char *argv[] = {"rm", "-r", nama[3], nama[4], nama[5], NULL};
			execv("/usr/bin/rm", argv);
		}
	}
	else
	{
		while((wait(&statusfunction2)) > 0);
		return;
	}
}
int main()
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
	
	if((chdir("/home/nazhwaameera/Praktikum")) < 0)
	{
		exit(EXIT_FAILURE);
	}
	
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	
	while(1)
	{
		time_t waktu;
		struct tm* tm_now;
		
		waktu = time(NULL);
		tm_now = localtime(&waktu);
		
		if(tm_now->tm_mon==3 && tm_now->tm_mday==9 && tm_now->tm_hour==16 && tm_now->tm_min==22 && tm_now->tm_sec==0)
		{
			function1();
		}
		else if(tm_now->tm_mon==3 && tm_now->tm_mday==9 && tm_now->tm_hour==22 && tm_now->tm_min==22 && tm_now->tm_sec==0)
		{
			function2();		
		}
	}
	sleep(1);
}
    

