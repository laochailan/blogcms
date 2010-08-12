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

/** @file article.h
	@brief Declares the article structure and freeing functions.
 */

#ifndef ARTICLE_H
#define ARTICLE_H

#include <time.h>

/** @brief Article data structure.
		
		Stores the data of one article on the blog. Articles are mapped from the filesystem. One file in the article directory is interpreted as one article.
 */
struct article {
	/// @brief Path to the corresponding text file.
	char *path;
	/** @brief Title of the article.
			
			Read from the first line in the text file.
	 */
	char *title;
	/// @brief Time and date of creation or ctime of the file.
	time_t ctime;
	/** @brief Actual article content.
	
			Can contain serveral tags to format the article and include media. Normal html in the article file is not escaped.
	 */
	char *text;
};

void free_article(struct article *a);
void free_article_array(struct article **a);

#endif