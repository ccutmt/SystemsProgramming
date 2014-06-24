/*
 * ArrayList
 */
#ifndef ARRAYLIST_H
#define ARRAYLIST_H

typedef struct
{
    int current;
    int size;
    void **elements;
}ArrayList;

void	initArrayList (ArrayList*const);
void	add (ArrayList*const, void * e);
int	isEmpty (const ArrayList*const);
void    resize(ArrayList* const);
void    removeAt(ArrayList *const list, int index);
void 	freeAll(ArrayList *list);
void* 	getElement (ArrayList *const list, int index);

#endif // ARRAYLIST_H
