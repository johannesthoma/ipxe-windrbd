/*
 * Copyright (c) 2019 Johannes Thoma <johannes@johannesthoma.com>.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */


FILE_LICENCE ( GPL2_OR_LATER );

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <byteswap.h>
#include <ipxe/features.h>
#include <ipxe/pci.h>
#include <ipxe/in.h>
#include <ipxe/netdevice.h>
#include <ipxe/ethernet.h>
#include <ipxe/vlan.h>
#include <ipxe/tcpip.h>
#include <ipxe/dhcp.h>
#include <ipxe/iscsi.h>
#include <ipxe/ibft.h>

#include <ipxe/acpi.h>
#include <ipxe/drbd.h>

/** @file
 *
 * DRBD boot parameters
 *
 * We currently just create an empty DRBD acpi table to signal
 * that we are booting via net. Later a complete DRBD config
 * (ideally in drbd.conf format) will be stored in the ACPI
 * DRBD table.
 */

FEATURE ( FEATURE_PROTOCOL, "WinDRBD", DHCP_EB_FEATURE_WINDRBD, 1 );

/**
 * Check if DRBD descriptor is complete
 *
 * @v desc		ACPI descriptor
 * @ret rc		Return status code
 */
static int drbd_complete ( struct acpi_descriptor *desc ) {
	(void) desc;

	return 0;
}

int is_booting_windrbd(void)
{
	char windrbd_root[4096];

	windrbd_root[0] = '\0';
	fetch_string_setting ( NULL, &windrbd_root_setting, windrbd_root,
			       sizeof ( windrbd_root ) );

	return windrbd_root[0] != '\0';
}

/**
 * Install DRBD ACPI table
 *
 * @v install		Installation method
 * @ret rc		Return status code
 */
static int drbd_install ( int ( * install ) ( struct acpi_header *acpi ) ) {
	struct acpi_header *acpi;
	char *config;
	size_t config_len, total_len;
	int rc;
	char windrbd_root[4096];

	rc = 0;

	windrbd_root[0] = '\0';
	fetch_string_setting ( NULL, &windrbd_root_setting, windrbd_root,
			       sizeof ( windrbd_root ) );

	if ( ! windrbd_root[0] ) {
		DBG ( "windrbd-root setting not found, try set windrbd-root <URL>\nor use DHCP option ixpe.windrbd-root to set it.\n");
		return 0;	/* we must not fail, since we might also boot via iSCSI for example. */
	}
	DBG ( "DRBD boot URI is %s\n", windrbd_root );

	config_len = strlen ( windrbd_root );
	total_len = config_len + 1 + sizeof( *acpi );
//	config_len = 0x1c;
//	total_len = config_len + sizeof( *acpi );

	acpi = zalloc ( total_len );
	if ( ! acpi ) {
			/* TODO: */
//		rc = -ENOMEM;
		rc = -1;
		goto err_malloc;
	}
	config = ( ( char *) acpi) + sizeof ( *acpi );
	strcpy ( config, windrbd_root );
//	memset ( config, 0, config_len );

	/* Fill in ACPI header */
	acpi->signature = cpu_to_le32 ( DRBD_SIG );
	acpi->length = cpu_to_le32 ( total_len );
	acpi->revision = 1;

	/* Install ACPI table */
	if ( ( rc = install ( acpi ) ) != 0 ) {
		DBG ( "DRBD could not install ACPI table: %s\n", strerror ( rc ) );
		goto err_install;
	}
/*
	rc = ibft_model.install ( install ); 
	DBG ( "ibft model install returned %d\n", rc );
*/

 err_install:
	free ( acpi );
 err_malloc:
	return rc;
}

/** DRBD model */
struct acpi_model drbd_model __acpi_model = {
	.descs = LIST_HEAD_INIT ( drbd_model.descs ),
	.complete = drbd_complete,
	.install = drbd_install,
};
