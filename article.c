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

#include "article.h"

#include <stdlib.h>

void free_article(struct article *a) {
	free(a->title);
	free(a->path);
	free(a->text);
	
	free(a);
}

void free_article_array(struct article **a) {
	int i;
	for(i = 0; a[i] != NULL; i++)
		free_article(a[i]);
	
	free(a);
}