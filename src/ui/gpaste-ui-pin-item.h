/*
 * This file is part of GPaste.
 *
 * Copyright (c) 2010-2018, Marc-Antoine Perennou <Marc-Antoine@Perennou.com>
 */

#if !defined (__G_PASTE_H_INSIDE__) && !defined (G_PASTE_COMPILATION)
#error "Only <gpaste.h> can be included directly."
#endif

#ifndef __G_PASTE_UI_PIN_ITEM_H__
#define __G_PASTE_UI_PIN_ITEM_H__

#include <gpaste-ui-item-action.h>

G_BEGIN_DECLS

#define G_PASTE_TYPE_UI_PIN_ITEM (g_paste_ui_pin_item_get_type ())

G_PASTE_FINAL_TYPE (UiPinItem, ui_pin_item, UI_PIN_ITEM, GPasteUiItemAction)

void       g_paste_ui_pin_item_set_pinned (GPasteUiPinItem *self,
                                           gboolean         pinned);
GtkWidget *g_paste_ui_pin_item_new        (GPasteClient    *client);

G_END_DECLS

#endif /*__G_PASTE_UI_PIN_ITEM_H__*/
