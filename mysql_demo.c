#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mysql_util.h"

int main(void) 
{
    if (init_mysql());
        print_mysql_error(NULL);

	list_thinfo();
	if(0 == add_thinfo(	"456322", "22.22", "45.44", "2019-11-15 16:57:33"))
	{
		printf("Add item success!!\n");
		list_thinfo();
	}
	deinit_mysql();
    return EXIT_SUCCESS;
}

