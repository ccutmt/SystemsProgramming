#include <stdio.h>
#include "ArrayList.h"

int main(void)
{
    
    	/*
	 * Test ArrayList
	 */
	ArrayList* x = malloc(sizeof(ArrayList));
	init(x);
	int i;
	for(i = 1; i <= 20; i++){
		add(x, (Element){i});
	}
	print(x);
	removeAt(x, 19);
	print(x);
    
    return 0;
}
