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
#include <unistd.h>

#include "omc.h"
#include "widget.h"

#define FRAME_PERIOD 0.1 /* seconds */
#define ALPHA_STEP 25

/* keep a global copy of this, so it only has to be created once */
static Evas_Smart *smart;

typedef struct _animator_s
{
  int buf;
  int pos;
  double timeout;
  Evas_Object *buffer1;
  Evas_Object *buffer2;
  Ecore_Timer *timer;
  Evas_List *imgs;
} _animator_t;

/*** smart object handler functions ***/

static void
_animator_add (Evas_Object *o)
{
  extern omc_t *omc;
  _animator_t *data;
 
  data = calloc (1, sizeof (_animator_t));
  if (!data)
    return;

  evas_object_smart_data_set (o, data);

  data->buf = 0;
  data->pos = 0;
  data->timeout = 0;
  
  data->buffer1 = evas_object_image_add (omc->evas);
  evas_object_smart_member_add (o, data->buffer1);

  data->buffer2 = evas_object_image_add (omc->evas);
  evas_object_smart_member_add (o, data->buffer2);

  data->timer = NULL;
  data->imgs = NULL;
}

static void
_animator_del (Evas_Object *o)
{
  _animator_t *data;
  
  data = evas_object_smart_data_get (o);

  evas_object_del (data->buffer1);
  evas_object_del (data->buffer2);
  if (data->timer)
  {
    ecore_timer_del (data->timer);
    data->timer = NULL;
  }

  free (data);
}

static void
_animator_move (Evas_Object *o, Evas_Coord x, Evas_Coord y)
{
  _animator_t *data;
  
  data = evas_object_smart_data_get (o);
  evas_object_move (data->buffer1, x, y);
  evas_object_move (data->buffer2, x, y);
}

static void
_animator_resize (Evas_Object *o, Evas_Coord w, Evas_Coord h)
{
  _animator_t *data;
  
  data = evas_object_smart_data_get (o);

  image_resize (data->buffer1, w, h);
  image_resize (data->buffer2, w, h);
}

static int
animator_show_pulse (void *data)
{
  Evas_Object *obj = data;
  Evas_Coord alpha;

  evas_object_color_get (obj, NULL, NULL, NULL, &alpha);
  alpha += ALPHA_STEP;
  evas_object_color_set (obj, alpha, alpha, alpha, alpha);
  evas_object_color_get (obj, NULL, NULL, NULL, &alpha);

  if (alpha >= 255)
  {
    evas_object_color_set (obj, 255, 255, 255, 255);
    return 0;
  }

  /* return 1 to keep the timer going */
  return 1;
}

static int
animator_hide_pulse (void *data)
{
  Evas_Object *obj = data;
  Evas_Coord alpha;

  evas_object_color_get (obj, NULL, NULL, NULL, &alpha);
  alpha -= ALPHA_STEP;
  evas_object_color_set (obj, alpha, alpha, alpha, alpha);
  evas_object_color_get (obj, NULL, NULL, NULL, &alpha);

  if (alpha <= 0)
  {
    evas_object_color_set (obj, 255, 255, 255, 0);
    evas_object_hide (obj);
    return 0;
  }

  /* return 1 to keep the timer going */
  return 1;
}

static void
cb_show_steps (void *data, Evas *e, Evas_Object *obj, void *event_info)
{
  ecore_timer_add (FRAME_PERIOD, animator_show_pulse, obj);
}

static int
animator_timer_pulse (void *cookie)
{
  Evas_Object *obj = cookie;
  _animator_t *data;
  Evas_List *list;
  char *img;
  
  data = evas_object_smart_data_get (obj);
  if (!data)
    return 0;
  
  list = evas_list_nth_list (data->imgs, data->pos);
  if (!list)
    return 0;
  
  img = (char *) list->data;
  if (!img)
    return 0;

  if (data->buf == 0)
  {
    evas_object_color_set (data->buffer1, 255, 255, 255, 0);
    evas_object_image_file_set (data->buffer1, img, NULL);

    ecore_timer_add (FRAME_PERIOD, animator_hide_pulse, data->buffer2);
    evas_object_show (data->buffer1);
    data->buf = 1;
  }
  else
  {
    evas_object_color_set (data->buffer2, 255, 255, 255, 0);
    evas_object_image_file_set (data->buffer2, img, NULL);

    ecore_timer_add (FRAME_PERIOD, animator_hide_pulse, data->buffer1);
    evas_object_show (data->buffer2);
    data->buf = 0;
  }

  data->pos++;
  if (data->pos >= evas_list_count (data->imgs))
    data->pos = 0;

  /* check for single element: no transition effect is needed */
  if (evas_list_count (data->imgs) == 1)
    return 0;

  /* return 1 to keep the timer going */
  return 1;
}

