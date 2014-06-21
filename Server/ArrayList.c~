#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ArrayList.h"

void init (ArrayList *const list)
{
    list->size = 2;
    list->elements = (Element*) calloc(sizeof(Element), list->size);
    list->current = -1;
}

int add (ArrayList *const list, Element e)
{
    if (list->current+1 < list->size)
    {
    	list->elements[list->current] = e;
    	return 1;
    }
    else
    {
    	resize(list);
    	list->elements[list->current] = e;
    	return 1;
    }
}
static void resize(ArrayList* const list)
{
    int oldsize = list->size;
    list->size *= 2;
    printf("new size = %i", list->size);
    Element *newArr = (Element*) calloc(list->size, sizeof(Element));
    memcpy(newArr,list->elements, oldsize);  
    free(list->elements);
    list->elements = newArr;
}

int indexOf (const ArrayList *const list, Element e)
{
    int index = 0;
    while (index <= list->current)
    {
    if (e.data == list->elements[index].data) return index;
    index++;
    }
    return 0;
}

int isEmpty (const ArrayList *const list)
{
    return list->current == -1;
}

/*Element *removeAt(ArrayList *const list, int index)
{
    if (list->current >= index)
    {
    Element *e = &list->elements[index];
    shift(list, index, 1, LEFT);
    list->current--;    
    return e;
    }
    return NULL;
}*/

void print (const ArrayList *const list)
{
    int i;
    for (i=0; i<=list->current; i++)
    {
    	Element e = list->elements[i];
    	printElement(&e);
    }
    printf("\n");
}

void clean(ArrayList *list)
{
    free(list->elements);
}

static void printElement(const Element *const e)
{
    printf("%i ", e->data);
}



