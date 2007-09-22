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

#ifndef _IMAGE_H_
#define _IMAGE_H_

Evas_Object *
image_new (char *id, int focusable,
           char *name, char *fname, int layer,
           char *x, char *y, char *w, char *h);

void
image_set (Evas_Object *image, char *id, int focusable,
           char *name, char *fname, int layer,
           char *x, char *y, char *w, char *h);

#endif /* _IMAGE_H_ */