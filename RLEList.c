#include "RLEList.h"
#include <stdlib.h>

#define EMPTY_CHARACTER '\0'
#define ZERO_TIMES 0
#define A_NULL_POINTER -1

struct RLEList_t
{
    char character;
    int times;
    struct RLEList_t* next_node;
};

/**
* RLEListCreate: Allocates a new empty RLE list.
*
* The function allocates and returns a new RLEList struct containing a list with no characters.
* Note that the user must free the returned list using RLEListDestroy() once it is no longer needed.
*
* @return
* 	NULL if allocations failed.
* 	A new RLEList in case of success.
*/
RLEList RLEListCreate()
{
    RLEList rle_list = malloc(sizeof(*rle_list));
    if(!rle_list)
    {
        return NULL;
    }
    rle_list->character = EMPTY_CHARACTER;
    rle_list->times = ZERO_TIMES;
    rle_list->next_node = NULL;
    return rle_list;
}

/**
* RLEListDestroy: Deallocates an existing RLE list.
*
* @param list - RLE list to be deallocated. If RLE list is NULL nothing will be done
*/
void RLEListDestroy(RLEList list)
{
    if(!list)
        return;
    RLEList tobedeleted = list;
    while(list)
    {
        tobedeleted = list;
        list = list->next_node;
        free(tobedeleted);
    }
}

/**
*   RLEListAppend: add a specified character at the end of the list.
*
* @param list - The RLE list for which to add the character
* @param value - The character which needs to be added.
* @return
* 	RLE_LIST_NULL_ARGUMENT if a NULL was sent as one of the parameters
* 	RLE_LIST_OUT_OF_MEMORY if an allocation failed
* 	RLE_LIST_SUCCESS if the character has been inserted successfully
*/
RLEListResult RLEListAppend(RLEList list, char value)
{
    if(!list)
    {
        return RLE_LIST_NULL_ARGUMENT;
    }
    if(!(list->times))
    {
        list->character = value;
        (list->times)++;
        return RLE_LIST_SUCCESS;
    }
    while(list->next_node)
    {
        list = list->next_node;
    }
    if(list->character == value)
    {
        (list->times)++;
        return RLE_LIST_SUCCESS;
    }
    else
    {
        RLEList newnode = RLEListCreate();
        if(!newnode)
        {
            return RLE_LIST_OUT_OF_MEMORY;
        }
        newnode->character = value;
        (newnode->times)++;
        list->next_node = newnode;
        return RLE_LIST_SUCCESS;
    }

}

/**
* RLEListSize: Returns the total number of characters in an RLE list.
* @param list - The RLE list whose size is requested
* @return
* 	-1 if a NULL pointer was sent.
* 	Otherwise the total number of characters in the list.
*/
int RLEListSize(RLEList list)
{
    if(!list)
    {
        return A_NULL_POINTER;
    }
    else
    {
        int sum = 0;
        while(list)
        {
            sum += list->times;
            list = list->next_node;
        }
        return sum;
    }
}

/**
*   RLEListRemove: Removes a character found at a specified index in an RLE list.
*
* @param list - The RLE list to remove the character from.
* @param index - The index at which the character to be removed is found.
* @return
* 	RLE_LIST_NULL_ARGUMENT if a NULL was sent to the function.
* 	RLE_LIST_INDEX_OUT_OF_BOUNDS if given index is not withing the list's bounds.
* 	RLE_LIST_SUCCESS the character found at index has been removed successfully.
*/
RLEListResult RLEListRemove(RLEList list, int index)
{
    if(!list)
    {
        return RLE_LIST_NULL_ARGUMENT;
    }
    if(index < 0 || index > RLEListSize(list))
    {
        return RLE_LIST_INDEX_OUT_OF_BOUNDS;
    }
    RLEList ptr = list;
    RLEList previous = ptr;
    int currentappears = ZERO_TIMES;
    while( index > currentappears)
    {
        currentappears+=list->times;
        previous = ptr;
        ptr = ptr->next_node;
    }
    (ptr->times)--;
    if (!(ptr->times))
    {
        previous->next_node = ptr->next_node;
        free(ptr);
    }
    if((previous->character) == (previous->next_node->character))
    {
        previous->times+=previous->next_node->times;
        RLEList tempPtr = previous->next_node;
        previous->next_node = previous->next_node->next_node;
        free(tempPtr);
    }
    return RLE_LIST_SUCCESS;
}

