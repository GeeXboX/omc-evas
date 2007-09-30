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

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include <Evas.h>
#include <dbus/dbus.h>
#include <libhal.h>
#include <libhal-storage.h>

#include "hal_drv_storage.h"

#define HAL_INFO_PRODUCT     "info.product"
#define HAL_VOLUME_DISC_VCD  "volume.disc.is_vcd"
#define HAL_VOLUME_DISC_SVCD "volume.disc.is_svcd"
#define HAL_VOLUME_DISC_DVD  "volume.disc.is_videodvd"

static Evas_List *volumes_list = NULL;

static const struct {
  const char *name;
  volume_disc_t type;
} disc_type_mapping[] = {
  { "Data",             VOLUME_DISC_DATA     },
  { "CDDA",             VOLUME_DISC_CDDA     },
  { "VideoCD",          VOLUME_DISC_VCD      },
  { "Super VideoCD",    VOLUME_DISC_SVCD     },
  { "DVD",              VOLUME_DISC_DVD      },
  { NULL }
};

static const char *
disc_get_type_name (volume_disc_t type)
{
  int i;

  for (i = 0; disc_type_mapping[i].name; i++)
    if (disc_type_mapping[i].type == type)
      return disc_type_mapping[i].name;

  return "";
}

static const struct {
  const char *name;
  LibHalDriveBus bus;
} drv_bus_mapping[] = {
  { "Unknown",             LIBHAL_DRIVE_BUS_UNKNOWN       },
  { "IDE",                 LIBHAL_DRIVE_BUS_IDE           },
  { "SCSI",                LIBHAL_DRIVE_BUS_SCSI          },
  { "USB",                 LIBHAL_DRIVE_BUS_USB           },
  { "FireWire",            LIBHAL_DRIVE_BUS_IEEE1394      },
  { "CCW",                 LIBHAL_DRIVE_BUS_CCW           },
  { NULL }
};

static const char *
drive_get_bus_name (LibHalDriveBus bus)
{
  int i;

  for (i = 0; drv_bus_mapping[i].name; i++)
    if (drv_bus_mapping[i].bus == bus)
      return drv_bus_mapping[i].name;

  return "";
}

static const struct {
  const char *name;
  LibHalDriveType type;
} drv_type_mapping[] = {
  { "Removable Disk",         LIBHAL_DRIVE_TYPE_REMOVABLE_DISK          },
  { "Disk",                   LIBHAL_DRIVE_TYPE_DISK                    },
  { "CDROM",                  LIBHAL_DRIVE_TYPE_CDROM                   },
  { "Floppy",                 LIBHAL_DRIVE_TYPE_FLOPPY                  },
  { "Tape",                   LIBHAL_DRIVE_TYPE_TAPE                    },
  { "CompactFlash",           LIBHAL_DRIVE_TYPE_COMPACT_FLASH           },
  { "MemoryStick",            LIBHAL_DRIVE_TYPE_MEMORY_STICK            },
  { "SmartMedia",             LIBHAL_DRIVE_TYPE_SMART_MEDIA             },
  { "SD/MMC",                 LIBHAL_DRIVE_TYPE_SD_MMC                  },
  { "Camera",                 LIBHAL_DRIVE_TYPE_CAMERA                  },
  { "Portable Audio Player",  LIBHAL_DRIVE_TYPE_PORTABLE_AUDIO_PLAYER   },
  { "ZIP",                    LIBHAL_DRIVE_TYPE_ZIP                     },
  { "JAZ",                    LIBHAL_DRIVE_TYPE_JAZ                     },
  { "FlashKey",               LIBHAL_DRIVE_TYPE_FLASHKEY                },
  { "MagnetoOptical",         LIBHAL_DRIVE_TYPE_MO                      },
  { NULL }
};

static const char *
drive_get_type_name (LibHalDriveType type)
{
  int i;

  for (i = 0; drv_type_mapping[i].name; i++)
    if (drv_type_mapping[i].type == type)
      return drv_type_mapping[i].name;

  return "";
}

