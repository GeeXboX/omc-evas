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

/* keep a global copy of this, so it only has to be created once */
static Evas_Smart *smart;

typedef struct menu_s
{
  int x, y;
  int w, h;
  menu_align_t align;
  Evas_Object *select;
  Evas_Object *frame;
  Evas_List *items;
  Evas_List *hidden;
  int layer;
  const style_t *style;
} menu_t;

/*** smart object handler functions ***/

static void
_menu_add (Evas_Object *o)
{
  extern omc_t *omc;
  menu_t *data;
 
  data = calloc (1, sizeof (menu_t));
  if (!data)
    return;

  evas_object_smart_data_set (o, data);

  data->select = evas_object_image_add (omc->evas);
  evas_object_smart_member_add (o, data->select);

  data->frame = evas_object_image_add (omc->evas);
  evas_object_smart_member_add (o, data->frame);

  data->items = NULL;
  data->hidden = NULL;

  data->layer = 1;
}

static void
_menu_del (Evas_Object *o)
{
  menu_t *data;
  
  data = evas_object_smart_data_get (o);

  evas_object_del (data->select);
  evas_object_del (data->frame);

  if (data->items)
  {
    Evas_List *list;
    for (list = data->items; list; list = list->next)
    {
      Evas_Object *obj = NULL;
    
      obj = (Evas_Object *) list->data;
      if (!obj)
        continue;
      
      evas_object_del (obj);
      data->items = evas_list_remove_list (data->items, data->items);
    }
    free (data->items);

    for (list = data->hidden; list; list = list->next)
    {
      Evas_Object *obj = NULL;
    
      obj = (Evas_Object *) list->data;
      if (!obj)
        continue;
      
      evas_object_del (obj);
      data->hidden = evas_list_remove_list (data->hidden, data->hidden);
    }
    free (data->hidden);
  }
  
  free (data);
}

static void
_menu_move (Evas_Object *o, Evas_Coord x, Evas_Coord y)
{
  menu_t *data;
  
  data = evas_object_smart_data_get (o);

  data->x = x;
  data->y = y;
  evas_object_move (data->frame, x, y);
  evas_object_move (data->select, x, y);
  menu_compute_items (o);
}

static void
_menu_resize (Evas_Object *o, Evas_Coord w, Evas_Coord h)
{
  menu_t *data;
  
  data = evas_object_smart_data_get (o);

  data->w = w;
  data->h = h;
  image_resize (data->frame, w, h);
  //evas_object_resize (data->select, w, h);
  menu_compute_items (o);
}

static void
_menu_show (Evas_Object *o)
{
  menu_t *data;
  
  data = evas_object_smart_data_get (o);
  evas_object_show (data->frame);
  
  if (data->items)
  {
    Evas_List *list;
    for (list = data->items; list; list = list->next)
    {
      Evas_Object *obj = NULL;
    
      obj = (Evas_Object *) list->data;
      if (!obj)
        continue;
      
      evas_object_show (obj);
    }
  }
}

static void
_menu_hide (Evas_Object *o)
{
  menu_t *data;
  
  data = evas_object_smart_data_get (o);
  evas_object_hide (data->frame);
  
  if (data->items)
  {
    Evas_List *list;
    for (list = data->items; list; list = list->next)
    {
      Evas_Object *obj = NULL;
    
      obj = (Evas_Object *) list->data;
      if (!obj)
        continue;
      
      evas_object_hide (obj);
    }
  }
}

static void
_menu_color_set (Evas_Object *o, int r, int g, int b, int a)
{
  menu_t *data;
  
  data = evas_object_smart_data_get (o);
  //evas_object_color_set (data->text, r, g, b, a);
}

static void
_menu_clip_set (Evas_Object *o, Evas_Object *clip)
{
  menu_t *data;
  
  data = evas_object_smart_data_get (o);
  //evas_object_clip_set (data->text, clip);
}

static void
_menu_clip_unset (Evas_Object *o)
{
  menu_t *data;
  
  data = evas_object_smart_data_get (o);
  //evas_object_clip_unset (data->text);
}

static Evas_Smart *
_menu_smart_get (void)
{
  const Evas_Smart_Class sc = { "menu", EVAS_SMART_CLASS_VERSION, 
                                _menu_add, _menu_del,
                                _menu_move, _menu_resize,
                                _menu_show, _menu_hide,
                                _menu_color_set,
                                _menu_clip_set, _menu_clip_unset,
                                NULL };
  if (smart)
    return smart;

  smart = evas_smart_class_new (&sc);

  return smart; 
}

