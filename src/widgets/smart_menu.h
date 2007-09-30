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

#ifndef _SMART_MENU_H_
#define _SMART_MENU_H_

#include "widget.h"

typedef enum {
  MENU_ALIGN_LEFT,
  MENU_ALIGN_CENTER,
  MENU_ALIGN_RIGHT
} menu_align_t;

Evas_Object *
menu_new (char *id, int layer, const style_t *style,
          menu_align_t align, char *select, int select_w,
          int select_h);

Evas_Object *
menu_add_item (Evas_Object *menu, char *id, char *str);

void
menu_compute_items (Evas_Object *menu);

#endif /* _SMART_MENU_H_ */
