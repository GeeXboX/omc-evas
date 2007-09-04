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
#include <stdio.h>

#include "omc.h"
#include "color.h"
#include "widget.h"

/* keep a global copy of this, so it only has to be created once */
static Evas_Smart *smart;

typedef struct _box_yesno_s
{
  Evas_Object *bg;
  Evas_Object *box;
  Evas_Object *txt;
  Evas_Object *ok;
  Evas_Object *cancel;
} _box_yesno_t;

/*** smart object handler functions ***/

static void
_box_yesno_add (Evas_Object *o)
{
  extern omc_t *omc;
  _box_yesno_t *data;
 
  data = calloc (1, sizeof (_box_yesno_t));
  if (!data)
    return;

  evas_object_smart_data_set (o, data);

  data->bg = evas_object_image_add (omc->evas);
  evas_object_smart_member_add (o, data->bg);

  data->box = evas_object_image_add (omc->evas);
  evas_object_smart_member_add (o, data->box);

  data->txt = evas_object_text_add (omc->evas);
  evas_object_smart_member_add (o, data->txt);

  data->ok = evas_object_text_add (omc->evas);
  evas_object_smart_member_add (o, data->ok);

  data->cancel = evas_object_text_add (omc->evas);
  evas_object_smart_member_add (o, data->cancel);
}

static void
_box_yesno_del (Evas_Object *o)
{
  _box_yesno_t *data;
  
  data = evas_object_smart_data_get (o);

  evas_object_del (data->bg);
  evas_object_del (data->box);
  evas_object_del (data->txt);
  evas_object_del (data->ok);
  evas_object_del (data->cancel);

  free (data);
}

static void
_box_yesno_move (Evas_Object *o, Evas_Coord x, Evas_Coord y)
{
  _box_yesno_t *data;
  
  data = evas_object_smart_data_get (o);

  //evas_object_move (data->box_yesno, x, y);
}

static void
_box_yesno_resize (Evas_Object *o, Evas_Coord w, Evas_Coord h)
{
  _box_yesno_t *data;
  
  data = evas_object_smart_data_get (o);

  //evas_object_resize (data->box_yesno, w, h);
}

static void
_box_yesno_show (Evas_Object *o)
{
  _box_yesno_t *data;
  
  data = evas_object_smart_data_get (o);

  evas_object_show (data->bg);
  evas_object_show (data->box);
  evas_object_show (data->txt);
  evas_object_show (data->ok);
  evas_object_show (data->cancel);
  evas_object_focus_set (data->ok, 1);
}

static void
_box_yesno_hide (Evas_Object *o)
{
  _box_yesno_t *data;
  
  data = evas_object_smart_data_get (o);

  evas_object_hide (data->bg);
  evas_object_hide (data->box);
  evas_object_hide (data->txt);
  evas_object_hide (data->ok);
  evas_object_hide (data->cancel);
}

static void
_box_yesno_color_set (Evas_Object *o, int r, int g, int b, int a)
{
  _box_yesno_t *data;
  
  data = evas_object_smart_data_get (o);
  //evas_object_color_set (data->box_yesno, r, g, b, a);
}

static void
_box_yesno_clip_set (Evas_Object *o, Evas_Object *clip)
{
  _box_yesno_t *data;
  
  data = evas_object_smart_data_get (o);
  //evas_object_clip_set (data->box_yesno, clip);
}

static void
_box_yesno_clip_unset (Evas_Object *o)
{
  _box_yesno_t *data;
  
  data = evas_object_smart_data_get (o);
  //evas_object_clip_unset(data->box_yesno);
}

static Evas_Smart *
_box_yesno_smart_get (void)
{
  const Evas_Smart_Class sc = { "box_yesno", EVAS_SMART_CLASS_VERSION, 
                                _box_yesno_add, _box_yesno_del,
                                _box_yesno_move, _box_yesno_resize,
                                _box_yesno_show, _box_yesno_hide,
                                _box_yesno_color_set,
                                _box_yesno_clip_set, _box_yesno_clip_unset,
                                NULL };
  if (smart)
    return smart;

  smart = evas_smart_class_new (&sc);

  return smart; 
}

/*** external API ***/

Evas_Object *
box_yesno_new (char *id, char *str, char *font, char *color, 
               int size, int alpha, char *x, char *y, char *w, char *h,
               char *ok, char *cancel,
               void (*cb_ok) (Evas_Object *obj, void *event_info),
               void (*cb_cancel) (Evas_Object *obj, void *event_info))
{
  extern omc_t *omc;
  Evas_Object *box_yesno;
  _box_yesno_t *data;
  color_t *cl;
  int x2, y2, w2, h2;
  char x3[16], y3[16];
  
  box_yesno = evas_object_smart_add (omc->evas, _box_yesno_smart_get ());

  x2 = compute_coord (x, omc->w);
  y2 = compute_coord (y, omc->h);
  w2 = compute_coord (x, omc->w);
  h2 = compute_coord (y, omc->h);
  cl = color_new (color, alpha);
  
  data = evas_object_smart_data_get (box_yesno);

  image_set (data->bg, NULL, 0, "data/alpha-layer.png",
             NULL, 50, 0, 0, "1280", "720");

  image_set (data->box, NULL, 0, "data/frame-plain.png",
             NULL, 51, x, y, w, h);

  sprintf (x3, "%d", x2 + 120);
  sprintf (y3, "%d", y2 + 100);
  text_set (data->txt, NULL, 0, 52, str, font, color, NULL,
            size, alpha, x3, y3);

  sprintf (x3, "%d", x2 + 30);
  sprintf (y3, "%d", y2 + 200);
  
  text_set (data->ok, "ok", 1, 52, ok, font, color, "#FF00FF",
            size, alpha, x3, y3);

  sprintf (x3, "%d", x2 + 350);
  
  text_set (data->cancel, "cancel", 1, 52, cancel, font, color, "#FF00FF",
            size, alpha, x3, y3);

  object_set_relatives (data->ok, NULL, NULL, NULL, data->cancel);
  object_set_relatives (data->cancel, NULL, NULL, data->ok, NULL);

  object_add_default_cb (data->ok, cb_ok);
  object_add_default_cb (data->cancel, cb_cancel);

  evas_object_name_set (box_yesno, id);
  
  return box_yesno;
}
