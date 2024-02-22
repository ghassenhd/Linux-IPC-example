#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include "apps.h"

#define INVALID_ARGS        (-2 )

/**
 * check if the file with file_path is modified from the last call
 * @param file_path [IN] : the path of the file
 * @param old_modification_date [IN/OUT] : the modification date read in the last call
 * RETURN : 1 if there is a change, else 0
 */
int file_check(char * file_path, time_t * old_modification_date)
{
  struct stat file_status;
  stat(file_path,&file_status);
  if(file_status.st_mtime != *old_modification_date)
  {
#ifdef DEBUG_LOG
    printf("the file has changed\n");
#endif
    *old_modification_date=file_status.st_mtime;
    return 1;
  }
  return 0;
}

int main(int argc, char ** argv)
{
  char * file_path="/tmp/access_points", * fifo="/tmp/fifo_for_app_a_and_b", fifo_buf[MAX_FIFO_MSG];
  int option='h', sleep_duration=1, fd;
  pid_t app_b_pid=0;
  time_t old_modification_date=0;
  //reading options
  while ((option = getopt (argc, argv, "f:p:h")) != -1)
  {
    switch(option)
    {
      case 'f':
        file_path = optarg;
        break;
      case 'p':
        sleep_duration = atoi(optarg);
        break;
      case '?': case 'h':
        printf( "%s [-f <file path>] [-p <period in seconds>]\n"
                "f : the path of the access points\n"
                "p : the wainting duration befor checking the file\n",argv[0]); 
        return INVALID_ARGS;
      default :
        printf( "[getopt] failed to parse the arguments \n"
                "%s [-f <file path>] [-p <period in seconds>]\n"
                "f : the path of the access points\n"
                "p : the wainting duration befor checking the file\n",argv[0]); 
        return INVALID_ARGS;
    }
  }
  //sending the path of the access point file to app_b
  mkfifo(fifo, 0666);
  fd = open(fifo, O_WRONLY);
  write(fd, file_path, strlen(file_path)+1);
  close(fd);
  //reciving the pid of app_b
  fd = open(fifo, O_RDONLY);
  read(fd, fifo_buf, MAX_FIFO_MSG);
  app_b_pid=atol(fifo_buf);
  close(fd);
  //check if there is modification on the file after each sleep_duration 
  //seconds and if there is a change send SIGUSR1 to app_b
  while(1)
  {
    if(file_check(file_path, &old_modification_date))
    {
      kill(app_b_pid,SIGUSR1);
    }
    sleep(sleep_duration);
  }

  return 0;
}
