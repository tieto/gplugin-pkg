/*
 * Copyright (C) 2011-2013 Gary Kramlich <grim@reaperworld.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <gplugin/gplugin.h>

#include <gplugin-gtk/gplugin-gtk-store.h>
#include <gplugin-gtk/gplugin-gtk-private.h>

#define GPLUGIN_GTK_STORE_GET_PRIVATE(obj) \
	(G_TYPE_INSTANCE_GET_PRIVATE((obj), GPLUGIN_GTK_TYPE_STORE, GPluginGtkStorePrivate))

/******************************************************************************
 * Structs
 *****************************************************************************/
typedef struct {
	gboolean dummy;
} GPluginGtkStorePrivate;

/******************************************************************************
 * Enums
 *****************************************************************************/

/******************************************************************************
 * Globals
 *****************************************************************************/

/******************************************************************************
 * Helpers
 *****************************************************************************/
static void
gplugin_gtk_store_add_plugin(GPluginGtkStore *store, GPluginPlugin *plugin) {
	GtkTreeIter iter;
	GPluginPluginInfo *info = gplugin_plugin_get_info(plugin);
	GString *str = g_string_new("");

	g_string_append_printf(str, "<span font-weight=\"bold\">%s</span>",
	                       gplugin_plugin_info_get_name(info));
	g_string_append_printf(str, " %s\n",
	                       gplugin_plugin_info_get_version(info));
	g_string_append_printf(str, "%s",
	                       gplugin_plugin_info_get_summary(info));

	gtk_list_store_append(GTK_LIST_STORE(store), &iter);
	gtk_list_store_set(GTK_LIST_STORE(store), &iter,
	                   GPLUGIN_GTK_STORE_LOADED_COLUMN, FALSE,
	                   GPLUGIN_GTK_STORE_PLUGIN_COLUMN, g_object_ref(plugin),
	                   GPLUGIN_GTK_STORE_MARKUP_COLUMN, str->str,
	                   -1);

	g_string_free(str, TRUE);
	g_object_unref(G_OBJECT(info));
}

static void
gplugin_gtk_store_add_plugin_by_id(GPluginGtkStore *store, const gchar * id) {
	GSList *plugins = NULL, *l = NULL;

	plugins = gplugin_manager_find_plugins(id);
	for(l = plugins; l; l = l->next)
		gplugin_gtk_store_add_plugin(store, GPLUGIN_PLUGIN(l->data));
	gplugin_manager_free_plugin_list(plugins);
}

/******************************************************************************
 * GObject Stuff
 *****************************************************************************/
G_DEFINE_TYPE(GPluginGtkStore, gplugin_gtk_store, GTK_TYPE_LIST_STORE);

static void
gplugin_gtk_store_constructed(GObject *obj) {
	GList *l, *ids = NULL;

	G_OBJECT_CLASS(gplugin_gtk_store_parent_class)->constructed(obj);

	ids = gplugin_manager_list_plugins();
	for(l = ids; l; l = l->next)
		gplugin_gtk_store_add_plugin_by_id(GPLUGIN_GTK_STORE(obj),
		                                   (const gchar *)l->data);
	g_list_free(ids);
}

static void
gplugin_gtk_store_dispose(GObject *obj) {
	G_OBJECT_CLASS(gplugin_gtk_store_parent_class)->dispose(obj);
}

static void
gplugin_gtk_store_init(GPluginGtkStore *store) {
	GType *types = (GType *)gplugin_gtk_get_store_column_types();

	gtk_list_store_set_column_types(GTK_LIST_STORE(store),
	                                GPLUGIN_GTK_STORE_N_COLUMNS,
	                                types);
}

static void
gplugin_gtk_store_class_init(GPluginGtkStoreClass *klass) {
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);

	g_type_class_add_private(obj_class, sizeof(GPluginGtkStorePrivate));

	obj_class->constructed = gplugin_gtk_store_constructed;
	obj_class->dispose = gplugin_gtk_store_dispose;

}

/******************************************************************************
 * API
 *****************************************************************************/
GPluginGtkStore *
gplugin_gtk_store_new(void) {
	return GPLUGIN_GTK_STORE(g_object_new(GPLUGIN_GTK_TYPE_STORE, NULL));
}

