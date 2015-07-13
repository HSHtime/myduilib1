// tooltest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <libimobiledevice/libimobiledevice.h>
#include <libimobiledevice/lockdown.h>
#include "common/utils.h"
#include <libimobiledevice/installation_proxy.h>
#include <libimobiledevice/notification_proxy.h>
#include <libimobiledevice/afc.h>

static void print_usage(int argc, char **argv)
{
	int i = 0;
	char *name = NULL;

	name = strrchr(argv[0], '/');
	printf("Usage: %s [OPTIONS]\n", (name ? name + 1 : argv[0]));
	printf("Show information about a connected device.\n\n");
	printf("  -d, --debug\t\tenable communication debugging\n");
	printf("  -s, --simple\t\tuse a simple connection to avoid auto-pairing with the device\n");
	printf("  -u, --udid UDID\ttarget specific device by its 40-digit device UDID\n");
	printf("  -q, --domain NAME\tset domain of query to NAME. Default: None\n");
	printf("  -k, --key NAME\tonly query key specified by NAME. Default: All keys.\n");
	printf("  -x, --xml\t\toutput information as xml plist instead of key/value pairs\n");
	printf("  -h, --help\t\tprints usage information\n");
	printf("\n");
	printf("  Known domains are:\n\n");
	
	
	printf("\n");
	printf("Homepage: <http://libimobiledevice.org>\n");
}
int installer_list(instproxy_client_t &ipc)
{
	int xml_mode = 0;
	plist_t client_opts = instproxy_client_options_new();
	instproxy_client_options_add(client_opts, "ApplicationType", "User", NULL);
	instproxy_error_t err;
	plist_t apps = NULL;
	int i;
	err = instproxy_browse(ipc, client_opts, &apps);
	instproxy_client_options_free(client_opts);
	if (err != INSTPROXY_E_SUCCESS) {
		fprintf(stderr, "ERROR: instproxy_browse returned %d\n", err);
		return -1;
	}
	if (!apps || (plist_get_node_type(apps) != PLIST_ARRAY)) {
		fprintf(stderr,
			"ERROR: instproxy_browse returnd an invalid plist!\n");
		return -1;
	}
	if (xml_mode) {
		char *xml = NULL;
		uint32_t len = 0;

		plist_to_xml(apps, &xml, &len);
		if (xml) {
			puts(xml);
			free(xml);
		}
		plist_free(apps);
		return -1;
	}
	for (i = 0; i < plist_array_get_size(apps); i++) {
		plist_t app = plist_array_get_item(apps, i);
		plist_t p_appid =
			plist_dict_get_item(app, "CFBundleIdentifier");
		char *s_appid = NULL;
		char *s_dispName = NULL;
		char *s_version = NULL;
		plist_t dispName =
			plist_dict_get_item(app, "CFBundleDisplayName");
		plist_t version = plist_dict_get_item(app, "CFBundleVersion");

		if (p_appid) {
			plist_get_string_val(p_appid, &s_appid);
		}
		if (!s_appid) {
			fprintf(stderr, "ERROR: Failed to get APPID!\n");
			break;
		}

		if (dispName) {
			plist_get_string_val(dispName, &s_dispName);
		}
		if (version) {
			plist_get_string_val(version, &s_version);
		}

		if (!s_dispName) {
			s_dispName = _strdup(s_appid);
		}
		if (s_version) {
			printf("%s - %s %s\n", s_appid, s_dispName, s_version);
			free(s_version);
		}
		else {
			printf("%s - %s\n", s_appid, s_dispName);
		}
		free(s_dispName);
		free(s_appid);
	}
	plist_free(apps);
}

