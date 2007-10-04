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
#include <stdio.h>

#include "omc.h"
#include "color.h"
#include "widget.h"

#define TXT_BLOCK_SIZE 32768

/* keep a global copy of this, so it only has to be created once */
static Evas_Smart *smart;

typedef struct _textblock_s
{
  int editable;
  Evas_Object *text;
  Evas_Textblock_Style *style;
} _textblock_t;

/*** smart object handler functions ***/

static void
_textblock_add (Evas_Object *o)
{
  extern omc_t *omc;
  _textblock_t *data;
 
  data = calloc (1, sizeof (_textblock_t));
  if (!data)
    return;

  evas_object_smart_data_set (o, data);

  data->editable = 0;
  
  data->text = evas_object_textblock_add (omc->evas);
  evas_object_smart_member_add (o, data->text);

  data->style = NULL;
}

static void
_textblock_del (Evas_Object *o)
{
  _textblock_t *data;
  
  data = evas_object_smart_data_get (o);

  evas_object_del (data->text);
  if (data->style)
    evas_textblock_style_free (data->style);
  
  free (data);
}

static void
_textblock_move (Evas_Object *o, Evas_Coord x, Evas_Coord y)
{
  _textblock_t *data;
  
  data = evas_object_smart_data_get (o);
  evas_object_move (data->text, x, y);
}

static void
_textblock_resize (Evas_Object *o, Evas_Coord w, Evas_Coord h)
{
  _textblock_t *data;
  
  data = evas_object_smart_data_get (o);

  evas_object_resize (data->text, w, h);
}

static void
_textblock_show (Evas_Object *o)
{
  _textblock_t *data;
  
  data = evas_object_smart_data_get (o);
  evas_object_show (data->text);
}

static void
_textblock_hide (Evas_Object *o)
{
  _textblock_t *data;
  
  data = evas_object_smart_data_get (o);
  evas_object_hide (data->text);
}

static void
_textblock_color_set (Evas_Object *o, int r, int g, int b, int a)
{
  _textblock_t *data;
  
  data = evas_object_smart_data_get (o);
  evas_object_color_set (data->text, r, g, b, a);
}

static void
_textblock_clip_set (Evas_Object *o, Evas_Object *clip)
{
  _textblock_t *data;
  
  data = evas_object_smart_data_get (o);
  evas_object_clip_set (data->text, clip);
}

static void
_textblock_clip_unset (Evas_Object *o)
{
  _textblock_t *data;
  
  data = evas_object_smart_data_get (o);
  evas_object_clip_unset(data->text);
}

static Evas_Smart *
_textblock_smart_get (void)
{
  const Evas_Smart_Class sc = { "textblock", EVAS_SMART_CLASS_VERSION, 
                                _textblock_add, _textblock_del,
                                _textblock_move, _textblock_resize,
                                _textblock_show, _textblock_hide,
                                _textblock_color_set,
                                _textblock_clip_set, _textblock_clip_unset,
                                NULL };
  if (smart)
    return smart;

  smart = evas_smart_class_new (&sc);

  return smart; 
}

/*** callbacks ***/

/* static void */
/* cb_textblock_edit (void *data, Evas *e, Evas_Object *obj, void *event_info) */
/* { */
/*   Evas_Event_Key_Down *event = (Evas_Event_Key_Down *) event_info; */
/*   Evas_Textblock_Cursor *cursor = NULL; */
/*   Evas_Object *cur = (Evas_Object *) data; */
/*   Evas_Coord x = 0, y = 0, cx = 0, cy = 0, cw = 0, ch = 0; */
  
/*   cursor = (Evas_Textblock_Cursor *) evas_object_textblock_cursor_get (obj); */
  
/*   if (!event->keyname) */
/*     return; */

/*   if (!strcmp (event->keyname, "Left")) */
/*     evas_textblock_cursor_char_prev (cursor); */
/*   else if (!strcmp (event->keyname, "Right")) */
/*     evas_textblock_cursor_char_next (cursor); */
/*   else if (!strcmp (event->keyname, "Return")) */
/*     textblock_format (obj, BLK_NEW_LINE); */
/*   else if (!strcmp (event->keyname, "Tab")) */
/*     textblock_format (obj, BLK_NEW_TAB); */
/*   else if (!strcmp (event->keyname, "Delete")) */
/*   { */
/*     if (evas_textblock_cursor_char_next (cursor)) */
/*       evas_textblock_cursor_char_delete (cursor); */
/*   } */
/*   else if (!strcmp (event->keyname, "BackSpace")) */
/*     evas_textblock_cursor_char_delete (cursor); */
/*   else */
/*   { */
/*     if (event->string) */
/*     { */
/*       textblock_add_text (obj, (char *) event->string); */
/*       evas_textblock_cursor_char_prev (cursor); */
/*     } */
/*   } */

