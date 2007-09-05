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

static Evas_Object *bg;
static Evas_Object *bt;
static Evas_Object *bb;
static Evas_Object *clock;

static Evas_Object *menu;
static Evas_Object *item;

static Evas_Object *button_info;
static Evas_Object *button_exit;

static Evas_Object *yesno;

static Evas_Object *animator;

static void
cb_animator_show (void *data, Evas *e,
                  Evas_Object *obj, void *event_info)
{
  char *img = (char *) data;
  if (img)
  {
    evas_object_image_file_set (animator, img, NULL);
    evas_object_show (animator);
  }
}

static void
cb_animator_hide (void *data, Evas *e,
                  Evas_Object *obj, void *event_info)
{
  if (animator)
    evas_object_hide (animator);
}

static void
cb_exit_box_ok (Evas_Object *obj, void *event_info)
{
  omc_uninit ();
}

static void
cb_exit_box_cancel (Evas_Object *obj, void *event_info)
{
  evas_object_hide (yesno);
  evas_object_del (yesno);
  evas_object_focus_set (button_exit, 1);
}

static void
cb_button_info_ok (Evas_Object *obj, void *event_info)
{
  printf ("!!!! INFO !!!\n");
}

static void
cb_button_exit_ok (Evas_Object *obj, void *event_info)
{
  /* exit message box */
  yesno = box_yesno_new ("yesno", "Really wanna quit ?",
                         "FreeSans", "#FFFFFF", 
                         30, 255, "30%", "30%", "40%", "40%",
                         "OK", "Cancel", cb_exit_box_ok, cb_exit_box_cancel);

  evas_object_show (yesno);
}

static void
screen_main_uninit (screen_t *screen)
{
  
}

void
screen_main_init (screen_t *screen)
{
  if (!screen)
    return;

  screen->uninit = screen_main_uninit;

  /* background pictures */
  bg = image_new ("background", 0, "data/background.png",
                  NULL, 1, "0", "0", "100%", "100%");
  screen_add_object (screen, bg);
  evas_object_show (bg);
  
  bt = image_new ("banner-top", 0, "data/banner-top.png",
                  NULL, 1, "0", "0", "100%", "35%");
  screen_add_object (screen, bt);
  evas_object_show (bt);

  bb = image_new ("banner-bottom", 0, "data/banner-bottom.png",
                  NULL, 1, "0", "80%", "100%", "20%");
  screen_add_object (screen, bb);
  evas_object_show (bb);

  /* date/clock */
  clock = clock_new ("clock", 1, "FreeSans", "#FFFFFF", 
                     20, 255, "990", "85");
  screen_add_object (screen, clock);
  evas_object_show (clock);

  /* animated logo */
  animator = image_new ("animator", 0, NULL,
                        NULL, 1, "40%", "19%", "60%", "66%");
  screen_add_object (screen, animator);

  /* main menu and its items */
  menu = menu_new ("menu", MENU_ALIGN_LEFT, 2,
                   NULL, NULL, NULL,
                   "FreeSans", "#FFFFFF", "#FF00FF", 30, 255,
                   "2%", "32%", "30%", "52%");

  item = menu_add_item (menu, "item_movies", "Watch Movies ...");
  evas_object_event_callback_add (item, EVAS_CALLBACK_FOCUS_IN,
                                  cb_animator_show, "data/movie.png");
  evas_object_event_callback_add (item, EVAS_CALLBACK_FOCUS_OUT,
                                  cb_animator_hide, NULL);
  /* set focus to first menu item */
  evas_object_focus_set (item, 1);
  
  item = menu_add_item (menu, "item_music", "Listen to Music ...");
  evas_object_event_callback_add (item, EVAS_CALLBACK_FOCUS_IN,
                                  cb_animator_show, "data/music.png");
  evas_object_event_callback_add (item, EVAS_CALLBACK_FOCUS_OUT,
                                  cb_animator_hide, NULL);

  item = menu_add_item (menu, "item_image", "View Pictures ...");
  evas_object_event_callback_add (item, EVAS_CALLBACK_FOCUS_IN,
                                  cb_animator_show, "data/image.png");
  evas_object_event_callback_add (item, EVAS_CALLBACK_FOCUS_OUT,
                                  cb_animator_hide, NULL);

  evas_object_show (menu);

  /* bottom line info/exit buttons */
  button_info = button_new ("info", "data/sub-info-nofocus.png",
                            "data/sub-info-focus.png", 1,
                            "91%", "88%", "40", "40", cb_button_info_ok);
  evas_object_show (button_info);

  button_exit = button_new ("exit", "data/sub-shutdown-nofocus.png",
                            "data/sub-shutdown-focus.png", 1,
                            "95%", "88%", "40", "40", cb_button_exit_ok);
  evas_object_show (button_exit);

  object_set_relatives (item, NULL, button_info, NULL, button_info);
  object_set_relatives (button_info, item, NULL, NULL, button_exit);
  object_set_relatives (button_exit, item, NULL, button_info, NULL);
}
