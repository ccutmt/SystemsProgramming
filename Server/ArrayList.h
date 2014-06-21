/*
 * ArrayList
 */
#ifndef ARRAYLIST_H
#define ARRAYLIST_H

typedef struct
{
    int data;
}Element;

typedef struct
{
    int current;
    int size;
    Element *elements;
}ArrayList;

void        init           (ArrayList*const);
void        clean          (ArrayList*);
int         add            (ArrayList*const, Element);
void        print          (const ArrayList*const);
int         indexOf        (const ArrayList*const, Element);
int         isEmpty        (const ArrayList*const);
static void    printElement(const Element*const);
static void    resize(ArrayList* const);
//Element *removeAt(ArrayList *const list, int index);

#endif // ARRAYLIST_H
