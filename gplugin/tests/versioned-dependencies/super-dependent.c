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

G_MODULE_EXPORT GPluginPluginInfo *
gplugin_query(GError **error) {
	const gchar * const dependencies[] = {
		"gplugin/test-no-version",
		"gplugin/test-exact1==1.0",
		"gplugin/test-exact2=1.0",
		"gplugin/test-greater>1.9",
		"gplugin/test-greater-equal>=2.0",
		"gplugin/test-less<1.0",
		"gplugin/test-less-equal<=1.0",
		"gplugin/foo|gplugin/bar",
		"gplugin/foo|gplugin/baz>=2.0",
		"gplugin/foo|gplugin/fez<1.9",
		NULL
	};

	return gplugin_plugin_info_new(
		"gplugin/super-dependent",
		0x04030201,
		"name", "super dependent",
		"dependencies", dependencies,
		NULL
	);
}

G_MODULE_EXPORT gboolean
gplugin_load(GPluginNativePlugin *plugin, GError **error) {
	return TRUE;
}

G_MODULE_EXPORT gboolean
gplugin_unload(GPluginNativePlugin *plugin, GError **error) {
	return TRUE;
}
