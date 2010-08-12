/*
This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor,
Boston, MA  02110-1301, USA.

---
Copyright (C) 2010, Lukas Weber <laochailan@web.de>
*/

#include "read.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <time.h>
#include <dirent.h>
#include <sys/stat.h>
#include <assert.h>

void parse_img(char **dest, char *src, int *destsize, int *destit, int *srcit) {
	char *newdest;
	
	const char *beg = &src[*srcit];
	
	while(*beg != ' ') beg++; // jump over the "[img"
	while(*beg == ' ') beg++;
	
	const char *mid = strchr(beg, ' ');
	const char *end = strchr(mid, ']');
	
	assert(beg && mid && end);
		
	while(*mid == ' ') mid++;
	
	const int imgsize = mid - beg;
	const int descsize = end - mid;
	
	char *image = malloc(imgsize + 1);								
	char *desc = malloc(descsize + 1);
	char *buf;
	
	image[imgsize] = '\0';
	desc[descsize] = '\0';
	
	strncpy(image, beg, imgsize);
	strncpy(desc, mid, descsize);
	
	const char format[] = { "<div class=\"imagebox\"><img class=\"image\" src=\"images/%s\" alt=\"%s\" /><br />%s</div>" };
	
	int bufsize = strlen(format) - 6 + strlen(image) + 2*strlen(desc) + 1; // - 3*2 because of 3*"%s"
	buf = malloc(bufsize + 1);
	
	snprintf(buf, bufsize, format, image, desc, desc);
	newdest = realloc(*dest, *destsize += bufsize + 1);
	assert(newdest != NULL);
	
	strncpy(newdest + *destit, buf, bufsize);
	
	(*srcit) += end - &src[*srcit];
	(*destit) += strlen(buf);
	
	free(desc);
	free(image);
	free(buf);
	
	*dest = newdest;
}

void parse_link(char** dest, char *src, int *destsize, int *destit, int *srcit) {
	char *newdest;
	
	const char *beg = &src[*srcit]; 
	const char *mid = strchr(beg, '|');
	const char *end = strchr(mid, ']');
	
	while(*beg != ' ') beg++; // jump over the "[link"
	while(*beg == ' ') beg++;
	while(*mid == ' ') mid++;

	assert(beg && mid && end);
	
	const int addrsize = mid - beg;
	const int namesize = end - mid - 1;
	
	char *addr = malloc(addrsize + 1);
	char *name = malloc(namesize + 1);
	
	addr[addrsize] = '\0';
	name[namesize] = '\0';
	
	strncpy(addr, beg, addrsize);
	strncpy(name, mid + 1, namesize);
	
	const char format[] = { "<a href=\"%s\">%s</a>" };
	const int bufsize = strlen(format) - 2*strlen("%s") + strlen(name) + strlen(addr) + 1;
	
	char *buf = malloc(bufsize);
	
	sprintf(buf, format, addr, name);
	
	newdest = realloc(*dest, (*destsize) += strlen(buf) + 1);
	assert(newdest != NULL);
	
	strncpy(&newdest[*destit], buf, strlen(buf));
	
	*srcit += end - &src[*srcit];
	*destit += strlen(buf);
	
	free(addr);
	free(name);
	free(buf);
	
	*dest = newdest;
}

void parse_article(struct article *article) {
	int i;
	int i2 = 0;
	
	int destsize = strlen(article->text) + 1;
	char *dest = malloc(destsize);
	
	dest[0] = '\0';
	
	for(i = 0; i < strlen(article->text); i++) {
		char ch = article->text[i];
		if(ch == '[') {
			if(strncmp(&article->text[i]+1, "img ", 4) == 0)
				parse_img(&dest, article->text, &destsize, &i2, &i);
			else if(strncmp(&article->text[i]+1, "link ", 5) == 0) {
				parse_link(&dest, article->text, &destsize, &i2, &i);
			} else
				dest[i2++] = '[';
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

struct article **read_dir(DIR *dir) {
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
		
	qsort(temp, size, sizeof(struct article *), (__compar_fn_t) compare_ctime);
	
	free(stats);
	
	temp = realloc(temp, (size+1)*sizeof(struct article *));
	temp[size] = NULL; // terminating
	
	return temp;
}

void read_content(struct article *article) {
	FILE *file;
	
	if((file = fopen(article->path, "r")) == NULL)
		err(EXIT_FAILURE, "Error opening \"%s\"", article->path);	
	
	article->title = malloc(BUFSIZE);
	fgets(article->title, BUFSIZE, file);
	article->title[strlen(article->title)-1] = '\0'; // remove '\n'
	
	int i = 0, ch;
	int reserved = BUFSIZE;
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

struct article **read_articles() {	
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