int _tmain(int argc, char* argv[])
{
	lockdownd_client_t client = NULL;
	lockdownd_error_t ldret = LOCKDOWN_E_UNKNOWN_ERROR;
	idevice_t device = NULL;
	idevice_error_t ret = IDEVICE_E_UNKNOWN_ERROR;
	int i;
	int simple = 0;
	const char* udid = NULL;
	plist_t node = NULL;
	int model = 0;
	enum ModelEnum{ INFO, INSTALL_LIST, INSTALL_INSTALL,BACKUP };

	np_client_t np = NULL;
	afc_client_t afc = NULL;
	char *appid;
	lockdownd_service_descriptor_t service_descriptor = NULL;
	instproxy_client_t ipc = NULL;
	/* parse cmdline args */
	for (i = 1; i < argc; i++)
	{
		if (!strcmp(argv[i], "-info"))
		{
			model = INFO;
			continue;
		}
		else if (!strcmp(argv[i], "-installer"))
		{
			i++;
			
			if (!argv[i])
			{
				print_usage(argc, argv);
				goto exit;
			}
			else if (!strcmp(argv[i], "-i"))
			{
				i++;
				model = INSTALL_INSTALL;
				if (!argv[i])
				{
					print_usage(argc, argv);
					goto exit;
				}
				else
					appid = argv[i];
				continue;
			}
			else if (!strcmp(argv[i], "-l"))
			{
				i++;
				model = INSTALL_LIST;
				continue;
			}
			else
			{
				print_usage(argc, argv);
				goto exit;
			}
		}
	
	}

		ret = idevice_new(&device, udid);
		if (ret != IDEVICE_E_SUCCESS) {
			if (udid) {
				printf("No device found with udid %s, is it plugged in?\n", udid);
			}
			else {
				printf("No device found, is it plugged in?\n");
			}
			return -1;
		}

		if (model == INFO)
		{
			if (LOCKDOWN_E_SUCCESS != (ldret = simple ?
				lockdownd_client_new(device, &client, "ideviceinfo") :
				lockdownd_client_new_with_handshake(device, &client, "ideviceinfo"))) {
				fprintf(stderr, "ERROR: Could not connect to lockdownd, error code %d\n", ldret);
				idevice_free(device);
				return -1;
			}
			/* run query and output information */
			if (lockdownd_get_value(client, NULL, NULL, &node) == LOCKDOWN_E_SUCCESS) {
				if (node) {
					plist_print_to_stream(node, stdout);
				}
				plist_free(node);
				node = NULL;
			}
		
		}
		else if (model == INSTALL_LIST)
		{
			if (LOCKDOWN_E_SUCCESS != lockdownd_client_new_with_handshake(device, &client, "ideviceinstaller")) {
				fprintf(stderr, "Could not connect to lockdownd. Exiting.\n");
				goto exit;
			}

			if ((lockdownd_start_service
				(client, "com.apple.mobile.notification_proxy",
				&service_descriptor) != LOCKDOWN_E_SUCCESS) || !service_descriptor->port) {
				fprintf(stderr,
					"Could not start com.apple.mobile.notification_proxy!\n");
				goto exit;
			}

			if (np_client_new(device, service_descriptor, &np) != NP_E_SUCCESS) {
				fprintf(stderr, "Could not connect to notification_proxy!\n");
				goto exit;
			}
			if ((lockdownd_start_service
				(client, "com.apple.mobile.installation_proxy",
				&service_descriptor) != LOCKDOWN_E_SUCCESS) || !service_descriptor->port) {
				fprintf(stderr,
					"Could not start com.apple.mobile.installation_proxy!\n");
				goto exit;
			}

			if (instproxy_client_new(device, service_descriptor, &ipc) != INSTPROXY_E_SUCCESS) {
				fprintf(stderr, "Could not connect to installation_proxy!\n");
				goto exit;
			}
			installer_list(ipc);
			
		}
		else if (model == INSTALL_INSTALL)
		{
		}
	exit:	
		if (np) {
			np_client_free(np);
		}
		if (ipc) {
			instproxy_client_free(ipc);
		}
		if (afc) {
			afc_client_free(afc);
		}
		if (client) {
			lockdownd_client_free(client);
		}
		idevice_free(device);
	return 0;
}