storage_volume_t *
storage_volume_new (void)
{
  storage_volume_t *sv = malloc (sizeof (storage_volume_t));

  sv->name = NULL;
  sv->icon = NULL;
  sv->device= NULL;
  sv->mount_point = NULL;
  sv->bus = LIBHAL_DRIVE_BUS_UNKNOWN;
  sv->drive_type = -1;
  sv->disc_type = -1;

  return sv;
}

void
storage_volume_free (storage_volume_t *sv)
{
  if (!sv)
    return;

  if (sv->name)
    free (sv->name);
  if (sv->icon)
    free (sv->icon);
  if (sv->device)
    free (sv->device);
  if (sv->mount_point)
    free (sv->mount_point);
  free (sv);
}

void
storage_volume_print (storage_volume_t *sv)
{
  if (!sv)
    return;

  printf ("%s\n", sv->name);
  printf (" Drive type: %s on %s bus\n",
          drive_get_type_name (sv->drive_type), drive_get_bus_name (sv->bus));
  printf (" Linux Device: %s\n", sv->device);
  if (sv->drive_type != LIBHAL_DRIVE_TYPE_CDROM
      && sv->disc_type != VOLUME_DISC_CDDA) 
  printf (" Mount Point: %s\n", sv->mount_point);
  if (sv->drive_type == LIBHAL_DRIVE_TYPE_CDROM)
    printf (" Disc type: %s\n", disc_get_type_name (sv->disc_type));
  printf ("\n");
}

static void
hal_parse_drive_volume (LibHalContext *halctx, char *udi, int vol_num,
                        LibHalDrive *drv, LibHalDriveBus bus,
                        LibHalDriveType drive_type)
{
  storage_volume_t *sv;
  LibHalVolume *vol;
  DBusError error;
  char name[512];
  char *info = NULL;

  vol = libhal_volume_from_udi (halctx, udi);

  if (!libhal_volume_is_mounted (vol) && !libhal_volume_disc_has_audio (vol))
    return;

  sv = storage_volume_new ();

  dbus_error_init (&error);
  if (libhal_device_property_exists
      (halctx, libhal_volume_get_storage_device_udi (vol),
       HAL_INFO_PRODUCT, &error))
    info = libhal_device_get_property_string
      (halctx, libhal_volume_get_storage_device_udi (vol),
       HAL_INFO_PRODUCT, &error);
  dbus_error_free (&error);
  
  strcpy (name, info ? info : "");
  strcat (name, " - [");
  if (libhal_volume_get_label (vol))
    strcat (name, libhal_volume_get_label (vol));
  else
  {
    char part[16];
    sprintf (part, "%d", vol_num + 1);
    strcat (name, part);
  }
  strcat (name, "]");

  if (info)
    free (info);
  
  sv->name = strdup (name);
  sv->icon = NULL;
  if (libhal_volume_get_device_file (vol))
    sv->device = strdup (libhal_volume_get_device_file (vol));
  if (libhal_volume_get_mount_point (vol))
    sv->mount_point = strdup (libhal_volume_get_mount_point (vol));
  sv->bus = bus;
  sv->drive_type = drive_type;

  if (sv->drive_type == LIBHAL_DRIVE_TYPE_CDROM)
  {
    /* defaults to data disc */
    sv->disc_type = VOLUME_DISC_DATA;

    /* check for audio CD */
    if (libhal_volume_disc_has_audio (vol))
      sv->disc_type = VOLUME_DISC_CDDA;

    /* check for VCD */
    if (libhal_device_property_exists (halctx, udi,
                                       HAL_VOLUME_DISC_VCD, &error))
    {
      if (libhal_device_get_property_bool (halctx, udi,
                                           HAL_VOLUME_DISC_VCD, &error))
        sv->disc_type = VOLUME_DISC_VCD;
    }

    /* check for SVCD */
    if (libhal_device_property_exists (halctx, udi,
                                       HAL_VOLUME_DISC_SVCD, &error))
    {
      if (libhal_device_get_property_bool (halctx, udi,
                                           HAL_VOLUME_DISC_SVCD, &error))
        sv->disc_type = VOLUME_DISC_SVCD;
    }

    /* check for DVD */
    if (libhal_device_property_exists (halctx, udi,
                                       HAL_VOLUME_DISC_DVD, &error))
    {
      if (libhal_device_get_property_bool (halctx, udi,
                                           HAL_VOLUME_DISC_DVD, &error))
        sv->disc_type = VOLUME_DISC_DVD;
    }
  }

  volumes_list = evas_list_append (volumes_list, sv);
  storage_volume_print (sv);

  libhal_volume_free (vol);
}

