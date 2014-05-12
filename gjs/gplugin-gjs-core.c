/*
 * Copyright (C) 2011-2014 Gary Kramlich <grim@reaperworld.com>
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

#include <gplugin.h>
#include <gplugin-native.h>

#include "gplugin-gjs-loader.h"
#include "gplugin-gjs-plugin.h"

G_MODULE_EXPORT GPluginPluginInfo *
gplugin_query(GPLUGIN_UNUSED GError **error) {
	const gchar * const authors[] = {
		"Gary Kramlich <grim@reaperworld.com>",
		NULL
	};

	return gplugin_plugin_info_new(
		"gplugin/gjs-loader",
		GPLUGIN_NATIVE_PLUGIN_ABI_VERSION,
		"internal", TRUE,
		"load-on-query", TRUE,
		"name", "Gjs JavaScript Plugin Loader",
		"version", GPLUGIN_VERSION,
		"license-id", "GPL3",
		"summary", "A plugin that can load Gjs JavaScript plugins",
		"description", "This plugin allows the loading of plugins written in "
		               "Gjs JavaScript.",
		"authors", authors,
		"website", GPLUGIN_WEBSITE,
		"category", "loaders",
		NULL
	);
}

G_MODULE_EXPORT gboolean
gplugin_load(GPluginNativePlugin *plugin,
             GPLUGIN_UNUSED GError **error)
{
	gplugin_gjs_loader_register(plugin);
	gplugin_gjs_plugin_register(plugin);

	gplugin_manager_register_loader(gplugin_gjs_loader_get_type());

	return TRUE;
}

G_MODULE_EXPORT gboolean
gplugin_unload(GPLUGIN_UNUSED GPluginNativePlugin *plugin,
               GPLUGIN_UNUSED GError **error)
{
	return FALSE;
}

