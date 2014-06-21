#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ArrayList.h"

void init (ArrayList *const list)
{
	list->size = 2;
	list->elements = (Element*) calloc(list->size, sizeof(Element));
	list->current = -1;
}

void add (ArrayList *const list, Element e)
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
	Element *newArr = (Element*) malloc(sizeof(Element) * list->size);
	memcpy(newArr,list->elements, (list->current+1) * sizeof(Element));
	free(list->elements);
	list->elements = newArr;
}

int indexOf (const ArrayList *const list, Element e)
{
	int index = -1;
	int counter = 0;
	while (counter <= list->current)
	{
		if (e.data == list->elements[counter].data)
			return counter;
		counter++;
	}
	return index;
}

int isEmpty (const ArrayList *const list)
{
	return list->current == -1;
}

void removeAt(ArrayList *const list, int index)
{
	if (index <= list->current)
	{
		memmove(&list->elements[index],&list->elements[index+1], ((list->current+1) - index) * sizeof(Element));
		list->current--;
	}
}

void clean(ArrayList *list)
{
	free(list->elements);
}

void print (const ArrayList *const list)
{
	int i;
	for (i=0; i<= list->current; i++)
	{
		Element e = list->elements[i];
		printElement(&e);
	}
	printf("\n");
}

void printElement(const Element *const e)
{
	printf("%i ", e->data);
}



