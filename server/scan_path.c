#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>


/*
  return the size of directory
 */

unsigned long size_dir(char *path)
{
  struct dirent* in_file;
  DIR *fd;
  unsigned long size=0;

  fd = opendir(path);
  if( fd == NULL)
    {
      perror("File not found");
      return -1;
    }

   while( in_file = readdir(fd) )
    {
      
      if (!strcmp (in_file->d_name, "."))
	continue;
      if (!strcmp (in_file->d_name, ".."))
	continue;

      int length = strlen(in_file->d_name)+strlen(path)+2;
      char *name = malloc(sizeof(char)*length);

      struct stat buf;
      
      memset(name, '\0', length);
      
      strncpy(name, path, strlen(path));
      strncat(name, "\\", 1);
      strncat(name, in_file->d_name, strlen(in_file->d_name));

      if( stat(name,&buf) < 0 )
	{
	  perror("Error: stat");
	  return -1;
	};
      
      size+=buf.st_size;
      free(name);
    }
   closedir(fd);
   return size;
}


/*
  - path : startup folder path
  - flag : 1 recursive mode, 0 no

  Stampa a video il path del file, la size, il timestamp e i permessi.
 */

int scan_path(char *path, int flag, time_t tserver)
{
  struct dirent* in_file;
  DIR *fd;
  
  printf("\n=== PATH = %s ===\n",path);

  fd = opendir(path);
  if( fd == NULL )
    {
      perror("File not found");
      return -1;
    }

  while( in_file = readdir(fd) )
    {
      
      if (!strcmp (in_file->d_name, "."))
	continue;
      if (!strcmp (in_file->d_name, ".."))
	continue;


      int length = strlen(in_file->d_name)+strlen(path)+2;
      char *name = malloc(sizeof(char)*length);

      memset(name, '\0', length);
      
      strncpy(name, path, strlen(path));
      strncat(name, "/", 1);
      strncat(name, in_file->d_name, strlen(in_file->d_name));

      
      struct stat buf;
      int f=1;
        printf("%s\n", name);

        if( stat(name,&buf) < 0 )
	{
	  perror("Error: stat");
	  return -1;
	};

      char *perm = malloc(sizeof(char)*3);
 
      perm[0] = buf.st_mode & S_IEXEC ? 'x' : '-'; //x
      perm[1] = buf.st_mode & S_IREAD ? 'r' : '-'; //r
      perm[2] = buf.st_mode & S_IWRITE ? 'w' : '-'; //w
      

      if( S_ISDIR(buf.st_mode) && flag)
	{
	  printf("Path = %s, Size = %d bytes\n",name, size_dir(name));
	  scan_path(name,1,tserver);
	  f=!f;
	}

      if(f) printf("Path = %s, Size = %d bytes, Timestamp = %d, Rights = %s\n",name, buf.st_size,time(NULL) - tserver, perm);

      free(perm);
      free(name);
    }

  closedir(fd);
  
  return 0;
}




// Test
int main()
{
  time_t current_time = time(NULL); //tempo di avvio del server

  
  return scan_path("/home/mishima/Documents/Syrus/",1,current_time);
}
