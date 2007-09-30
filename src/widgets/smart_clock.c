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
#include <time.h>

#include "omc.h"
#include "color.h"
#include "widget.h"

static const style_t default_style = {
        { "FreeSans", 20, { 0xFF, 0xFF, 0xFF, 0xFF }, { 0x00, 0x00, 0x00, 0xFF } },
        { "FreeSans", 20, { 0xFF, 0x00, 0xFF, 0xFF }, { 0x00, 0x00, 0x00, 0xFF } }
};

/* keep a global copy of this, so it only has to be created once */
static Evas_Smart *smart;

typedef struct _clock_s
{
  Evas_Object *clock;
  Ecore_Timer *timer;
} _clock_t;

/*** smart object handler functions ***/

static void
_clock_add (Evas_Object *o)
{
  extern omc_t *omc;
  _clock_t *data;
 
  data = calloc (1, sizeof (_clock_t));
  if (!data)
    return;

  evas_object_smart_data_set (o, data);

  data->clock = evas_object_text_add (omc->evas);
  evas_object_smart_member_add (o, data->clock);

  data->timer = NULL;
}

static void
_clock_del (Evas_Object *o)
{
  _clock_t *data;
  
  data = evas_object_smart_data_get (o);

  evas_object_del (data->clock);
  ecore_timer_del (data->timer);
  free (data);
}

static void
_clock_move (Evas_Object *o, Evas_Coord x, Evas_Coord y)
{
  _clock_t *data;
  
  data = evas_object_smart_data_get (o);
  evas_object_move (data->clock, x, y);
}

static void
_clock_resize (Evas_Object *o, Evas_Coord w, Evas_Coord h)
{
  _clock_t *data;
  
  data = evas_object_smart_data_get (o);

  evas_object_resize (data->clock, w, h);
}

static void
_clock_show (Evas_Object *o)
{
  _clock_t *data;
  
  data = evas_object_smart_data_get (o);
  evas_object_show (data->clock);
}

static void
_clock_hide (Evas_Object *o)
{
  _clock_t *data;
  
  data = evas_object_smart_data_get (o);
  evas_object_hide (data->clock);
}

static void
_clock_color_set (Evas_Object *o, int r, int g, int b, int a)
{
  _clock_t *data;
  
  data = evas_object_smart_data_get (o);
  evas_object_color_set (data->clock, r, g, b, a);
}

static void
_clock_clip_set (Evas_Object *o, Evas_Object *clip)
{
  _clock_t *data;
  
  data = evas_object_smart_data_get (o);
  evas_object_clip_set (data->clock, clip);
}

static void
_clock_clip_unset (Evas_Object *o)
{
  _clock_t *data;
  
  data = evas_object_smart_data_get (o);
  evas_object_clip_unset(data->clock);
}

static Evas_Smart *
_clock_smart_get (void)
{
  const Evas_Smart_Class sc = { "clock", EVAS_SMART_CLASS_VERSION, 
                                _clock_add, _clock_del,
                                _clock_move, _clock_resize,
                                _clock_show, _clock_hide,
                                _clock_color_set,
                                _clock_clip_set, _clock_clip_unset,
                                NULL };
  if (smart)
    return smart;

  smart = evas_smart_class_new (&sc);

  return smart; 
}

/*** callbacks ***/

static int
clock_timer_pulse (void *data)
{
  Evas_Object *obj = data;
  time_t tt;
  char *t;
  
  tt = time (NULL);
  t = ctime (&tt);
  t[strlen (t) - 1] = '\0';
  evas_object_text_text_set (obj, t);

  /* return 1 to keep the timer going */
  return 1;
}

/*** external API ***/

Evas_Object *
clock_new (char *id, int layer, const style_t *style)
{
  extern omc_t *omc;
  Evas_Object *clock;
  _clock_t *data;
  const color_t *cl;

  if (!id) /* mandatory */
    return NULL;

  if (!style)
    style = &default_style;
  
  clock = evas_object_smart_add (omc->evas, _clock_smart_get ());

  cl = &style->normal.font_color;
  
  data = evas_object_smart_data_get (clock);

  evas_object_text_font_set (data->clock, style->normal.font, style->normal.font_size);
  clock_timer_pulse (data->clock);
  evas_object_color_set (data->clock, cl->r, cl->g, cl->b, cl->a);
  evas_object_layer_set (data->clock, layer);

  data->timer = ecore_timer_add (1.0, clock_timer_pulse, data->clock);

  evas_object_name_set (clock, id);
  
  return clock;
}
