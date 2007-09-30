/* GeeXboX Open Media Center.
 * Drive/Volume Storage detection using FreeDesktop HAL/DBUS.
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

#ifndef _HAL_DRV_STORAGE_H_
#define _HAL_DRV_STORAGE_H_

#include <libhal.h>
#include <libhal-storage.h>

typedef enum {
  VOLUME_DISC_DATA,
  VOLUME_DISC_CDDA,
  VOLUME_DISC_VCD,
  VOLUME_DISC_SVCD,
  VOLUME_DISC_DVD,
} volume_disc_t;

typedef struct storage_volume_s {
  char *name;
  char *icon;
  char *device;
  char *mount_point;
  LibHalDriveBus bus;
  LibHalDriveType drive_type;
  volume_disc_t disc_type;
} storage_volume_t;

storage_volume_t * storage_volume_new (void);
void storage_volume_free (storage_volume_t *sv);
void storage_volume_print (storage_volume_t *sv);
Evas_List * hal_get_storage_volumes (void);

#endif /* _HAL_DRV_STORAGE_H_ */