/*** callbacks ***/



/*** external API ***/

Evas_Object *
menu_new (char *id, int layer, const style_t *style,
          menu_align_t align, char *select, int select_w,
          int select_h)
{
  extern omc_t *omc;
  Evas_Object *menu;
  menu_t *data;

  if (!id) /* mandatory */
    return NULL;

  menu = evas_object_smart_add (omc->evas, _menu_smart_get ());
  data = evas_object_smart_data_get (menu);
  
  data->align = align;

  if (!style)
    style = &default_text_style;

  data->style = style;
  data->layer = layer;
  
  image_set (data->frame, NULL, 1, 0, "data/frame.png", NULL);
  
  if (select)
  {
    image_set (data->select, id, 5, 0, select, NULL);
    image_resize (data->select, select_w, select_h);
  }

/*   text_set (data->text, id, 1, layer, str, font, color, fcolor, */
/*             size, alpha, "0", "0"); */
    
/*   if (logo) */
/*   { */
/*     image_set (data->image, id, 0, logo, NULL, layer, x, y, w, h); */

/*     evas_object_event_callback_add (data->text, EVAS_CALLBACK_FOCUS_IN, */
/*                                     cb_mouse_menu_logo_show, data->image); */
/*     evas_object_event_callback_add (data->text, EVAS_CALLBACK_FOCUS_OUT, */
/*                                     cb_mouse_menu_logo_hide, data->image); */
/*   } */

  evas_object_name_set (menu, id);
  
  return menu;
}

Evas_Object *
menu_add_item (Evas_Object *menu, char *id, char *str)
{
  Evas_Object *item = NULL;
  menu_t *data;
  int cnt;
  
  data = evas_object_smart_data_get (menu);

  item = text_new (id, data->layer + 1, data->style, 1, str);
  
/*   if (logo) */
/*   { */
/*     Evas_Object *item_logo = NULL; */
/*     item_logo = image_new (omc, 0, logo, NULL, 0, */
/*                            logo_x, logo_y, logo_w, logo_h); */
/*     menu->hidden = evas_list_append (menu->hidden, item_logo); */
/*     evas_object_event_callback_add (item, EVAS_CALLBACK_MOUSE_IN, */
/*                                     cb_mouse_menu_item_logo_show, item_logo); */
/*     evas_object_event_callback_add (item, EVAS_CALLBACK_MOUSE_OUT, */
/*                                     cb_mouse_menu_item_logo_hide, item_logo); */
/*   } */

/*   if (menu->select) */
/*   { */
/*     evas_object_event_callback_add (item, EVAS_CALLBACK_MOUSE_IN, */
/*                                     cb_mouse_menu_show_selector, */
/*                                     menu->select); */
/*     evas_object_event_callback_add (item, EVAS_CALLBACK_MOUSE_OUT, */
/*                                     cb_mouse_menu_hide_selector, */
/*                                     menu->select); */
/*   } */

  data->items = evas_list_append (data->items, item);
  cnt = evas_list_count (data->items);

  if (cnt > 1)
  {
    Evas_Object *prev = (Evas_Object *) evas_list_nth (data->items, cnt - 2);
    object_set_relatives (item, prev, NULL, NULL, NULL);
    object_set_relatives (prev, NULL, item, NULL, NULL);
  }

  menu_compute_items (menu);
  
  return item;
}

void
menu_compute_items (Evas_Object *menu)
{
  Evas_List *list;
  int items, i, x, y, h;
  menu_t *data;

  data = evas_object_smart_data_get (menu);
  items = evas_list_count (data->items);

  if (!items)
    return;

  i = 0;

  x = 0;
  y = data->y;
  h = (int) (data->h / items);

  for (list = data->items; list; list = list->next)
  {
    Evas_Object *obj = NULL;

    obj = (Evas_Object *) list->data;
    if (!obj)
      continue;

    switch (data->align)
    {
    case MENU_ALIGN_LEFT:
      x = data->x + 25;
      break;
    case MENU_ALIGN_CENTER:
      x = (int) (data->x +
                 (data->w - evas_object_text_horiz_advance_get (obj)) / 2);
      break;
    case MENU_ALIGN_RIGHT:
      x = (int) (data->x + data->w - evas_object_text_horiz_advance_get (obj));
      break;
    }

    y = (int) (data->y + i++ * h +
               (h - evas_object_text_max_ascent_get (obj)) / 2);
    evas_object_move (obj, x, y);
  }
}
