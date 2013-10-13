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

#include <stdio.h>
#include <stdlib.h>

#include <gtk/gtk.h>

#include <gplugin/gplugin.h>
#include <gplugin-gtk/gplugin-gtk.h>

/******************************************************************************
 * Globals
 *****************************************************************************/
static gboolean show_internal = FALSE;
static gboolean add_default_paths = TRUE;
static gchar **paths = NULL;

/******************************************************************************
 * Callbacks
 *****************************************************************************/
static gboolean
window_closed_cb(GtkWidget *w, GdkEvent *e, gpointer d) {
	gtk_main_quit();

	return FALSE;
}

/******************************************************************************
 * Helpers
 *****************************************************************************/
static gboolean
internal_cb(const gchar *n, const gchar *v, gpointer d, GError **e) {
	show_internal = TRUE;

	return TRUE;
}

static gboolean
no_default_cb(const gchar *n, const gchar *v, gpointer d, GError **e) {
	add_default_paths = FALSE;

	return TRUE;
}

static GtkWidget *
create_window(void) {
	GtkWidget *window, *view, *sw;

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "GPlugin Viewer");
	gtk_container_set_border_width(GTK_CONTAINER(window), 12);
	g_signal_connect(G_OBJECT(window), "delete-event",
	                 G_CALLBACK(window_closed_cb), NULL);

	sw = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(sw),
	                               GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(sw),
	                                    GTK_SHADOW_IN);
	gtk_container_add(GTK_CONTAINER(window), sw);

	view = gplugin_gtk_view_new();
	gplugin_gtk_view_set_show_internal(GPLUGIN_GTK_VIEW(view), show_internal);
	gtk_container_add(GTK_CONTAINER(sw), view);

	return window;
}

/******************************************************************************
 * Main Stuff
 *****************************************************************************/
static GOptionEntry entries[] = {
	{
		"internal", 'i', G_OPTION_FLAG_NO_ARG, G_OPTION_ARG_CALLBACK,
		internal_cb, "Show internal plugins",
		NULL,
	}, {
		"no-default-paths", 'D', G_OPTION_FLAG_NO_ARG, G_OPTION_ARG_CALLBACK,
		no_default_cb, "Do not search the default plugin paths",
		NULL,
	}, {
		"path", 'p', 0, G_OPTION_ARG_STRING_ARRAY,
		&paths, "Additional paths to look for plugins",
		"PATH",
	}, {
		NULL
	},
};

gint
main(gint argc, gchar **argv) {
	GError *error = NULL;
	GOptionContext *ctx = NULL;
	GtkWidget *window = NULL;

	gtk_init(&argc, &argv);

	gplugin_init();

	ctx = g_option_context_new("");
	g_option_context_add_main_entries(ctx, entries, NULL);
	g_option_context_add_group(ctx, gtk_get_option_group(TRUE));
	g_option_context_parse(ctx, &argc, &argv, &error);
	g_option_context_free(ctx);

	if(error) {
		fprintf(stderr, "%s\n", error->message);

		g_error_free(error);

		gplugin_uninit();

		return EXIT_FAILURE;
	}

	if(add_default_paths)
		gplugin_manager_add_default_paths();

	if(paths) {
		gint i;

		for(i = 0; paths[i]; i++)
			gplugin_manager_prepend_path(paths[i]);
	}

	gplugin_manager_refresh();

	/* now create and show the window */
	window = create_window();
	gtk_widget_show_all(window);

	gtk_main();

	gplugin_uninit();

	return 0;
}

