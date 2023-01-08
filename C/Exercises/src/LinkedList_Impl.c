#include <stddef.h> // required for NULL
#include <stdio.h>
#include "linked_list.h"


struct list_node *list_append(LISTNODE head, LISTNODE_ITEM item)
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
    item->next = NULL;
    return item;
}

struct list_node *list_get_tail(LISTNODE head)
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


struct list_node *list_pop(LISTNODE head)
{
    struct list_node *current_head = *head;
    if (!current_head)
    {
        return NULL;
    }
    *head = (*head)->next;
    current_head->next = NULL;
    return current_head;
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

LISTNODE_ITEM list_remove(LISTNODE head, const char *item)
{
    if (head == NULL) return NULL;
    if (*head == NULL) return NULL; 

    LISTNODE_ITEM previous_item = NULL;
    LISTNODE_ITEM current_item = *head;
    struct string_item * current_string = (struct string_item *)current_item;
    
    while (current_string->string != item)
    {
        previous_item = current_item;
        current_item = current_item->next;
        current_string = (struct string_item *)current_item;
        if (!current_item->next) //check for end of nodes
        {
            return NULL;
        }
    }
    previous_item->next = current_item->next;
    return current_item;
}

/* EXPLANATION
*
*@head is a pointer of a pointer to the head of the list_node object/struct
*Ex. Let's imagine we have a linked list: 
*     [1|*] -> [2|*] -> [3|null]
*
*    prev = NULL ; *head points to = [1|*] ; next = null;
*    while current != null
*    FIRST ITERARION
*       next = [2|*]
*       current->next = null;
*       prev = [1|*]
*       current = [2|*]
*       --[1|null] -> [2|*] -> [3|null] 
*    SECOND ITERATION
*       next = [3|null]
*       current->next = [1|*]
*       prev = [2|*]
*       current = [3|null]
*       --[1|null] <- [2|*]  [3|null] 
*    THIRD ITERATION
*       next = null
*       current->next = [2|*]
*       prev = [3|*]
*       current = null
*       --[1|null] <- [2|*] <- [3|*] 
*     *head = [3|*] 
*/
void reverse(LISTNODE head)
{
    LISTNODE_ITEM prev = NULL;
    LISTNODE_ITEM current = *head;
    LISTNODE_ITEM next = NULL;

    while (current != NULL)
    {
        next = current->next;

        current->next = prev;

        prev = current;
        current = next;
    }
    *head = prev;    
}
