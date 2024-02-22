#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <signal.h>
#include <string.h>
#include<json-c/json.h>
#include "linked_list.h"
#include "apps.h"

#define MAX_CHAR_NUM_IN_FILE (3*1024)

struct sigaction action;
char file_path[MAX_FIFO_MSG];

struct linked_list * new_ap=NULL, * old_ap=NULL;

/**
 * display the added access point
 * @param data [IN] : the data of the access point
 * RETURN : void
 */
void display_added(struct ll_data data)
{
  printf("%s is added to the list with SNR %d and channel %d\n", data.ssid, data.snr, data.channel);
}

/**
 * display the deleted access point
 * @param data [IN] : the data of the access point
 * RETURN : void
 */
void display_deleted(struct ll_data data)
{
  printf("%s is removed from the list\n", data.ssid);
}

/**
 * display the modification if it exist for access point
 * @param old_data [IN] : the old data of the access point
 * @param new_data [IN] : the new data of the access point
 * RETURN : void
 */
void display_modified(struct ll_data old_data,struct ll_data new_data)
{
  if(old_data.snr!=new_data.snr)
    printf("%s’s SNR has changed from %d to %d\n", old_data.ssid, old_data.snr, new_data.snr);
  if(old_data.channel!=new_data.channel)
    printf("%s’s channel has changed from %d to %d\n", old_data.ssid, old_data.channel, new_data.channel);
}

/**
 * check for any modification in the access point list and display them
 * @param local_new_ap [IN] : the address for the updated list
 * RETURN : void
 */
void display_changes(struct linked_list * local_new_ap)
{
  int index=-1;
  struct linked_list * local_old_ap=NULL;
  while (local_new_ap)
  {
    if(-1==(index=find_ll(local_new_ap->data,old_ap)))
    {
      display_added(local_new_ap->data);
    }
    else
    {
      delete_ll_index(index,&old_ap);
    }
    local_new_ap=local_new_ap->next;
  }
  local_old_ap=old_ap;
  while(local_old_ap)
  {
    display_deleted(local_old_ap->data);
    local_old_ap=local_old_ap->next;
  }
}

/**
 * parse the json file.
 * update the new_ap with the data read from the json file.
 * compare between new_ap and old_ap and display the change.
 * and then free the old_ap and update it to new_ap.
 * @param signum [IN] : not used
 * RETURN : void
 */
void signal_handler (int signum)
{
  //ignoring the signal SIGUSR1 so the function signal_handler run without interaptions
  action.sa_handler = SIG_IGN;
  sigaction (SIGUSR1, &action, NULL);

  struct json_object *parsed_json;
  struct json_object * access_points;
  struct json_object * access_point;
  struct json_object * ssid;
  struct json_object * snr;
  struct json_object * channel;
  FILE * fd;
  char buf [MAX_CHAR_NUM_IN_FILE];
  size_t num_access_points;
  //read the whole json file 
  fd=fopen(file_path,"r");
  fread(buf,MAX_CHAR_NUM_IN_FILE,1,fd);
  fclose(fd);
  //prase it
  parsed_json = json_tokener_parse(buf);
  //get the access_points array
  if(json_object_object_get_ex(parsed_json, "access_points",&access_points))
  {
    num_access_points = json_object_array_length (access_points);
    //save each object in the array to the new_ap
    for(int i=0;i<num_access_points;i++)
    {
      if(access_point = json_object_array_get_idx(access_points, i))
      {
        if(!json_object_object_get_ex(access_point, "ssid", &ssid))
        {
#ifdef DEBUG_LOG
          printf("[func=%s | LINE=%d] ERROR : can't prase the ssid\n",__func__,__LINE__);
#endif
          continue;
        }
        if(!json_object_object_get_ex(access_point, "snr", &snr))
        {
#ifdef DEBUG_LOG
          printf("[func=%s | LINE=%d] ERROR : can't prase the snr\n",__func__,__LINE__);
#endif
          continue;
        }
        if(!json_object_object_get_ex(access_point, "channel", &channel))
        {
#ifdef DEBUG_LOG
          printf("[func=%s | LINE=%d] ERROR : can't prase the channel\n",__func__,__LINE__);
#endif
          continue;
        }
      }
      struct ll_data data;
      strncpy(data.ssid,json_object_get_string(ssid),MAX_SSID_LEN+1);
      data.snr=json_object_get_int(snr);
      data.channel=json_object_get_int(channel);
      add_ll_last(data, &new_ap);
    }
  }
  else
  {
    //if there is no access_points array the  the new_ap is empty
    new_ap=NULL;
  }
  //desplay changes and aupdate old_ap
  display_changes(new_ap);
  free_ll(&old_ap);
  old_ap=new_ap;
  new_ap=NULL;

  //re-handle the signal SIGUSR1
  action.sa_handler = signal_handler;
  sigaction (SIGUSR1, &action, NULL);
}

int main ()
{
  char * fifo="/tmp/fifo_for_app_a_and_b", fifo_buf[MAX_FIFO_MSG];
  int fd;
  pid_t pid = getpid();
  //get the path of the json file from app_a
  mkfifo(fifo, 0666);
  fd = open(fifo,O_RDONLY);
  read(fd, file_path, MAX_FIFO_MSG);
  close(fd);
  //send this app pid to app_a
  snprintf(fifo_buf,MAX_FIFO_MSG,"%ld",(long int)pid);
  fd = open(fifo, O_WRONLY);
  write(fd, fifo_buf, MAX_FIFO_MSG);
  close(fd);
  //initializing the signal handler
  memset (&action, 0, sizeof (action));
  action.sa_handler = signal_handler;
  sigaction (SIGUSR1, &action, NULL);
  //waiting for the signal
  while(1);
    sleep(10);

  return 0;
}
