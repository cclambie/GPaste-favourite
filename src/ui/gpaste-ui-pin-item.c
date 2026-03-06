/*
 * This file is part of GPaste.
 *
 * Copyright (c) 2010-2018, Marc-Antoine Perennou <Marc-Antoine@Perennou.com>
 */

#include <gpaste-ui-pin-item.h>

struct _GPasteUiPinItem
{
    GPasteUiItemAction parent_instance;
};

typedef struct
{
    GPasteClient *client;
    GtkImage     *icon;
    gchar        *uuid;
    gboolean      pinned;
} GPasteUiPinItemPrivate;

G_PASTE_DEFINE_TYPE_WITH_PRIVATE (UiPinItem, ui_pin_item, G_PASTE_TYPE_UI_ITEM_ACTION)

static void
g_paste_ui_pin_item_update_icon (GPasteUiPinItem *self)
{
    GPasteUiPinItemPrivate *priv = g_paste_ui_pin_item_get_instance_private (self);

    if (priv->pinned)
    {
        gtk_image_set_from_icon_name (priv->icon, "starred-symbolic", GTK_ICON_SIZE_MENU);
        gtk_widget_set_tooltip_text (GTK_WIDGET (self), _("Unpin"));
    }
    else
    {
        gtk_image_set_from_icon_name (priv->icon, "non-starred-symbolic", GTK_ICON_SIZE_MENU);
        gtk_widget_set_tooltip_text (GTK_WIDGET (self), _("Pin"));
    }
}

/**
 * g_paste_ui_pin_item_set_pinned:
 * @self: a #GPasteUiPinItem instance
 * @pinned: whether the item is pinned
 *
 * Set the current pin state for visual feedback
 */
G_PASTE_VISIBLE void
g_paste_ui_pin_item_set_pinned (GPasteUiPinItem *self,
                                gboolean         pinned)
{
    g_return_if_fail (_G_PASTE_IS_UI_PIN_ITEM (self));

    GPasteUiPinItemPrivate *priv = g_paste_ui_pin_item_get_instance_private (self);

    priv->pinned = pinned;
    g_paste_ui_pin_item_update_icon (self);
}

static void
g_paste_ui_pin_item_activate (GPasteUiItemAction *self,
                              GPasteClient       *client,
                              const gchar        *uuid)
{
    GPasteUiPinItemPrivate *priv = g_paste_ui_pin_item_get_instance_private (G_PASTE_UI_PIN_ITEM (self));

    /* Toggle the pinned state */
    gboolean new_pinned = !priv->pinned;

    g_paste_client_set_pinned (client, uuid, new_pinned, NULL, NULL);

    /* Update local state and icon */
    priv->pinned = new_pinned;
    g_paste_ui_pin_item_update_icon (G_PASTE_UI_PIN_ITEM (self));
}

static void
g_paste_ui_pin_item_class_init (GPasteUiPinItemClass *klass)
{
    G_PASTE_UI_ITEM_ACTION_CLASS (klass)->activate = g_paste_ui_pin_item_activate;
}

static void
g_paste_ui_pin_item_init (GPasteUiPinItem *self)
{
    GPasteUiPinItemPrivate *priv = g_paste_ui_pin_item_get_instance_private (self);

    priv->pinned = FALSE;
}

/**
 * g_paste_ui_pin_item_new:
 * @client: a #GPasteClient
 *
 * Create a new instance of #GPasteUiPinItem
 *
 * Returns: a newly allocated #GPasteUiPinItem
 *          free it with g_object_unref
 */
G_PASTE_VISIBLE GtkWidget *
g_paste_ui_pin_item_new (GPasteClient *client)
{
    g_return_val_if_fail (_G_PASTE_IS_CLIENT (client), NULL);

    GtkWidget *self = g_paste_ui_item_action_new (G_PASTE_TYPE_UI_PIN_ITEM, client, "non-starred-symbolic", _("Pin"));
    GPasteUiPinItemPrivate *priv = g_paste_ui_pin_item_get_instance_private (G_PASTE_UI_PIN_ITEM (self));

    /* Get a reference to the icon for later updates */
    GtkWidget *child = gtk_bin_get_child (GTK_BIN (self));
    if (GTK_IS_IMAGE (child))
        priv->icon = GTK_IMAGE (child);

    return self;
}
