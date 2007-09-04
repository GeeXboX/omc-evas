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

#include "omc.h"
#include "widgets/widget.h"
#include "screens/screen.h"

#define DEFAULT_WIDTH  1280
#define DEFAULT_HEIGHT 720
#define DEFAULT_DEPTH  24
#define DEFAULT_WM_CAPTION "GeeXboX Open Media Center"

#define OMC_DATA_DIR "./data"

void
omc_init (void)
{
  omc = malloc (sizeof (omc_t));
  omc->evas = NULL;
  omc->ee = NULL;
  omc->scr = NULL;
  omc->w = DEFAULT_WIDTH;
  omc->h = DEFAULT_HEIGHT;
}

void
omc_uninit (void)
{
  ecore_main_loop_quit ();
  
  if (omc->scr)
    screen_uninit (omc->scr);

  if (omc->evas)
    evas_free (omc->evas);
  
  free (omc);
}

static int
main_signal_exit (void *data, int ev_type, void *ev)
{
  ecore_main_loop_quit ();

  return 1;
}

static void
main_delete_request (Ecore_Evas *ee)
{
  ecore_main_loop_quit ();
}

static int
main_start (int argc, char **argv)
{
  if (!ecore_init ())
    return -1;

  ecore_app_args_set (argc, (const char **) argv);
  ecore_event_handler_add (ECORE_EVENT_SIGNAL_EXIT, main_signal_exit, NULL);
  if (!ecore_evas_init ())
    return -1;

  omc->ee = ecore_evas_software_x11_new (NULL, 0,  0, 0, omc->w, omc->h);
  if (!omc->ee)
    return -1;

  ecore_evas_callback_delete_request_set (omc->ee, main_delete_request);
  ecore_evas_title_set (omc->ee, DEFAULT_WM_CAPTION);
  ecore_evas_name_class_set (omc->ee, "geexbox_omc", "main");
  ecore_evas_size_min_set (omc->ee, omc->w, omc->h);
  ecore_evas_size_max_set (omc->ee, omc->w, omc->h);
  ecore_evas_fullscreen_set (omc->ee, 0);
  ecore_evas_show (omc->ee);

  
  omc->evas = ecore_evas_get (omc->ee);
  evas_image_cache_set (omc->evas, 8 * 1024 * 1024);
  evas_font_cache_set (omc->evas, 1 * 1024 * 1024);
  evas_font_path_append (omc->evas, OMC_DATA_DIR);
   
  return 1;
}

static void
main_stop (void)
{
  ecore_evas_shutdown ();
  ecore_shutdown ();
}

int
main (int argc, char **argv)
{
  omc_init ();

  if (main_start (argc, argv) < 1)
    return -1;
  
  /* init main screen */
  screen_init (SCREEN_TYPE_MAIN);

  ecore_main_loop_begin ();
  main_stop ();
  omc_uninit ();
  
  return 0;
}
