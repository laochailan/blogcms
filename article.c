#include "article.h"

#include <stdlib.h>

void free_article(struct article *a)
{
	free(a->title);
	free(a->path);
	free(a->text);
	
	free(a);
}

void free_article_array(struct article **a)
{
	int i;
	for(i = 0; a[i] != NULL; i++)
		free_article(a[i]);
	
	free(a);
}