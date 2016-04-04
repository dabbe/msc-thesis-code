#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/timeb.h>

#include "ezxml.h"

int max = -1;

void traverse_tree(ezxml_t* f1, int depth)
{
	ezxml_t node = *f1;
	if(depth > max){
		max = depth;
	}

		
	if(node->child != NULL)
	{
		traverse_tree(&(node->child), depth + 1);
	}

	if(node->ordered != NULL){
		traverse_tree(&(node->ordered), depth);
	}
}

void main(int argc, char *argv[])
{
	if(argc != 2)
	{
		printf("Usage: ./%s [file]\n", __FILE__);
		exit(1);
	}

  	printf("Max before: %d", max);

	ezxml_t f1 = ezxml_parse_file(argv[1]);

  	traverse_tree(&f1, 0);

  	printf("Max after: %d", max);



	ezxml_free(f1);
}

