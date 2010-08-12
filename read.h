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

/** @file read.h
	@brief Functions for reading and parsing articles.
 */

#ifndef READ_H
#define READ_H

#include "article.h"

#define BUFSIZE 1024
#define ARTICLEDIR "articles/"

/** @brief	Parses one img-tag.
		The image tag at \a src is replaced by the corresponding html-code and written @a dest.
	@param	dest		Pointer to the destination.
	@param	src			Pointer to the beginning of the tag in the source.
	@param	destsize	Size of the allocated area dest points to.
	@param	destit		Pointer to the current destination iterator.
	@param	srcit		Pointer to the current source iterator.
 */
void parse_img(char **dest, char *src, int *destsize, int *destit, int *srcit);

/** @brief	Parses one link-tag.
	@see parse_img(char *dest, char *src, int *destsize, int *destit, int *srcit)
 */
void parse_link(char **dest, char *src, int *destsize, int *destit, int *srcit);

/** @brief Parses tags in article text.
		Calls all parse_* functions on the tags and escapes the text.
	@param article		Article to be parsed.
 */
void parse_article(struct article*);

/** @brief Reads out the article directory.
		The files in the directory are mapped to an empty article array in alphabetical order.
	@return				Array containing all articles with filenames set.
 */
struct article **read_dir();

/** @brief Reads article content
	@param article Article created by \a read_dir()
 */
void read_content(struct article*);

/// @brief Reads out all blog articles.
struct article **read_articles();

#endif