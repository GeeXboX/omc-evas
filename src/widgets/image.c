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

#include "omc.h"
#include "widget.h"

static void
cb_img_focus_update (void *data, Evas *e, Evas_Object *obj, void *event_info)
{
  char *file = (char *) data;
  if (obj)
    evas_object_image_file_set (obj, file, NULL);
}

void
image_set (Evas_Object *image, char *id, int focusable,
           char *name, char *fname, int layer,
           char *x, char *y, char *w, char *h)
{
  extern omc_t *omc;
  int x2, y2, w2, h2;

  x2 = compute_coord (x, omc->w);
  y2 = compute_coord (y, omc->h);
  w2 = compute_coord (w, omc->w);
  h2 = compute_coord (h, omc->h);
  
  evas_object_image_file_set (image, name, NULL);
  evas_object_move (image, x2, y2); 
  evas_object_resize (image, w2, h2);
  evas_object_image_fill_set (image, 0, 0, w2, h2);
  evas_object_layer_set (image, layer);
  evas_object_image_smooth_scale_set (image, 1);

  if (focusable && fname)
  {
    evas_object_event_callback_add (image, EVAS_CALLBACK_FOCUS_IN,
                                    cb_img_focus_update, fname);
    evas_object_event_callback_add (image, EVAS_CALLBACK_FOCUS_OUT,
                                    cb_img_focus_update, name);
  }

  if (id)
    evas_object_name_set (image, id);
}

Evas_Object *
image_new (char *id, int focusable,
           char *name, char *fname, int layer,
           char *x, char *y, char *w, char *h)
{
  Evas_Object *image;
  
  image = evas_object_image_add (omc->evas);

  image_set (image, id, focusable, name, fname, layer, x, y, w, h);
  
  return image;
}