static void
_animator_show (Evas_Object *o)
{
  _animator_t *data;
  
  data = evas_object_smart_data_get (o);

  animator_timer_pulse (o);

  if (!data->timer)
    data->timer = ecore_timer_add (data->timeout,
                                   animator_timer_pulse, o);
}

static void
_animator_hide (Evas_Object *o)
{
  _animator_t *data;
  
  data = evas_object_smart_data_get (o);

  evas_object_hide (data->buffer1);
  evas_object_hide (data->buffer2);

  if (data->timer)
  {
    ecore_timer_del (data->timer);
    data->timer = NULL;
  }
}

static void
_animator_color_set (Evas_Object *o, int r, int g, int b, int a)
{
  _animator_t *data;
  
  data = evas_object_smart_data_get (o);
  evas_object_color_set (data->buffer1, r, g, b, a);
  evas_object_color_set (data->buffer2, r, g, b, a);
}

static void
_animator_clip_set (Evas_Object *o, Evas_Object *clip)
{
  _animator_t *data;
  
  data = evas_object_smart_data_get (o);
  evas_object_clip_set (data->buffer1, clip);
  evas_object_clip_set (data->buffer2, clip);
}

static void
_animator_clip_unset (Evas_Object *o)
{
  _animator_t *data;
  
  data = evas_object_smart_data_get (o);
  evas_object_clip_unset (data->buffer1);
  evas_object_clip_unset (data->buffer2);
}

static Evas_Smart *
_animator_smart_get (void)
{
  const Evas_Smart_Class sc = { "animator", EVAS_SMART_CLASS_VERSION, 
                                _animator_add, _animator_del,
                                _animator_move, _animator_resize,
                                _animator_show, _animator_hide,
                                _animator_color_set,
                                _animator_clip_set, _animator_clip_unset,
                                NULL };
  if (smart)
    return smart;

  smart = evas_smart_class_new (&sc);

  return smart; 
}

/*** external API ***/

Evas_Object *
animator_new (char *id, int layer, double timeout)
{
  extern omc_t *omc;
  Evas_Object *animator;
  _animator_t *data;

  if (!id) /* mandatory */
    return NULL;

  animator = evas_object_smart_add (omc->evas, _animator_smart_get ());

  data = evas_object_smart_data_get (animator);

  data->timeout = timeout;
  image_set (data->buffer1, NULL, layer, 0, NULL, NULL);
  evas_object_image_alpha_set (data->buffer1, 1);
  evas_object_event_callback_add (data->buffer1, EVAS_CALLBACK_SHOW,
                                  cb_show_steps, NULL);

  image_set (data->buffer2, NULL, layer, 0, NULL, NULL);
  evas_object_image_alpha_set (data->buffer2, 1);
  evas_object_event_callback_add (data->buffer2, EVAS_CALLBACK_SHOW,
                                  cb_show_steps, NULL);

  evas_object_name_set (animator, id);
  
  return animator;
}

void
animator_set_image_list (Evas_Object *obj, char **image_list)
{
  _animator_t *data;

  data = evas_object_smart_data_get (obj);

  /* clear any current image list */
  if (data->imgs)
  {
    Evas_List *list;
    for (list = data->imgs; list; list = list->next)
    {
      char *img = (char *) list->data;
      if (!img)
        continue;
      
      data->imgs = evas_list_remove_list (data->imgs, list);
    }
    free (data->imgs);
  }

  /* fill in new list */
  while (*image_list)
  {
    data->imgs = evas_list_append (data->imgs, *image_list++);
  }

  /* always display first picture from list */
  data->pos = 0;
  data->buf = 0;
  
  /* show animation */
  evas_object_show (obj);
}
