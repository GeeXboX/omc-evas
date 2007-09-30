/* GeeXboX Open Media Center.
 * Copyright (C) 2007 Benjamin Zores <ben@geexbox.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef _SMART_TEXTBLOCK_H_
#define _SMART_TEXTBLOCK_H_

typedef enum {
  BLK_NEW_LINE,
  BLK_NEW_TAB
} textblock_blk_type_t;

#define BLK_ALIGN_LEFT "left"
#define BLK_ALIGN_CENTER "center"
#define BLK_ALIGN_RIGHT "right"
#define BLK_ALIGN_TOP "top"
#define BLK_ALIGN_BOTTOM "bottom"

Evas_Object *
textblock_new (char *id, int layer, const style_t *style, char *str,
               int editable, char *halign, char *valign);

void
textblock_format (Evas_Object *blk, textblock_blk_type_t format);

void
textblock_add_text (Evas_Object *blk, char *text);

#endif /* _SMART_TEXTBLOCK_H_ */
