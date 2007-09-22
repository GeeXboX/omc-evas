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

#ifndef _SMART_BOX_YESNO_H_
#define _SMART_BOX_YESNO_H_

Evas_Object *
box_yesno_new (char *id, char *str, char *font, char *color, 
               int size, int alpha, char *x, char *y, char *w, char *h,
               char *ok, char *cancel,
               void (*cb_ok) (Evas_Object *obj, void *event_info),
               void (*cb_cancel) (Evas_Object *obj, void *event_info));

#endif /* _SMART_BOX_YESNO_H_ */
