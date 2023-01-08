#include <stddef.h> // required for NULL
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#include "C:\Users\david\Documents\AIV_3\C\Exercises\LinkedList_Impl.c"

#define LISTNODE struct list_node **
#define LISTNODE_ITEM struct list_node *

struct list_node *list_get_tail(struct list_node **head);

struct list_node
{
    struct list_node *prev; //DoublyLinkedList
    struct list_node *next;
};

struct string_item
{
    struct list_node node;
    const char *string;
};

struct list_node *list_append(struct list_node **head, struct list_node *item)
{
    struct list_node *tail = list_get_tail(head);
    if (!tail)
    {
        *head = item;
    }
    else
    {
        tail->next = item;
    }
    item->prev = tail;
    item->next = NULL;
    return item;
}

struct list_node *list_get_tail(struct list_node **head)
{
    struct list_node *current_node = *head;
    struct list_node *last_node = NULL;
    while (current_node)
    {
        last_node = current_node;
        current_node = current_node->next;
    }
    return last_node;
}

struct string_item *string_item_new(const char *string)
{
    struct string_item *item = malloc(sizeof(struct string_item));
    if (!item)
    {
        return NULL;
    }
    item->string = string;
    return item;
}

void insert_after(LISTNODE head_ref, LISTNODE tail_ref, const char *value, int position)
{    
  // Create the new node
  struct string_item * new_node = malloc(sizeof(struct string_item));
  new_node->string = value;

  // Check if the doubly linked list is empty
  if (*head_ref == NULL) {
    *head_ref = &new_node->node;
    *tail_ref = &new_node->node;
    return;
  }

  // Find the node at the given position
  struct string_item* current = (struct string_item *)head_ref;
  int i = 0;
  while (i < position && current->node.next != NULL) {
    current = (struct string_item *)current->node.next;
    //printf("%s\n",current->string);
    i++;
  }

  // Set the next and previous pointers of the new node
  new_node->node.next = current->node.next;
  new_node->node.prev = &current->node;

  // Set the previous pointer of the node after the new node to point to the new node
  if (current->node.next != NULL) {
    current->node.next->prev = &new_node->node;
  }

  // Set the next pointer of the node before the new node to point to the new node
  current->node.next = &new_node->node;

}
/*
*It deletes the node at the n-th position, scanning the list of nodes one by one O(n)
*@head is a pointer of a pointer to the head of the list_node object/struct
*@position is the position of the element trying to be removed
*/
void delete_N(LISTNODE head, int position)
{

    struct list_node* temp;
    struct list_node* prev;
    temp = *head;
    prev = *head;

    for (int i = 0; i < position; i++)
    {
        //if we are trying to remove the first node of the list
        if (i == 0 && position == 1)
        {
            //assign the head to the next value (What if it's only one element?)
            *head = (*head)->next;
            //free the temp list_node in order to remove the (past) head element
            free(temp);
        }
        else //if position is not 1
        {
            //if we find the element we want to delete
            if( i == position-1 && temp)
            {
                //since we are about do delete the current item
                // we have to change the reference to the next item of the previous item
                // to the current item's next element
                prev->next = temp->next;
                free(temp);    
            }
            else
            {
                //my previous node becomes the actual node
                prev = temp;

                if(prev == NULL)
                    break;
                //my temp node becomes the next node
                temp = temp->next;
            }
        }
    }
}

void insert_before(LISTNODE head_ref, LISTNODE tail_ref, const char *value, int position) {
  // Create the new node
  struct string_item * new_node = malloc(sizeof(struct string_item));
  new_node->string = value;

  // Check if the doubly linked list is empty
  if (*head_ref == NULL) {
    *head_ref = &new_node->node;
    *tail_ref = &new_node->node;
    return;
  }

  // Find the node at the given position
  struct string_item* current = (struct string_item *)head_ref;
  int i = 0;
  while (i < position && current->node.next != NULL) {
    current = (struct string_item *)current->node.next;
    //printf("%s\n",current->string);
    i++;
  }

  // Set the next and previous pointers of the new node
  new_node->node.next = &current->node;
  new_node->node.prev = current->node.prev;

  // Set the previous pointer of the node after the new node to point to the new node
  if (current->node.prev != NULL) {
    current->node.prev->next = &new_node->node;
  }

  // Set the next pointer of the node before the new node to point to the new node
  current->node.prev = &new_node->node;
}

void shuffle(LISTNODE head_ref, LISTNODE tail_ref) {
  // Check if the doubly linked list is empty or has only one node
  if (*head_ref == NULL || (*head_ref)->next == NULL) {
    return;
  }

  // Initialize the current node to be the head of the list
  struct string_item* current = (struct string_item *)(*head_ref)->next;

  // Iterate over the nodes in the list from the head to the tail
  while (current != NULL) {
    // Generate a random number between 0 and the number of remaining nodes, including the current node
    int num_remaining = 0;
    struct string_item* temp = current;
    while (temp != NULL) {
      num_remaining++;
      temp = (struct string_item *)temp->node.next;
    }
    int index = rand() % num_remaining;

    // Find the node at the randomly-generated index
    temp = current;
    int i = 0;
    while (i < index && temp->node.next != NULL) {
      temp = (struct string_item *)temp->node.next;
      i++;
    }

    // Swap the data of the current node with the data of the node at the randomly-generated index
    const char * temp_data = temp->string;
    temp->string = current->string;
    current->string = temp_data;

    // Set the current node to be the next node in the list
    current = (struct string_item *)current->node.next;
  }
}

int main()
{
    struct string_item *Node = NULL;

    list_append((struct list_node **)&Node, (struct list_node *)string_item_new("1"));
    list_append((struct list_node **)&Node, (struct list_node *)string_item_new("2"));
    list_append((struct list_node **)&Node, (struct list_node *)string_item_new("3"));
    list_append((struct list_node **)&Node, (struct list_node *)string_item_new("4"));
    struct string_item *string_item = Node;

    //delete_N((struct list_node **)&string_item, 2);
    //insert_before((struct list_node **)&string_item, (struct list_node **)list_get_tail((struct list_node **)&string_item), "7", 2);
    //insert_after((struct list_node **)&string_item, (struct list_node **)list_get_tail((struct list_node **)&string_item), "7", 2);

    srand((int)time(NULL));
    shuffle((struct list_node **)&string_item, (struct list_node **)list_get_tail((struct list_node **)&string_item));

    while (string_item)
    {
        printf("%s\n", string_item->string);
        string_item = (struct string_item *)string_item->node.next;
    }
}