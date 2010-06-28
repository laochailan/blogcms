#include "article.h"

#define ARTICLEDIR "articles/"

char *parse_img(char *dest, char *src, int *destsize, int *destit, int *srcit);
char *parse_link(char *dest, char *src, int *destsize, int *destit, int *srcit);

void parse_article(struct article*);
struct article **read_dir();

void read_content(struct article*);

struct article **read_articles();