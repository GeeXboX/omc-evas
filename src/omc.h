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

#ifndef _OMC_H_
#define _OMC_H_

#include "config.h"

#include <inttypes.h>
#include <Evas.h>
#include <Ecore.h>
#include <Ecore_Evas.h>

#include "screens/screen.h"

typedef struct omc_s {
  Evas *evas;  
  Ecore_Evas *ee;
  screen_t *scr;            /* current screen */
  uint16_t w;
  uint16_t h;
} omc_t;

omc_t *omc;

void omc_init (void);
void omc_uninit (void);

#endif /* _OMC_H_ */
