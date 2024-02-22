#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "linked_list.h"

/**
 * look for ssid in ll with same value of data ssid and display the change
 * @param data [IN] : the access point with the ssid to look for
 * @param ll [IN] : the list of access point to look in
 * RETURN : the index of the access point in the list withe the same ssid of
 * data, else -1
 */
int find_ll (struct ll_data data, struct linked_list * ll)
{
  int i=0;
  while (ll)
  {
    if(!strncmp(ll->data.ssid,data.ssid,MAX_SSID_LEN))
      {
        display_modified(ll->data,data);
        return i;
      }
      i++;
      ll=ll->next;
   }
   return -1;
}

/**
 * delete the access point with the index "index"
 * @param index [IN] : the index of the access point to delete
 * @param linked_list [IN/OUT] : the list of access points 
 * RETURN : 1 if the access point is deleted, else 0
 */
int delete_ll_index (int index, struct linked_list ** ll)
{
  struct linked_list * buff=NULL;
  if(!*ll)
    return  0;
  if(0 >= index)
  {
    buff = (*ll)->next;
    free (*ll);
    *ll=buff;
    return 1;
  }
  return delete_ll_index(index-1,&((*ll)->next));
}

/**
 * add a new access point at the end of the list
 * @param data [IN] : access point to be added
 * @param ll  [IN/OUT] : the list of access points
 * RETURN : void
 */
void add_ll_last(struct ll_data data, struct linked_list ** ll)
{
  struct linked_list * buff = (struct linked_list *)malloc(sizeof(struct linked_list));
  while(*ll)
  {
    ll=(&(*ll)->next);
  }
  buff->next=NULL;
  buff->data=data;
  *ll=buff;
}

/**
 * deallocate the memory of the access point list and set linked_l to NULL
 * @param  linked_l [OUT] : the list of access points
 * RETURN : void
 */
void free_ll (struct linked_list ** linked_l)
{
  struct linked_list * ll =*linked_l;
  struct linked_list * buff;
  while(ll)
  {
    buff=ll;
    ll=ll->next;
    free(buff);
  }
  *linked_l=NULL;
}
