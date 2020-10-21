#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <limits.h>
#include <time.h>
#include <string.h>

void printents(DIR * dir, int a) //int a: 1 = no arg, 2 = -i, 3 = -l, 4 = -R
{
  struct dirent *de;
  struct stat fileStat;
  char *p=getenv("USER");
  char date[10];
  while ((de = readdir(dir)) != NULL)
  {
    if (strcmp(de->d_name, ".") != 0  && strcmp(de->d_name, "..") != 0)
    {
      if (a == 1) { printf("%s  ", de->d_name); }
      if (a == 2) { printf("%d %s\n",de->d_ino,de->d_name); }
      if (a == 3)
      {
        stat(de->d_name, &fileStat);
        printf( (S_ISDIR(fileStat.st_mode)) ? "d" : "-");           //permissions
        printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-");
        printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-");
        printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");
        printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-");
        printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-");
        printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-");
        printf( (fileStat.st_mode & S_IROTH) ? "r" : "-");
        printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-");
        printf( (fileStat.st_mode & S_IXOTH) ? "x" : "-");
        printf("  %-3d",fileStat.st_nlink);
        printf("%-8s",p);
        printf("%-7d",fileStat.st_size);
        strftime(date, 20, "%d/%m/%y", localtime(&(fileStat.st_ctime)));   //date last modified
        printf("%-10s", date);
        printf("%s\n", de->d_name);
      }
      if (a == 4)
      {
        if (de->d_type == DT_DIR)
        {
          printf(" %s  ", de->d_name);
        }
      }
    }
  }
  closedir(dir);
}

void printblocks(DIR * dir)
{
  struct dirent *de;
  struct stat stats;
  int blksize = 0;
  while ((de = readdir(dir)) != NULL)
  {
    if (strcmp(de->d_name, ".") != 0  && strcmp(de->d_name, "..") != 0)
    {
      stat(de->d_name, &stats);
      blksize = blksize + (int)stats.st_blocks/2;
    }
  }
  printf("Total: %d\n", blksize);
  closedir(dir);
}

//Doesnt print out exactly the same but the functionality should still be similar.
//Recursively prints out directories and their subdirectories.
static void printR(const char * dir_name)
{
  DIR * dir = opendir (dir_name);;
  if (! dir)
  {
    fprintf (stderr, "Cannot open directory '%s'\n", dir_name);
  }
  while (1)
  {
    struct dirent * de;
    const char * d_name;
    de = readdir (dir);
    if (! de)
    {
      break;
    }
    //d_name = de->d_name;
    if (de->d_type == DT_DIR)
    {
      if (strcmp(de->d_name, ".") != 0  && strcmp(de->d_name, "..") != 0)
      {
        int path_length;
        char path[PATH_MAX];
        path_length = snprintf (path, PATH_MAX, "%s/%s", dir_name, de->d_name);
        //printf ("%s\n", path);

        printf (" %s/%s\n", dir_name, de->d_name);
        printR(path);
      }
	  }
  }
  closedir(dir);
}

int main(int argc, char **argv)
{
    char current_dir[PATH_MAX];
    DIR *curd;
    if (getcwd(current_dir,sizeof(current_dir)) != NULL)
    {
      curd = opendir(current_dir);
    }
    else { perror("NULL pointer returned from cwd\n"); }

    if (argc < 2)   //input
    {
      printents(curd,1);
    }
    else if(strcmp(argv[1],"-i")==0)
    {
      printents(curd,2);
    }
    else if(strcmp(argv[1],"-l")==0)
    {
      printblocks(curd);
      curd = opendir(current_dir);
      printents(curd,3);
    }
    else if(strcmp(argv[1],"-R")==0)
    {
      //printR(".", 0);
      printf("Directories: \n");
      printents(curd,4);
      printf("\n\nDirectories + Subdirectories:\n");
      printR(".");
    }
    else { perror("Cannot recognize argument or input\n"); }
    printf("\n");
    return 0;
}
