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

static char *movie_list[] = {
  "data/movie.png",
  "data/movie2.png",
  NULL
};

static char *music_list[] = {
  "data/music.png",
  "data/music2.png",
  "data/music3.png",
  "data/music4.png",
  NULL
};

static char *image_list[] = {
  "data/image.png",
  "data/image2.png",
  "data/image3.png",
  NULL
};

static char *weather_list[] = {
  "data/weather.png",
  "data/weather2.png",
  "data/weather3.png",
  "data/weather4.png",
  NULL
};

static void
cb_animator_show (void *data, Evas *e,
                  Evas_Object *obj, void *event_info)
{
  char **imgs = (char **) data;
  if (imgs)
    animator_set_image_list (animator, imgs);
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
  yesno = box_yesno_new ("yesno", "Really wanna quit ?", NULL,
                         "OK", "Cancel", cb_exit_box_ok, cb_exit_box_cancel);
  evas_object_move (yesno, compute_coord ("30%", omc->w), compute_coord ("30%", omc->h));
  evas_object_resize (yesno, compute_coord ("40%", omc->w), compute_coord ("40%", omc->h));

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
  bg = image_new ("background", 1, 0, "data/background.png",
                  NULL);
  image_resize (bg, compute_coord ("100%", omc->w), compute_coord ("100%", omc->h));
  screen_add_object (screen, bg);
  evas_object_show (bg);
  
  bt = image_new ("banner-top", 1, 0, "data/banner-top.png", NULL);
  image_resize (bt, compute_coord ("100%", omc->w), compute_coord ("35%", omc->h));
  screen_add_object (screen, bt);
  evas_object_show (bt);

  bb = image_new ("banner-bottom", 1, 0, "data/banner-bottom.png", NULL);
  screen_add_object (screen, bb);
  evas_object_move (bb, 0, compute_coord ("80%", omc->h));
  image_resize (bb, compute_coord ("100%", omc->w), compute_coord ("20%", omc->h));
  evas_object_show (bb);

  /* date/clock */
  clock = clock_new ("clock", 1, NULL);
  // Clock font size should be 20
  evas_object_move (clock, 990, 85);
  screen_add_object (screen, clock);
  evas_object_show (clock);

  /* animated logo */
  animator = animator_new ("animator", 1, 10.0);
  evas_object_move (animator, compute_coord ("40%", omc->w), compute_coord ("19%", omc->h));
  evas_object_resize (animator, compute_coord ("60%", omc->w), compute_coord ("66%", omc->h));
  
  screen_add_object (screen, animator);
  
  /* main menu and its items */
  menu = menu_new ("menu", 2, NULL, MENU_ALIGN_LEFT,
                   NULL, 0, 0);
  evas_object_move (menu, compute_coord ("2%", omc->w), compute_coord ("32%", omc->h));
  evas_object_resize (menu, compute_coord ("30%", omc->w), compute_coord ("52%", omc->h));

  item = menu_add_item (menu, "item_movies", "Watch Movies ...");
  evas_object_event_callback_add (item, EVAS_CALLBACK_FOCUS_IN,
                                  cb_animator_show, movie_list);
  evas_object_event_callback_add (item, EVAS_CALLBACK_FOCUS_OUT,
                                  cb_animator_hide, NULL);
  /* set focus to first menu item */
  evas_object_focus_set (item, 1);
  
  item = menu_add_item (menu, "item_music", "Listen to Music ...");
  evas_object_event_callback_add (item, EVAS_CALLBACK_FOCUS_IN,
                                  cb_animator_show, music_list);
  evas_object_event_callback_add (item, EVAS_CALLBACK_FOCUS_OUT,
                                  cb_animator_hide, NULL);

  item = menu_add_item (menu, "item_image", "View Pictures ...");
  evas_object_event_callback_add (item, EVAS_CALLBACK_FOCUS_IN,
                                  cb_animator_show, image_list);
  evas_object_event_callback_add (item, EVAS_CALLBACK_FOCUS_OUT,
                                  cb_animator_hide, NULL);

  item = menu_add_item (menu, "item_weather", "Weather Forecast ...");
  evas_object_event_callback_add (item, EVAS_CALLBACK_FOCUS_IN,
                                  cb_animator_show, weather_list);
  evas_object_event_callback_add (item, EVAS_CALLBACK_FOCUS_OUT,
                                  cb_animator_hide, NULL);

  evas_object_show (menu);

  /* bottom line info/exit buttons */
  button_info = button_new ("info", 1, "data/sub-info-nofocus.png",
                            "data/sub-info-focus.png", cb_button_info_ok);
  evas_object_move (button_info, compute_coord ("91%", omc->w), compute_coord ("88%", omc->h));
  image_resize (button_info, 40, 40);
  evas_object_show (button_info);

  button_exit = button_new ("exit", 1, "data/sub-shutdown-nofocus.png",
                            "data/sub-shutdown-focus.png", cb_button_exit_ok);
  evas_object_move (button_exit, compute_coord ("95%", omc->w), compute_coord ("88%", omc->h));
  image_resize (button_exit, 40, 40);
  evas_object_show (button_exit);

  object_set_relatives (item, NULL, button_info, NULL, button_info);
  object_set_relatives (button_info, item, NULL, NULL, button_exit);
  object_set_relatives (button_exit, item, NULL, button_info, NULL);
}
