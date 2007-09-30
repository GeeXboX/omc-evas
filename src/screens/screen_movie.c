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

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "screen.h"
#include "widgets/widget.h"
#include "omc.h"
#include "hal_drv_storage.h"

static Evas_Object *bg;
static Evas_Object *menu;
static Evas_Object *item;
static Evas_List *volumes;

static void
cb_browse_ok (Evas_Object *obj, void *event_info)
{
  Evas_Object *mnu;
  Evas_List *list;
  int first = 1;
  
  printf ("!!!! BROWSE OK !!!\n");
  volumes = hal_get_storage_volumes ();
  printf ("volumes = %p\n", volumes);
  if (!volumes)
    return;

  mnu = menu_new ("menu_browse", 2, NULL, MENU_ALIGN_LEFT, NULL, 0, 0);
  evas_object_move (mnu,
                    compute_coord ("35%", omc->w),
                    compute_coord ("32%", omc->h));
  evas_object_resize (mnu,
                      compute_coord ("60%", omc->w),
                      compute_coord ("52%", omc->h));

  for (list = volumes; list; list = list->next)
  {
    storage_volume_t *sv = NULL;
    Evas_Object *it = NULL;

    sv = (storage_volume_t *) list->data;
    if (!sv && !sv->name)
      continue;

    it = menu_add_item (mnu, "item", sv->name);
    if (first)
    {
      first = 0;
      evas_object_focus_set (it, 1);
    }
  }

  evas_object_show (mnu);
}

static void
screen_movie_uninit (screen_t *screen)
{
  
}

void
screen_movie_init (screen_t *screen)
{
  if (!screen)
    return;

  screen->uninit = screen_movie_uninit;

  /* background pictures */
  bg = image_new ("back_movie", 1, 0, "data/back_movie.jpeg", NULL);
  image_resize (bg,
                compute_coord ("100%", omc->w),
                compute_coord ("100%", omc->h));
  screen_add_object (screen, bg);
  evas_object_show (bg);
  
  /* main menu and its items */
  menu = menu_new ("menu", 2, NULL, MENU_ALIGN_LEFT,
                   NULL, 0, 0);
  evas_object_move (menu,
                    compute_coord ("2%", omc->w),
                    compute_coord ("32%", omc->h));
  evas_object_resize (menu,
                      compute_coord ("30%", omc->w),
                      compute_coord ("52%", omc->h));

  item = menu_add_item (menu, "item_browse", "Browse FileSystem ...");
  object_set_cb_ok (item, cb_browse_ok);
  /* set focus to first menu item */
  evas_object_focus_set (item, 1);
  
  item = menu_add_item (menu, "item_playdisc", "Play Disc ...");
  item = menu_add_item (menu, "item_netshare", "Network Shares ...");
  item = menu_add_item (menu, "item_webtv", "WebTVs ...");
 
  evas_object_show (menu);
}
