#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define SetNode struct set_node
#define SetTable struct set_table
#define cString const char

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
};

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

SetNode *set_insert(SetTable *table, cString *key, const size_t key_len)
{
    size_t hash = djb33x_hash(key, key_len);
    size_t index = hash % table->hashmap_size;
    SetNode *head = table->nodes[index];
    if (!head)
    {
        table->nodes[index] = malloc(sizeof(SetNode));
        if (!table->nodes[index]) return NULL;
        table->nodes[index]->key = key;
        table->nodes[index]->key_len = key_len;
        table->nodes[index]->next = NULL;
        return table->nodes[index];
    }
    set_collisions++;
    SetNode *new_item = malloc(sizeof(SetNode));
    if (!new_item) return NULL;
    new_item->key = key;
    new_item->key_len = key_len;
    new_item->next = NULL;
    SetNode *tail = head;
    while (head)
    {
        tail = head;
        head = head->next;
        if (head->key == key) return NULL; //don't add duplicates to the table
    }
    tail->next = new_item;
    return new_item;
}

size_t get_index(SetTable *table, cString *key, const size_t key_len)
{
    size_t hash = djb33x_hash(key, key_len);
    return hash % table->hashmap_size;
}

SetNode *get_index_head(SetTable *table, cString *key, const size_t key_len)
{
    size_t hash = djb33x_hash(key, key_len);
    size_t index = hash % table->hashmap_size;
    return table->nodes[index];
}

SetNode *set_search_key(SetTable *table, cString *key, const size_t key_len)
{
    SetNode *head = get_index_head(table, key, key_len);
    if (!head) return NULL;
    if (head->key == key)
    {
        return head;
    }
    if (!head->next)
    {
        return NULL;
    }
    while(head->key != key)
    {
        head = head->next;
        if (!head->next)
        {
            return NULL; 
        }//end of cycle
    }
    return head;
}

SetNode *set_remove_key(SetTable *table, cString *key, const size_t key_len)
{
    SetNode *head = get_index_head(table, key, key_len);
    if (!head) return NULL;
    SetNode *previous_item = NULL;
    if (head->key == key)
    {
        return head;
    }
    if (!head->next)
    {
        return NULL;
    }
    while(head->key != key)
    {
        previous_item = head;
        head = head->next;
        if (!head->next)
        {
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
    set_insert(table, "Hello", hashmap_size);
    set_insert(table, "World", hashmap_size);
    set_insert(table, "test", hashmap_size);
    set_insert(table, "ok", hashmap_size);
    set_insert(table, "right", hashmap_size);

    printf("value is: %s\n", set_search_key(table, "ok", hashmap_size)->key);
    set_remove_key(table, "ok", hashmap_size);
    printf("value is: %s\n", set_search_key(table, "ok", hashmap_size)->key);
    return 0;
}