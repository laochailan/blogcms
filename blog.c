#include "blog.h"

#include <stdio.h>
#include <err.h>
#include <time.h>
#include <stdlib.h>
#include <assert.h>

#include "read.h"

void print_header()
{
	printf(	"Content-type: text/html\n\n");
	puts(	"<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">");
	puts(	"<html xmlns=\"http://www.w3.org/1999/xhtml\" lang=\"en\" xml:lang=\"en\">");
	puts(	"\t<head>");
	puts(	"\t\t<meta http-equiv=\"Content-type\" content=\"text/html;charset=UTF-8\" />");
	puts(	"\t\t<link rel=\"stylesheet\" type=\"text/css\" href=\"style.css\" />");
	printf(	"\t\t<title>%s</title>\n",NAME);
	puts(	"\t</head>");
	puts(	"\t<body>");
	puts(	"\t\t<div id=\"content\">");
	printf(	"\t\t\t<div id=\"banner\"><img src=\"banner.png\" alt=\"%s\" /></div>\n", NAME);
}

void print_article(struct article *art)
{	
	puts(	"\t\t\t<div class=\"article\">");	
	printf(	"\t\t\t\t<div class=\"date\">%s</div>\n", ctime(&art->ctime));	
	printf(	"\t\t\t\t<div class=\"title\">%s</div>\n", art->title);
	puts(	"\t\t\t\t<br />");			
	printf(	"\t\t\t\t%s\n", art->text);	
	puts(	"\t\t\t</div>");
}

void print_footer()
{
	puts("\t\t<br />");
	puts("\t\t<div class=\"footer\">Allgemein vernünftig sowohl in <a href=\"http://validator.w3.org/check?uri=referer\">XHTML</a> als auch in <a href=\"http://jigsaw.w3.org/css-validator/check/referer\">CSS</a> || von laochailan || viral lizensiert unter <a rel=\"license\" href=\"http://creativecommons.org/licenses/by-nc-sa/3.0/\">CC-BY-NC-SA</a></div>");
	
	puts("\t\t</div>");
	puts("\t</body>");
	puts("</html>");
}

int main(int argc, char **args)
{
	struct article **articles = read_articles();
	
// 	while(FCGI_Accept() >= 0) { // FCGI
		print_header();
//	 	if(strcmp(argc[1], "main") == 0) { // more than one page
			int i;
			for(i = 0; articles[i] != NULL; i++)
				print_article(articles[i]);
// 		}
		print_footer();
// 	}
	free_article_array(articles);
	return 1;
}