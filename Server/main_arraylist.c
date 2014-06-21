#include <stdio.h>
#include "ArrayList.h"

int main(void)
{
    
    ArrayList list;
    init(&list);
    add(&list, (Element){4});
    add(&list, (Element){6});
    print(&list);

    /*printf("Adding 3 new elements\n");
    int i;
    for (i=1; i<=4; i++)
    {
    	add(&list, (Element){i});
    }
    
    print(&list);*/
    clean(&list);
    
    return 0;
}
