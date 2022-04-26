#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "RLEList.h"
#include "AsciiArtTool.h"
#define ONE_CHARACTER 1
#define EMPTY_CHARACTER '\0'

RLEList asciiArtRead(FILE* in_stream)
{
    RLEList list = RLEListCreate();
    char currentCharacter[ONE_CHARACTER]= "";
    while(fgets(currentCharacter, ONE_CHARACTER, in_stream))
    {
        RLEListAppend(list, *currentCharacter);
    }
    fclose(in_stream);
    return list;
}

RLEListResult asciiArtPrint(RLEList list, FILE *out_stream)
{
    if(!list)
    {
        return RLE_LIST_NULL_ARGUMENT;
    }
    int len = RLEListSize(list);
    char* unCompressedText = malloc(len*sizeof(*unCompressedText));
    if(!unCompressedText)
    {
        return RLE_LIST_OUT_OF_MEMORY;
    }
    for(int i=0; i<len; i++)
    {
        unCompressedText[i] = RLEListGet(list, i, RLE_LIST_SUCCESS);
    }
    fputs(unCompressedText,out_stream);
    fclose(out_stream);
    free(unCompressedText);
    return RLE_LIST_SUCCESS;
}

RLEListResult asciiArtPrintEncoded(RLEList list, FILE *out_stream)
{
    if(!list)
    {
        return RLE_LIST_NULL_ARGUMENT;
    }
    char *RLEListString = RLEListExportToString(list,RLE_LIST_SUCCESS);
    if(!RLEListString)
    {
        return RLE_LIST_NULL_ARGUMENT;
    }
    fputs(RLEListString, out_stream);
    fclose(out_stream);
    free(RLEListString);
    return RLE_LIST_SUCCESS;
}
