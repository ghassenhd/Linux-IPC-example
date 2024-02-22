#ifndef LINKED_LIST_HEADER
#define LINKED_LIST_HEADER

#define MAX_SSID_LEN        (32 )

struct ll_data
{
  char ssid[MAX_SSID_LEN+1];
  int snr;
  int channel;
};


struct linked_list
{
   struct ll_data        data;
   struct linked_list *  next;
};

void display_modified(struct ll_data old_data,struct ll_data new_data);

int find_ll (struct ll_data data, struct linked_list * ll);

int delete_ll_index (int index, struct linked_list ** ll);

void add_ll_last(struct ll_data data, struct linked_list ** ll);

void free_ll (struct linked_list ** linked_l);

#endif
