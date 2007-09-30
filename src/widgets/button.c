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

Evas_Object *
button_new (char *id, int layer, char *name, char *fname,
            void (*cb) (Evas_Object *obj, void *event_info))
{
  Evas_Object *button;

  if (!name || !fname || !id)
    return NULL;
  
  button = evas_object_image_add (omc->evas);

  image_set (button, id, layer, 1, name, fname);
  object_add_default_cb (button, cb);

  evas_object_name_set (button, id);
  
  return button;
}
