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

#include <stdlib.h>

#include <gplugin.h>
#include <gplugin-native.h>

#include <glib.h>

#include <gplugin/gplugin-loader-tests.h>

/******************************************************************************
 * Tests
 *****************************************************************************/
static void
test_broken_depend_plugin_load(void) {
	GPluginPlugin *plugin = NULL;
	GPluginPluginState state;
	GError *error = NULL;

	/* add the test directory to the plugin manager's search paths */
	gplugin_manager_append_path(TEST_DIR);

	/* refresh the plugin manager */
	gplugin_manager_refresh();

	/* find the dependent plugin and make sure it isn't loaded */
	plugin =
		gplugin_manager_find_plugin("gplugin/broken-dependent-native-plugin");
	g_assert(plugin != NULL);

	state = gplugin_plugin_get_state(plugin);
	g_assert_cmpint(state, !=, GPLUGIN_PLUGIN_STATE_LOADED);

	/* now attempt to load the dependent plugin, it's supposed to fail */
	g_assert(!gplugin_manager_load_plugin(plugin, &error));
}

/******************************************************************************
 * Test bad plugins
 *****************************************************************************/
static void
test_query_error(void) {
	GPluginPlugin *plugin = NULL;

	if(g_test_trap_fork(0, G_TEST_TRAP_SILENCE_STDERR)) {
		/* add the test directory to the plugin manager's search paths */
		gplugin_manager_append_path(TEST_BAD_DIR);

		/* refresh the plugin manager */
		gplugin_manager_refresh();

		/* find the query-error plugin */
		plugin = gplugin_manager_find_plugin("gplugin/query-error");
		g_assert(plugin == NULL);
	}

	g_test_trap_assert_failed();
}

/******************************************************************************
 * Main
 *****************************************************************************/
gint
main(gint argc, gchar **argv) {

	g_test_init(&argc, &argv, NULL);

	gplugin_init();

	gplugin_loader_tests_main(NULL, TEST_DIR, "native");

	g_test_add_func("/loaders/native/load/broken_dependent",
	                test_broken_depend_plugin_load);

	/* bad plugin tests */
	g_test_add_func("/loaders/native/error/query",
	                test_query_error);

	return g_test_run();
}

