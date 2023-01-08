#include <stdlib.h>

#define LISTNODE struct list_node **
#define LISTNODE_ITEM struct list_node *

struct list_node
{
    //struct list_node *prev; //DoublyLinkedList
    struct list_node *next;
};

struct string_item
{
    struct list_node node;
    const char *string;
};

struct list_node *list_get_tail(LISTNODE head);
struct list_node *list_append(LISTNODE head, LISTNODE_ITEM item);
struct string_item *string_item_new(const char *string);
struct list_node *list_pop(LISTNODE head);
void delete_N(LISTNODE head, int position);
