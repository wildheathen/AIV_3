#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define StringItem struct set_string_item
#define IntItem struct set_int_item
#define SetNode struct set_node
#define SetTable struct set_table
#define cString const char

SetNode *new_node(SetNode *node, cString *key, size_t key_len);
StringItem *string_item_new(cString *key, cString *string, size_t key_len);
//SetNode *get_index_head(SetTable *table, cString *key, const size_t key_len);

int set_collisions = 0;

size_t djb33x_hash(cString *key, const size_t keylen)
{
    size_t hash = 5381;
    for (size_t i = 0; i < keylen; i++)
    {
    hash = ((hash << 5) + hash) ^ key[i];
    }
    return hash;
}

struct set_node
{
    SetNode *next;
    cString *key;
    size_t key_len;
    size_t value;
};

struct set_string_item
{
    SetNode *node;
    cString *key;
    size_t key_len;
    cString *value;
};

StringItem *string_item_new(cString *key, cString *value, size_t key_len)
{
    //printf("0_a\n");
    StringItem *item = malloc(sizeof(StringItem));
    if (!item) return NULL;
    //printf("1_a\n");
    item->node = malloc(sizeof(SetNode));
    item->node->key = key;
    item->node->key_len = key_len;
    item->key = key;
    item->key_len = key_len;
    item->value = value;
    return item;
}

struct set_int_item
{
    SetNode *node;
    cString *key;
    size_t key_len;
    int value;
};

IntItem *int_item_new(cString *key, int value, size_t key_len)
{   
    //printf("0_b\n");
    IntItem *item = malloc(sizeof(IntItem));
    if(!item) return NULL;
    //printf("1_b\n");
    item->node = malloc(sizeof(SetNode));
    item->node->key = key;
    item->node->key_len = key_len;
    item->key_len = key_len;
    item->value = value;
    item->key = key;
    return item;
}

struct set_table
{
    SetNode **nodes;
    size_t hashmap_size ;
};

SetTable *set_table_new (const size_t hashmap_size )
{
    SetTable *table = malloc(sizeof(SetTable));
    if (!table) return NULL;
    table->hashmap_size = hashmap_size ;
    table->nodes = calloc(table->hashmap_size , sizeof(SetNode *));
    if (!table->nodes)
    {
    free(table);
    return NULL;
    }
    return table;
}

SetNode *set_insert(SetTable *table, SetNode *node)
{   
    printf("storing key: %s\n", node->key);
    //printf("2\n");
    size_t hash = djb33x_hash(node->key, node->key_len);
    size_t index = hash % table->hashmap_size;
    printf("stored index is: %llu\n", index);
    SetNode *head = table->nodes[index];
    if (!head)
    {
        //printf("3_a\n");
        table->nodes[index] = malloc(sizeof(SetNode));
        if (!table->nodes[index]) return NULL;
        //printf("4_a\n\n");
        table->nodes[index]->key = node->key;
        table->nodes[index]->key_len = node->key_len;
        table->nodes[index]->value = node->value;
        table->nodes[index]->next = NULL;
        return table->nodes[index];
    }
    //printf("3_b\n");
    set_collisions++;
    SetNode *new_item = malloc(sizeof(SetNode));
    if (!new_item) return NULL;
    //printf("3_bb\n");
    new_item->key = node->key;
    new_item->key_len = node->key_len;
    new_item->value = node->value;
    new_item->next = NULL;
    SetNode *tail = head;
    if (!head->next)
    {
        head->next = new_item;
        //printf("4_b\n\n");
        return new_item;
    }
    while (head)
    {
        tail = head;
        head = head->next;
        if (head->key == node->key) return NULL; //don't add duplicates to the table
    }
    tail->next = new_item;
    //printf("4_b\n\n");
    return new_item;
}

SetNode *get_index_head(SetTable *table, cString *key, const size_t key_len)
{
    size_t hash = djb33x_hash(key, key_len);
    size_t index = hash % table->hashmap_size;
    printf("found head at index: %llu\n", index);
    return table->nodes[index];
}

SetNode *set_search_key(SetTable *table, cString *key, const size_t key_len)
{
    printf("searching for: %s\n", key);
    SetNode *head = get_index_head(table, key, key_len);
    if (!head) return NULL;
    if (head->key == key)
    {
        printf("head is already the right item\n\n");
        return head;
    }
    if (!head->next)
    {
        printf("nvm, start has no next\n\n");
        return NULL;
    }
    while(head->key != key)
    {
        head = head->next;
        if (!head->next)
        {
            printf("nvm, cycle found nothing\n\n");
            return NULL; 
        }//end of cycle
    }
    printf("found key: %s\n, value: %llu", head->key, head->value);
    return head;
}

SetNode *set_remove_key(SetTable *table, cString *key, const size_t key_len)
{
    printf("removing at: %s\n", key);
    SetNode *head = get_index_head(table, key, key_len);
    SetNode *previous_item = NULL;
    if (!head) return NULL;
    if (head->key == key)
    {
        printf("head is already the right item\n\n");
        return head;
    }
    if (!head->next)
    {
        printf("nvm, start has no next\n\n");
        return NULL;
    }
    while(head->key != key)
    {
        previous_item = head;
        head = head->next;
        if (!head->next)
        {
            printf("nvm, cycle found nothing\n\n");
            return NULL; 
        }//end of cycle
    }
    previous_item->next = head->next;
    head->next = NULL;
    return head;
}

int main()
{
    const int hashmap_size = 10;
    SetTable *table = set_table_new(hashmap_size);
    set_insert(table, (SetNode *)string_item_new("key1", "Hello", hashmap_size));
    set_insert(table, (SetNode *)string_item_new("key2", "World", hashmap_size));
    set_insert(table, (SetNode *)string_item_new("key3", "Prime", hashmap_size));
    set_insert(table, (SetNode *)string_item_new("key4", "Octavia", hashmap_size));
    
    set_insert(table, (SetNode *)int_item_new("key5", 45, hashmap_size));
    set_insert(table, (SetNode *)int_item_new("key6", 234, hashmap_size));


    StringItem *string_item = malloc(sizeof(StringItem));
    IntItem *int_item = malloc(sizeof(IntItem));
    string_item = (StringItem *)set_search_key(table, "key1", hashmap_size);
    int_item = (IntItem *)set_search_key(table, "key5", hashmap_size);
    printf("string_item->key: %s, string_item->value: %s\n", string_item->key, string_item->value);
    printf("int_item->key: %s, int_item->value: %d\n", int_item->key, int_item->value);
    return 0;
}