#include "article.h"

#define ARTICLEDIR "articles/"

void parse_article(struct article*);
struct article **read_dir();

void read_content(struct article*);

struct article **read_articles();