/*   evas_object_geometry_get (obj, &x, &y, NULL, NULL); */
/*   evas_textblock_cursor_char_geometry_get (cursor, &cx, &cy, &cw, &ch); */
/*   evas_object_move (cur, cx + x + cw, cy + y); */
/* } */

/*** external API ***/

void
textblock_format (Evas_Object *blk, textblock_blk_type_t format)
{
  Evas_Textblock_Cursor *cursor;
  _textblock_t *data;
  
  data = evas_object_smart_data_get (blk);

  cursor = (Evas_Textblock_Cursor *)
    evas_object_textblock_cursor_get (data->text);

  if (!cursor)
    return;
  
  switch (format)
    {
    case BLK_NEW_LINE:
      evas_textblock_cursor_format_append (cursor, "\n");
      break;
    case BLK_NEW_TAB:
      evas_textblock_cursor_format_append (cursor, "\t");
      break;
    default:
      return;
    }
}

void
textblock_add_text (Evas_Object *blk, char *text)
{
  Evas_Textblock_Cursor *cursor;
  char str[TXT_BLOCK_SIZE];
  unsigned int i, x = 0;
  _textblock_t *data;
  
  if (!text)
    return;

  data = evas_object_smart_data_get (blk);
  
  cursor = (Evas_Textblock_Cursor *)
    evas_object_textblock_cursor_get (data->text);
  if (!cursor)
    return;
  
  bzero (str, TXT_BLOCK_SIZE);
  for (i = 0; i < strlen (text); i++)
  {
    if (text[i] == '\n')
    {
      evas_textblock_cursor_text_append (cursor, str);
      textblock_format (blk, BLK_NEW_LINE);
      bzero (str, TXT_BLOCK_SIZE);
      x = 0;
    }
    else if (text[i] == '\t')
    {
      evas_textblock_cursor_text_append (cursor, str);
      textblock_format (blk, BLK_NEW_TAB);
      bzero (str, TXT_BLOCK_SIZE);
      x = 0;
    }
    else
    {
      str[x] = text[i];
      x++;
    }
  }

  evas_textblock_cursor_text_append (cursor, str);
  evas_textblock_cursor_node_last (cursor);
}

Evas_Object *
textblock_new (char *id, int layer, const style_t *style, char *str,
               int editable, char *halign, char *valign)
{
  char blockStyle[1024];
  Evas_Object *blk;
  _textblock_t *data;

  if (!id)
    return NULL;

  if (!style)
    style = &default_text_style;

  blk = evas_object_smart_add (omc->evas, _textblock_smart_get ());

  data = evas_object_smart_data_get (blk);

  data->editable = editable;
  
  evas_object_layer_set (data->text, layer);

  //evas_object_textblock_cursor_new (data->text);

  sprintf (blockStyle, "DEFAULT='font=%s font_size=%d color=%X%X%XFF " \
           "wrap=word align=%s valign=%s'", style->normal.font, style->normal.font_size,
           style->normal.font_color.r, style->normal.font_color.g, style->normal.font_color.b,
           halign, valign);
  
  data->style = evas_textblock_style_new ();
  evas_textblock_style_set (data->style, blockStyle);
  evas_object_textblock_style_set (data->text, data->style);

  textblock_add_text (blk, str);
  
/*   if (editable) */
/*   { */
/*     Evas_Textblock_Cursor *cursor; */
/*     Evas_Object *cur = NULL; */
/*     Evas_Coord cx = 0, cy = 0, cw = 0, ch = 0; */

/*     cursor = evas_object_textblock_cursor_get (data->text); */
/*     evas_textblock_cursor_node_last (cursor); */
/*     evas_textblock_cursor_char_geometry_get (cursor, &cx, &cy, &cw, &ch); */
  
/*     cur = evas_object_rectangle_add (omc->evas); */
/*     evas_object_move (cur, cx + x2 + cw, cy + y2); */
/*     evas_object_resize (cur, 2, ch); */
/*     evas_object_color_set (cur, cl->r, cl->g, cl->b, cl->a); */
/*     evas_object_layer_set (cur, 0); */
/*     evas_object_show (cur); */

/*     evas_object_focus_set (data->text, 1); */
/*     evas_object_event_callback_add (data->text, EVAS_CALLBACK_KEY_DOWN, */
/*                                     cb_textblock_edit, cur); */
/*   } */

  evas_object_name_set (blk, id);
  
  return blk;
}