static void
hal_parse_cd_drive (LibHalContext *halctx, char *udi,
                    LibHalDrive *drv, LibHalDriveBus bus)
{
  int i, num_volumes;
  char **volumes;
  
  if (!libhal_drive_is_media_detected (drv))
    return;

  volumes = libhal_drive_find_all_volumes (halctx, drv, &num_volumes);

  for (i = 0; i < num_volumes; i++)
    hal_parse_drive_volume (halctx, volumes[i], i, drv,
                            bus, LIBHAL_DRIVE_TYPE_CDROM);
}

static void
hal_parse_disk_drive (LibHalContext *halctx, char *udi,
                      LibHalDrive *drv, LibHalDriveBus bus,
                      LibHalDriveType drive_type)
{
  int i, num_volumes;
  char **volumes;
  
  volumes = libhal_drive_find_all_volumes (halctx, drv, &num_volumes);

  for (i = 0; i < num_volumes; i++)
    hal_parse_drive_volume (halctx, volumes[i], i, drv, bus, drive_type);
}

static void
hal_parse_drive (LibHalContext *halctx, char *udi)
{
  LibHalDrive *drv;
  LibHalDriveBus bus;
  LibHalDriveType drive_type;
  
  if (!udi)
    return;
  
  drv = libhal_drive_from_udi (halctx, udi);
  if (!drv)
    return;

  bus = libhal_drive_get_bus (drv);
  drive_type = libhal_drive_get_type (drv);
  
  if (drive_type == LIBHAL_DRIVE_TYPE_CDROM)
    hal_parse_cd_drive (halctx, udi, drv, bus);
  else
    hal_parse_disk_drive (halctx, udi, drv, bus, drive_type);
  
  libhal_drive_free (drv);
}

Evas_List *
hal_get_storage_volumes (void)
{
  Evas_List *list;
  LibHalContext *halctx;
  DBusError error;
  DBusConnection *conn;
  int i, num_devices;
  char **device_list;

  if (volumes_list)
  {
    for (list = volumes_list; list; list = list->next)
    {
      storage_volume_t *sv = NULL;

      sv = (storage_volume_t *) list->data;
      if (!sv)
        continue;

      storage_volume_free (sv);
      volumes_list = evas_list_remove_list (volumes_list, volumes_list);
    }
    free (volumes_list);
    volumes_list = NULL;
  }
  
  dbus_error_init (&error);
  conn = dbus_bus_get (DBUS_BUS_SYSTEM, &error);
  
  halctx = libhal_ctx_new ();
  libhal_ctx_set_dbus_connection (halctx, conn);
  libhal_ctx_init (halctx, &error);

  device_list = libhal_get_all_devices (halctx, &num_devices, &error);

  for (i = 0; i < num_devices; i++)
    hal_parse_drive (halctx, device_list[i]);
  libhal_free_string_array (device_list);
  
  libhal_ctx_shutdown (halctx, &error);
  libhal_ctx_free (halctx);
  dbus_connection_unref (conn);
  dbus_error_free (&error);
  
  return volumes_list;
}
