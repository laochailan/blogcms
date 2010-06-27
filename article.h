#ifndef ARTICLE_H
#define ARTICLE_H

#include <time.h>

#define BUFSIZE 4096

struct article {
	char *path;
	char *title;
	time_t ctime;
	char *text;
};

void free_article(struct article *a);
void free_article_array(struct article **a);

#endif