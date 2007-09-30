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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "omc.h"
#include "widget.h"

typedef enum {
  KEY_UNKNOWN,
  KEY_UP,
  KEY_DOWN,
  KEY_LEFT,
  KEY_RIGHT,
  KEY_OK,
  KEY_CANCEL,
} action_key_t;

static const struct {
  const char *name;
  action_key_t key;
} key_map[] = {
  { "Up",           KEY_UP },
  { "Down",         KEY_DOWN },
  { "Left",         KEY_LEFT },
  { "Right",        KEY_RIGHT },
  { "Return",       KEY_OK },
  { "KP_Enter",     KEY_OK },
  { "Escape",       KEY_CANCEL },
  { NULL }
};

static action_key_t
event_get_action_key (const char *key)
{
  int i;
  
  if (!key)
    return KEY_UNKNOWN;

  printf ("key: %s\n", key);
  
  for (i = 0; key_map[i].name; i++)
    if (!strcmp (key_map[i].name, key))
      return key_map[i].key;

  return KEY_UNKNOWN;
}

/* Callbacks */
static void
cb_mouse_in (void *data, Evas *e, Evas_Object *obj, void *event_info)
{
  if (obj)
    evas_object_focus_set (obj, 1);
}

static void
cb_key_down (void *data, Evas *e, Evas_Object *obj, void *event_info)
{
  object_relatives_t *r = NULL;
  action_key_t k;
  Evas_List *list;
  Evas_Event_Key_Down *kd = (Evas_Event_Key_Down *) event_info;
  void (*cb) (Evas_Object *obj, void *event_info) = data;
  const char *obj_name = NULL;
  const char *r_name = NULL;
  
  /* check for existing object relatives structure */
  for (list = omc->scr->relatives; list; list = list->next)
  {
    r = (object_relatives_t *) list->data;

    if (!r)
      continue;
    if (r->obj == obj)
      break;
    obj_name = evas_object_name_get (obj);
    r_name = evas_object_name_get (r->obj);
    if (obj_name && r_name && !strcmp (r_name, obj_name))
      break;
    r = NULL;
  }

  k = event_get_action_key (kd->keyname);

  switch (k)
  {
  case KEY_UP:
    if (r && r->up)
      evas_object_focus_set (r->up, 1);
    break;
  case KEY_DOWN:
    if (r && r->down)
      evas_object_focus_set (r->down, 1);
    break;
  case KEY_LEFT:
    if (r && r->left)
      evas_object_focus_set (r->left, 1);
    break;
  case KEY_RIGHT:
    if (r && r->right)
      evas_object_focus_set (r->right, 1);
    break;
  case KEY_OK:
    if (cb)
      cb (obj, event_info);
    break;
  default:
    break;
  }
  
}

static void
cb_mouse_down (void *data, Evas *e, Evas_Object *obj, void *event_info)
{
  Evas_Event_Mouse_Down *md = (Evas_Event_Mouse_Down *) event_info;
  void (*cb) (Evas_Object *obj, void *event_info) = data;

  printf ("Mouse Button: %d\n", md->button);
  if (md->button == 1 && cb)
    cb (obj, event_info);
}

void
object_add_default_cb (Evas_Object *obj,
                       void (*cb) (Evas_Object *obj, void *event_info))
{
  if (!obj)
    return;
  
  evas_object_event_callback_add (obj, EVAS_CALLBACK_MOUSE_IN,
                                  cb_mouse_in, NULL);
  evas_object_event_callback_add (obj, EVAS_CALLBACK_KEY_DOWN,
                                  cb_key_down, cb);
  evas_object_event_callback_add (obj, EVAS_CALLBACK_MOUSE_DOWN,
                                  cb_mouse_down, cb);
}

static void
cb_key_ok (void *data, Evas *e, Evas_Object *obj, void *event_info)
{
  action_key_t k;
  Evas_Event_Key_Down *kd = (Evas_Event_Key_Down *) event_info;
  void (*cb) (Evas_Object *obj, void *event_info) = data;

  k = event_get_action_key (kd->keyname);

  switch (k)
  {
  case KEY_OK:
    if (cb)
      cb (obj, event_info);
    break;
  default:
    break;
  } 
}

void
object_set_cb_ok (Evas_Object *obj,
                  void (*cb) (Evas_Object *obj, void *event_info))
{
  if (!obj)
    return;

  evas_object_event_callback_add (obj, EVAS_CALLBACK_KEY_DOWN,
                                  cb_key_ok, cb);
}

static void
cb_key_cancel (void *data, Evas *e, Evas_Object *obj, void *event_info)
{
  action_key_t k;
  Evas_Event_Key_Down *kd = (Evas_Event_Key_Down *) event_info;
  void (*cb) (Evas_Object *obj, void *event_info) = data;

  k = event_get_action_key (kd->keyname);

  switch (k)
  {
  case KEY_CANCEL:
    if (cb)
      cb (obj, event_info);
    break;
  default:
    break;
  } 
}

void
object_set_cb_cancel (Evas_Object *obj,
                      void (*cb) (Evas_Object *obj, void *event_info))
{
  if (!obj)
    return;

  evas_object_event_callback_add (obj, EVAS_CALLBACK_KEY_DOWN,
                                  cb_key_cancel, cb);
}

int
compute_coord (char *coord, int max)
{
  int val = 0;
  char *end;
  
  if (!coord)
    return 0;

  val = strtol (coord, &end, 10);
  if (*end == '%')
    val = val * max / 100;

  return val;
}

void
object_set_relatives (Evas_Object *obj,
                      Evas_Object *up, Evas_Object *down,
                      Evas_Object *left, Evas_Object *right)
{
  screen_t *scr = omc->scr;
  object_relatives_t *relatives, *r = NULL;
  Evas_List *list;

  /* list is empty */
  if (!scr->relatives)
  {
    relatives = calloc (1, sizeof (object_relatives_t));
    relatives->obj = obj;
    scr->relatives = evas_list_append (scr->relatives, relatives);
  }

  /* check for existing object relatives structure */
  for (list = scr->relatives; list; list = list->next)
  {
    r = (object_relatives_t *) list->data;

    if (!r)
      continue;
    if (r->obj == obj)
      break;
    r = NULL;
  }

  /* new object, add its relatives entry to list */
  if (!r)
  {
    r = calloc (1, sizeof (object_relatives_t));
    r->obj = obj;
    scr->relatives = evas_list_append (scr->relatives, r);
  }

  if (up)
    r->up = up;
  if (down)
    r->down = down;
  if (left)
    r->left = left;
  if (right)
    r->right = right;

  printf ("Object %s has relatives: %s, %s, %s, %s\n",
          evas_object_name_get (obj),
          evas_object_name_get (up),
          evas_object_name_get (down),
          evas_object_name_get (left),
          evas_object_name_get (right));
}
