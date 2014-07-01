#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ArrayList.h"

void initArrayList (ArrayList *const list)
{
	list->size = 2;
	list->elements = (void *) calloc(list->size, sizeof(void *));
	list->current = -1;
}

void add (ArrayList *const list, void * e)
{
	if (list->current < list->size-1)
	{
		list->current++;
		list->elements[list->current] = e;
	}
	else
	{
		resize(list);
		list->current++;
		list->elements[list->current] = e;
	}
}

void resize(ArrayList* const list)
{
	list->size *= 2;
	void *newArr = malloc(sizeof(void *) * list->size);
	memcpy(newArr,list->elements, (list->current+1) * sizeof(void *));
	free(list->elements);
	list->elements = newArr;
}

int isEmpty (const ArrayList *const list)
{
	return list->current == -1;
}

void removeAt(ArrayList *const list, int index)
{
	if (index <= list->current)
	{
		memmove(&list->elements[index],&list->elements[index+1], ((list->current+1) - index) * sizeof(void *));
		list->current--;
	}
}

void freeAll(ArrayList *list)
{
	int i = 0;
 	for(i = 0; i <= list->current; i++)
        	free(getElement(list,i));
}

void * getElement (ArrayList *const list, int index)
{
    if (index <= list->current)
    {
    	return list->elements[index];
    }
    
    return NULL;
}
