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
#include "screen.h"
#include "widgets/widget.h"

void
screen_uninit (screen_t *screen)
{
  if (!screen)
    return;

  if (screen->uninit)
    screen->uninit (screen);

  if (screen->objects)
  {
    Evas_List *list;
    for (list = screen->objects; list; list = list->next)
    {
      Evas_Object *obj = NULL;
    
      obj = (Evas_Object *) list->data;
      if (!obj)
        continue;
      
      evas_object_del (obj);
      screen->objects =
        evas_list_remove_list (screen->objects, screen->objects);
    }
    free (screen->objects);
  }

/*   if (screen->relatives) */
/*   { */
/*     Evas_List *list; */
/*     for (list = screen->relatives; list; list = list->next) */
/*     { */
/* /\*       Evas_Object *obj = NULL; *\/ */
    
/* /\*       obj = (Evas_Object *) list->data; *\/ */
/* /\*       if (!obj) *\/ */
/* /\*         continue; *\/ */
      
/* /\*       evas_object_del (obj); *\/ */
/* /\*       screen->relatives = *\/ */
/* /\*         evas_list_remove_list (screen->relatives, screen->relatives); *\/ */
/*     } */
/*     free (screen->relatives); */
/*   } */
  
  free (screen);
}

void
screen_init (screen_type_t type)
{
  screen_t *screen;
  extern void * screen_main_init (screen_t *screen);
  extern void * screen_movie_init (screen_t *screen);

  screen = malloc (sizeof (screen_t));
  screen->objects = NULL;
  screen->relatives = NULL;
  screen->type = type;
  screen->current = NULL;
  screen->priv = NULL;
  screen->uninit = NULL;

  /* new current screen */
  omc->scr = screen;
  
  switch (type)
  {
  case SCREEN_TYPE_MAIN:
    screen_main_init (screen);
    break;
  case SCREEN_TYPE_MOVIE:
    screen_movie_init (screen);
    break;
  }
}

void
screen_switch (screen_type_t type)
{
  if (omc->scr)
    screen_uninit (omc->scr);

  screen_init (type);
}

void
screen_add_object (screen_t *screen, Evas_Object *obj)
{
  if (!screen || !obj)
    return;

  screen->objects = evas_list_append (screen->objects, obj);
}
