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
void        add            (ArrayList*const, Element);
void        print          (const ArrayList*const);
int         indexOf        (const ArrayList*const, Element);
int         isEmpty        (const ArrayList*const);
void    printElement(const Element*const);
void    resize(ArrayList* const);
void    removeAt(ArrayList *const list, int index);

#endif // ARRAYLIST_H
