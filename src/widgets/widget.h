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

#ifndef _WIDGET_H_
#define _WIDGET_H_

#include <inttypes.h>
#include <Evas.h>
#include <Ecore.h>
#include <Ecore_Evas.h>

typedef enum widget_flags {
  WIDGET_FLAG_NONE                  = 0x00,
  WIDGET_FLAG_SHOW                  = 0x01,
  WIDGET_FLAG_FOCUSABLE             = 0x02,
  WIDGET_FLAG_FOCUSED               = 0x04,
  WIDGET_FLAG_NEED_REDRAW           = 0x08,
} widget_flags_t;

typedef enum action_event_type {
  ACTION_EVENT_GO_UP,
  ACTION_EVENT_GO_DOWN,
  ACTION_EVENT_GO_LEFT,
  ACTION_EVENT_GO_RIGHT,
  ACTION_EVENT_CANCEL,
  ACTION_EVENT_OK
} action_event_type_t;

typedef enum {
  MENU_ALIGN_LEFT,
  MENU_ALIGN_CENTER,
  MENU_ALIGN_RIGHT
} menu_align_t;

typedef enum {
  BLK_NEW_LINE,
  BLK_NEW_TAB
} textblock_blk_type_t;

#define BLK_ALIGN_LEFT "left"
#define BLK_ALIGN_CENTER "center"
#define BLK_ALIGN_RIGHT "right"
#define BLK_ALIGN_TOP "top"
#define BLK_ALIGN_BOTTOM "bottom"

int compute_coord (char *coord, int max);

void object_add_default_cb (Evas_Object *obj,
                            void (*cb) (Evas_Object *obj, void *event_info));

Evas_Object *
image_new (char *id, int focusable,
           char *name, char *fname, int layer,
           char *x, char *y, char *w, char *h);

void
image_set (Evas_Object *image, char *id, int focusable,
           char *name, char *fname, int layer,
           char *x, char *y, char *w, char *h);

Evas_Object *
text_new (char *id, int focusable, int layer,
          char *str, char *font, char *color, char *fcolor, 
          int size, int alpha, char *x, char *y);

void
text_set (Evas_Object *text, char *id, int focusable, int layer,
          char *str, char *font, char *color, char *fcolor, 
          int size, int alpha, char *x, char *y);

Evas_Object *
entry_box_new (char *id, int layer, char *str, char *font,
               char *color, char *x, char *y, char *w, char *h);

Evas_Object *
clock_new (char *id, int layer, char *font, char *color, 
           int size, int alpha, char *x, char *y);

Evas_Object *
menu_new (char *id, menu_align_t align, int layer,
          char *select, char *select_w, char *select_h,
          char *font, char *color, char *fcolor, int size, int alpha,
          char *x, char *y, char *w, char *h);

Evas_Object *
menu_add_item (Evas_Object *menu, char *id, char *str);

void
menu_compute_items (Evas_Object *menu);

Evas_Object *
button_new (char *id, char *name, char *fname, int layer,
            char *x, char *y, char *w, char *h,
            void (*cb) (Evas_Object *obj, void *event_info));

Evas_Object *
box_yesno_new (char *id, char *str, char *font, char *color, 
               int size, int alpha, char *x, char *y, char *w, char *h,
               char *ok, char *cancel,
               void (*cb_ok) (Evas_Object *obj, void *event_info),
               void (*cb_cancel) (Evas_Object *obj, void *event_info));

void
textblock_format (Evas_Object *blk, textblock_blk_type_t format);

void
textblock_add_text (Evas_Object *blk, char *text);

Evas_Object *
textblock_new (char *id, char *str, int editable, char *x, char *y,
               char *w, char *h, int layer, char *font, char *color,
               int size, char *halign, char *valign);

Evas_Object *
animator_new (char *id, int layer, double timeout,
              char *x, char *y, char *w, char *h);

void
animator_set_image_list (Evas_Object *obj, char **image_list);

typedef struct object_relatives_s {
  Evas_Object *obj;
  Evas_Object *up;
  Evas_Object *down;
  Evas_Object *left;
  Evas_Object *right;
} object_relatives_t;

void
object_set_relatives (Evas_Object *obj,
                      Evas_Object *up, Evas_Object *down,
                      Evas_Object *left, Evas_Object *right);

#endif /* _WIDGET_H_ */
