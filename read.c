#include "read.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <time.h>
#include <dirent.h>
#include <sys/stat.h>
#include <assert.h>

void parse_article(struct article *article)
{
	int i;
	int i2 = 0;
	
	int destsize = strlen(article->text) + 1;
	char *dest = malloc(destsize);
	
	strcpy(dest, "");
	
	for(i = 0; i < strlen(article->text); i++) {
		char ch = article->text[i];
		if(ch == '[' && strncmp(&article->text[i]+1, "img", 3) == 0) {
			const char *beg = &article->text[i]+4;
			const char *mid = strchr(beg+1, ' ');
			const char *end = strchr(mid, ']');
			
			const int imgsize = mid - beg - 1;
			const int descsize = end - mid - 1;
			
			char *image = malloc(imgsize + 1);								
			char *desc = malloc(descsize + 1);
			char *buf;
			
			image[imgsize] = '\0';
			desc[descsize] = '\0';
			
			strncpy(image, beg+1, imgsize);
			strncpy(desc, mid+1, descsize);
			
			const char format[] = { "<div class=\"imagebox\"><img class=\"image\" src=\"images/%s\" alt=\"%s\" /><br />%s</div>" };
			
			int bufsize = strlen(format) - 6 + strlen(image) + 2*strlen(desc); // - 3*2 because of 3*"%s"
			buf = malloc(bufsize + 1);
			
			snprintf(buf, bufsize + 1, format, image, desc, desc);
			
			dest = realloc(dest, destsize += strlen(buf) + 1);
			assert(dest != NULL);
			
			strcat(dest, buf);
			
			i += end - &article->text[i];
			i2 += strlen(buf);
			
			free(desc);
			free(image);
			free(buf);
		} else if(ch == '\n') {
			const char src[] = { "<br />" };
			dest = realloc(dest, destsize += sizeof(src));
			strncpy(dest + i2, src, sizeof(src));
			i2 += strlen(src);			
		} else {
			dest[i2++] = ch;
		}
	}
	
	free(article->text);
	article->text = dest;
}

int compare_ctime(const struct article **a1, const struct article **a2) {
	if((*a1)->ctime > (*a2)->ctime)
		return -1;
	else if((*a1)->ctime < (*a2)->ctime)
		return 1;
	else
		return 0;
}

struct article **read_dir(DIR *dir)
{
	struct article **temp = NULL;
	
	int size = 0;
	
	struct dirent *file;
	
	while((file = readdir(dir)) != 0) {
		if(strcmp(file->d_name, ".") == 0 || strcmp(file->d_name, "..") == 0) continue;
		
		temp = realloc(temp, ++size * sizeof(struct article *));
		temp[size - 1] = malloc(sizeof(struct article));
		temp[size - 1]->path = malloc(strlen(ARTICLEDIR) + strlen(file->d_name) + 1);
		
		sprintf(temp[size - 1]->path, "%s%s", ARTICLEDIR, file->d_name);
	}
	
	int i;
	struct stat *stats = calloc(size, sizeof(struct stat));
	
	for(i = 0; i < size; i++) {
		stat(temp[i]->path, stats + i);
		temp[i]->ctime = stats[i].st_ctime;
	}
		
	qsort(temp, size, sizeof(struct article *), compare_ctime);
	
	free(stats);
	
	temp = realloc(temp, (size+1)*sizeof(struct article *));
	temp[size] = NULL; // terminating
	
	return temp;
}

void read_content(struct article *article)
{
	FILE *file;
	
	if((file = fopen(article->path, "r")) == NULL)
		err(EXIT_FAILURE, "Error opening \"%s\"", article->path);	
	
	article->title = malloc(BUFSIZE);
	fgets(article->title, BUFSIZE, file);
	article->title[strlen(article->title)-1] = '\0'; // remove '\n'
	
	int i = 0, ch;
	int reserved = 1024;
	article->text = malloc(reserved);
	while((ch = fgetc(file)) != EOF) {
		if(i >= reserved)
			article->text = realloc(article->text, reserved += 16);
		article->text[i++] = ch;
	}		
	
	if(i >= reserved) article->text = realloc(article->text, reserved += 1);
	article->text[i] = '\0';
	
	fclose(file);
}

struct article **read_articles()
{	
	DIR *articledir = opendir(ARTICLEDIR);
	if(articledir == NULL)
		err(EXIT_FAILURE, "Entrydir could not be opened");
	
	struct article **articles = read_dir(articledir);
	
	int i;	
	for(i = 0; articles[i] != NULL; i++) {	
		read_content(articles[i]);
		parse_article(articles[i]);
	}
		
	closedir(articledir);
	
	return articles;
}