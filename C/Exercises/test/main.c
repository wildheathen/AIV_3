#include "linked_list.h"
#include "aiv-cunit.h"

CUNIT_TEST(testAppendOneItem)
//void testAppendOneItem()
{
    //1. Setup scenario
    struct string_item *my_linked_list = NULL;
    //2. Stimulate
    list_append((LISTNODE)&my_linked_list, (LISTNODE_ITEM)string_item_new("1"));
    //3. Expectation
    struct string_item *string_item = my_linked_list;
    const char* actual = string_item->string;

    const char* expect = "1";


    CUNIT_STRING_EQ(expect,actual);
}

CUNIT_TEST(testAppendTwoItems)
//void testAppendTwoItems()
{
    //1. Setup scenario
    struct string_item *my_linked_list = NULL;
    //2. Stimulate
    list_append((LISTNODE)&my_linked_list, (LISTNODE_ITEM)string_item_new("1"));
    list_append((LISTNODE)&my_linked_list, (LISTNODE_ITEM)string_item_new("2"));
    //3. Expectation
    struct string_item *string_item = my_linked_list;
    const char* actual = string_item->string;
    const char* expect = "1";

    CUNIT_STRING_EQ(expect, actual);

    string_item = (struct string_item*)string_item->node.next;
    actual = string_item->string;
    expect = "2";

   CUNIT_STRING_EQ(expect, actual);
}

CUNIT_TEST(testPopFromOneItemList)
{
    //1. Setup scenario
    struct string_item *my_linked_list = NULL;
    list_append((LISTNODE)&my_linked_list, (LISTNODE_ITEM)string_item_new("1"));   
    //2. Stimulate
    LISTNODE_ITEM popped_node = list_pop((LISTNODE)&my_linked_list);
    struct string_item* string_item = (struct string_item*)popped_node;
    //3. Expectation
    CUNIT_STRING_EQ("1", string_item->string);

    CUNIT_IS_NULL(my_linked_list);
}

CUNIT_TEST(testPopFromEmptyList)
{
    //1. Setup scenario
    struct string_item *my_linked_list = NULL; 
    //2. Stimulate
    LISTNODE_ITEM popped_node = list_pop((LISTNODE)&my_linked_list);
    //3. Expectation
    CUNIT_IS_NULL(popped_node);
    CUNIT_IS_NULL(my_linked_list);
}

CUNIT_TEST(testPopFromTwoItemList)
{
    //1. Setup scenario
    struct string_item *my_linked_list = NULL;
    list_append((LISTNODE)&my_linked_list, (LISTNODE_ITEM)string_item_new("1"));   
    list_append((LISTNODE)&my_linked_list, (LISTNODE_ITEM)string_item_new("2"));  
    //2. Stimulate
    LISTNODE_ITEM popped_node = list_pop((LISTNODE)&my_linked_list);
    struct string_item* string_item = (struct string_item*)popped_node;
    //3. Expectation
    CUNIT_STRING_EQ("1", string_item->string);
    CUNIT_STRING_EQ("2", my_linked_list->string);
}

CUNIT_TEST(testRemoveFromEmptyList)
{
    //1. Setup scenario
    struct string_item *my_linked_list = NULL;

    delete_N((LISTNODE)&my_linked_list, 1); 
    
       
}

CUNIT_RUNNER(
    testAppendOneItem, testAppendTwoItems,
    testPopFromOneItemList, testPopFromEmptyList, testPopFromTwoItemList
);

/*
    struct string_item * my_linked_list = NULL;
    list_append((LISTNODE)&my_linked_list, (LISTNODE_ITEM)string_item_new("1"));
    list_append((LISTNODE)&my_linked_list, (LISTNODE_ITEM)string_item_new("2"));
    list_append((LISTNODE)&my_linked_list, (LISTNODE_ITEM)string_item_new("3"));
    list_append((LISTNODE)&my_linked_list, (LISTNODE_ITEM)string_item_new("4"));
    struct string_item *string_item = my_linked_list;

    //deleteN((struct list_node **)&my_linked_list, 7);
    reverse((LISTNODE)&string_item);

    delete_N((LISTNODE)&string_item, 2);

    while (string_item)
    {
        printf("%s\n", string_item->string);
        string_item = (struct string_item *)string_item->node.next;
    }
*/