/**
*   RLEListGet: Returns the character found at a specified index in an RLE list.
*
* @param list - The RLE list to retrieve the character from.
* @param index - The index at which the character to be retrieved is found.
* @param result - Pointer to be used to store the result of the operation, if it is NULL, the result will not be saved.
* 	RLE_LIST_NULL_ARGUMENT if a NULL was sent to the function as list.
* 	RLE_LIST_INDEX_OUT_OF_BOUNDS if given index is not withing the list's bounds.
* 	RLE_LIST_SUCCESS the character found at index has been retrieved successfully.
* @return
* 	0 if result is not RLE_LIST_SUCCESS.
* 	The character found at given index in case of success.
*/
char RLEListGet(RLEList list, int index, RLEListResult *result)
{
    //*result = RLE_LIST_SUCCESS;
    if(!list)
    {
        if(result)
        {
            *result = RLE_LIST_NULL_ARGUMENT;
        }
    }
    if(index < 0 || index > RLEListSize(list))
    {
        if(result)
        {
            *result =RLE_LIST_INDEX_OUT_OF_BOUNDS;
        }
    }
    int currentappears = ZERO_TIMES;
    while( index > currentappears)
    {
        currentappears+=list->times;
        if(list->next_node)
            list = list->next_node;
    }
    *result = RLE_LIST_SUCCESS;
    if(*result == RLE_LIST_SUCCESS)
    {
        return list->character;
    }
    else
    {
       return EMPTY_CHARACTER;
    }
}

/**
*   RLEListExportToString: Returns the characters found in an RLE list as a string.
*
* @param list - The RLE list to be exported as a string.
* @param result - Pointer to be used to store the result of the operation, if it is NULL, the result will not be saved.
* 	RLE_LIST_NULL_ARGUMENT if a NULL was sent to the function as list.
* 	RLE_LIST_SUCCESS the RLE list has been successfuly exported as a string.
* @return
* 	NULL if result is not RLE_LIST_SUCCESS.
* 	The string that corresponds to the received RLE list.
*/
char* RLEListExportToString(RLEList list, RLEListResult* result)
{
    //*result = RLE_LIST_SUCCESS;
    if(!list)
    {
        if(result)
        {
            *result = RLE_LIST_NULL_ARGUMENT;
        }
    }
    int len = RLEListSize(list);
    char* rleTotalString = (char*)malloc(len*sizeof(char));
    if(!rleTotalString)
    {
        *result = RLE_LIST_ERROR;
    }
    char* ptr =rleTotalString;
    while(list)
    {
        char currentchar = list->character;
        for(int i=0; i<list->times; i++)
        {

            *ptr = currentchar;
             ptr++;
        }
        list = list->next_node;
    }
    if(*result == RLE_LIST_SUCCESS)
    {
        return rleTotalString;
    }
    else
    {
       return NULL;
    }
}

/**
*   RLEListMap: Change the given RLE list's characters according to the received mapping function.
*               This function replaces each character of the give RLE list with its mapped character.
*
* @param list - The RLE list to edit.
* @param MapFunction - Pointer to a function of type MapFunction.
* @return
* 	RLE_LIST_NULL_ARGUMENT if a NULL was sent as a paramater.
* 	RLE_LIST_SUCCESS if the mapping is done successfully.
*/
RLEListResult RLEListMap(RLEList list, MapFunction map_function)
{
     if((!list) || (!map_function))
    {
            return RLE_LIST_NULL_ARGUMENT;
    }
    while(list)
    {
        char currentCharacter = list->character;
        int numOfAppears = list->times;
        while(numOfAppears)
        {
            list->character = map_function(currentCharacter);
            numOfAppears--;
        }
        list = list->next_node;
    }
    return RLE_LIST_SUCCESS;
}
