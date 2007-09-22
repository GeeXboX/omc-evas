/* GeeXboX Open Media Center.
 * Copyright (C) 2007 Evan Shaw <edsrzf@umr.edu>
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "omc.h"
#include "color.h"
#include "widget.h"
#include "bindings.h"

#define ENTRY_BOX_SIZE 1024

/* keep a global copy of this, so it only has to be created once */
static Evas_Smart *smart;

typedef struct _entry_box_s
{
  char str[ENTRY_BOX_SIZE];
  int x, y;
  int w, h;
  Evas_Object *rect;
  Evas_Object *text;
  int layer;
} _entry_box_t;

static void
_entry_box_add (Evas_Object *o)
{
  _entry_box_t *data;

  data = calloc (1, sizeof (_entry_box_t));
  if (!data)
    return;

  data->rect = evas_object_rectangle_add (omc->evas);
  evas_object_smart_member_add (o, data->text);
  
  data->text = evas_object_text_add (omc->evas);
  evas_object_smart_member_add (o, data->text);
  
  evas_object_smart_data_set (o, data);
}

static void
_entry_box_del (Evas_Object *o)
{
  _entry_box_t *data;

  data = evas_object_smart_data_get (o);

  evas_object_del (data->rect);
  evas_object_del (data->text);

  free (data);
}

static void
_entry_box_move (Evas_Object *o, Evas_Coord x, Evas_Coord y)
{
  _entry_box_t *data;

  data = evas_object_smart_data_get (o);

  evas_object_move (data->rect, x, y);
  evas_object_move (data->text, x + 2, y + 2);
}

static void
_entry_box_resize (Evas_Object *o, Evas_Coord w, Evas_Coord h)
{
  _entry_box_t *data;

  data = evas_object_smart_data_get (o);

  evas_object_resize (data->rect, w, h);
  evas_object_resize (data->text, w - 2, h - 2);
}

static void
_entry_box_show (Evas_Object *o)
{
  _entry_box_t *data;

  data = evas_object_smart_data_get (o);

  evas_object_show (data->rect);
  evas_object_show (data->text);
}

static void
_entry_box_hide (Evas_Object *o)
{
  _entry_box_t *data;

  data = evas_object_smart_data_get (o);

  evas_object_hide (data->rect);
  evas_object_hide (data->text);
}

static void
_entry_box_color_set (Evas_Object *o, int r, int g, int b, int a)
{
  _entry_box_t *data;

  data = evas_object_smart_data_get (o);

  evas_object_color_set (data->rect, r, g, b, a);
  evas_object_color_set (data->text, r, g, b, a);
}

static void
_entry_box_clip_set (Evas_Object *o, Evas_Object *clip)
{
  _entry_box_t *data;

  data = evas_object_smart_data_get (o);

  evas_object_clip_set (data->rect, clip);
  evas_object_clip_set (data->text, clip);
}

static void
_entry_box_clip_unset (Evas_Object *o)
{
  _entry_box_t *data;

  data = evas_object_smart_data_get (o);

  evas_object_clip_unset (data->rect);
  evas_object_clip_unset (data->text);
}

static Evas_Smart *
_entry_box_smart_get (void)
{
  const Evas_Smart_Class sc = { "entry_box", EVAS_SMART_CLASS_VERSION,
                                _entry_box_add, _entry_box_del,
                                _entry_box_move, _entry_box_resize,
                                _entry_box_show, _entry_box_hide,
                                _entry_box_color_set,
                                _entry_box_clip_set, _entry_box_clip_unset,
                                NULL };
  if (smart)
    return smart;

  smart = evas_smart_class_new (&sc);

  return smart;
}

static void
cb_entry_box_key_down (void *data, Evas *e,
                       Evas_Object *obj, void *event_info)
{
  Evas_Event_Key_Down *kd = (Evas_Event_Key_Down *) event_info;
  _entry_box_t *box_data = (_entry_box_t *) data;
  unsigned int str_len = strlen (box_data->str);

  if (kd->string)
  {
    switch (kd->string[0])
    {
    case KEY_BACKSPACE:
      if (str_len)
        box_data->str[str_len - 1] = '\0';
      break;
    case KEY_TAB:
    case KEY_ENTER:
    case KEY_ESCAPE:
    case KEY_DELETE:
      break;
    default:
      strncat (box_data->str, kd->string, 1);
      break;
    }
  }

  evas_object_text_text_set (box_data->text, box_data->str);
}

static void
cb_entry_box_mouse_in (void *data, Evas *e,
                       Evas_Object *obj, void *event_info)
{
  if (obj)
    evas_object_focus_set (obj, 1);
}

Evas_Object *
entry_box_new (char *id, int layer, char *str, char *font,
               char *color, char *x, char *y, char *w, char *h)
{
  Evas_Object *entry_box;
  _entry_box_t *data;
  color_t *cl;

  if (!id)
    return NULL;

  entry_box = evas_object_smart_add (omc->evas, _entry_box_smart_get());
  data = evas_object_smart_data_get (entry_box);

  data->x = compute_coord (x, omc->w);
  data->y = compute_coord (y, omc->h);
  data->w = compute_coord (w, omc->w);
  data->h = compute_coord (h, omc->h);

  data->layer = layer;
  strncpy (data->str, str, ENTRY_BOX_SIZE);

  /* TODO: Make alpha configurable */
  cl = color_new (color, 255);

  evas_object_color_set (data->rect, 0, 0, 0, 255);
  evas_object_layer_set (data->rect, layer);
  evas_object_show (data->rect);

  /* TODO: Make font size configurable */
  evas_object_text_font_set (data->text, font, 20);
  evas_object_text_text_set (data->text, str);
  evas_object_layer_set (data->text, layer + 1);
  evas_object_show (data->text);

  evas_object_move (entry_box, data->x, data->y);
  evas_object_resize (entry_box, data->w, data->h);
  evas_object_name_set (entry_box, id);
  
  evas_object_event_callback_add (data->text, EVAS_CALLBACK_MOUSE_IN,
                                  cb_entry_box_mouse_in, NULL);
  evas_object_event_callback_add (data->text, EVAS_CALLBACK_KEY_DOWN,
                                  cb_entry_box_key_down, data);

  return entry_box;
}
