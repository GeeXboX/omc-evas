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

#include <stdlib.h>
#include <string.h>

#include "omc.h"
#include "widget.h"
#include "color.h"

static void
cb_text_focus_update (void *data, Evas *e, Evas_Object *obj, void *event_info)
{
  color_t *color = (color_t *) data;
  if (obj)
    evas_object_color_set (obj, color->r, color->g, color->b, color->a);
}

void
text_set (Evas_Object *text, char *id, int layer, const style_t *style,
          int focusable, char *str)
{
  const color_t *cl = NULL, *fcl = NULL;

  if (!str || !id) /* mandatory */
    return;

  if (!style)
    style = &default_text_style;

  cl = &style->normal.font_color;
  fcl = &style->focused.font_color;

  evas_object_text_font_set (text, style->normal.font, style->normal.font_size);
  evas_object_text_text_set (text, str);
  evas_object_color_set (text, cl->r, cl->g, cl->b, cl->a);
  evas_object_layer_set (text, layer);

  if (focusable)
  {
    object_add_default_cb (text, NULL);
    evas_object_event_callback_add (text, EVAS_CALLBACK_FOCUS_IN,
                                    cb_text_focus_update, fcl);
    evas_object_event_callback_add (text, EVAS_CALLBACK_FOCUS_OUT,
                                    cb_text_focus_update, cl);
  }

  evas_object_name_set (text, id);
}

Evas_Object *
text_new (char *id, int layer, const style_t *style,
          int focusable, char *str)
{
  Evas_Object *text;
  
  text = evas_object_text_add (omc->evas);

  text_set (text, id, layer, style, focusable, str);
  
  return text;
}
