#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

/*
  - path : startup folder path
  - flag : 1 recursive mode, 0 no

  Stampa a video il path del file e la size.
 */

int scan_path(char *path, int flag)
{
  struct dirent* in_file;
  DIR *fd;
  
  printf("=== PATH = %s ===\n",path);

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
      strncat(name, "\\", 1);
      strncat(name, in_file->d_name, strlen(in_file->d_name));

      
      struct stat buf;
      int f=1;

      if( stat(name,&buf) < 0 )
	{
	  perror("Error: stat");
	  return -1;
	};

      if( S_ISDIR(buf.st_mode) && flag)
	{
	  printf("\n");
	  scan_path(name,1);
	  f=!f;
	}

      if(f) printf("Path = %s, Size = %d bytes\n",name, buf.st_size);

      free(name);
    }

  closedir(fd);
  
  return 0;
}




// Test
int main()
{

  return scan_path("DIR",1);
}
