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
#include "widget.h"

/* keep a global copy of this, so it only has to be created once */
static Evas_Smart *smart;

typedef struct _animator_s
{
  int pos;
  double timeout;
  Evas_Object *anim;
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

  data->pos = 0;
  data->timeout = 0;
  
  data->anim = evas_object_image_add (omc->evas);
  evas_object_smart_member_add (o, data->anim);

  data->timer = NULL;
  data->imgs = NULL;
}

static void
_animator_del (Evas_Object *o)
{
  _animator_t *data;
  
  data = evas_object_smart_data_get (o);

  evas_object_del (data->anim);
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
  evas_object_move (data->anim, x, y);
}

static void
_animator_resize (Evas_Object *o, Evas_Coord w, Evas_Coord h)
{
  _animator_t *data;
  
  data = evas_object_smart_data_get (o);

  evas_object_resize (data->anim, w, h);
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
  
  evas_object_image_file_set (data->anim, img, NULL);

  data->pos++;
  if (data->pos >= evas_list_count (data->imgs))
    data->pos = 0;

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
  
  evas_object_show (data->anim);
}

static void
_animator_hide (Evas_Object *o)
{
  _animator_t *data;
  
  data = evas_object_smart_data_get (o);

  evas_object_hide (data->anim);

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
  evas_object_color_set (data->anim, r, g, b, a);
}

static void
_animator_clip_set (Evas_Object *o, Evas_Object *clip)
{
  _animator_t *data;
  
  data = evas_object_smart_data_get (o);
  evas_object_clip_set (data->anim, clip);
}

static void
_animator_clip_unset (Evas_Object *o)
{
  _animator_t *data;
  
  data = evas_object_smart_data_get (o);
  evas_object_clip_unset (data->anim);
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
animator_new (char *id, int layer, double timeout,
              char *x, char *y, char *w, char *h)
{
  extern omc_t *omc;
  Evas_Object *animator;
  _animator_t *data;

  animator = evas_object_smart_add (omc->evas, _animator_smart_get ());

  data = evas_object_smart_data_get (animator);

  data->timeout = timeout;
  image_set (data->anim, NULL, 0, NULL, NULL, layer, x, y, w, h);
  
  if (id)
    evas_object_name_set (animator, id);
  
  return animator;
}

void
animator_set_image_list (Evas_Object *obj, char **image_list)
{
  _animator_t *data;

  data = evas_object_smart_data_get (obj);

  /* hide animation */
  evas_object_hide (obj);
 
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
  
  /* show animation */
  evas_object_show (obj);
}
