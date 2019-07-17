#ifndef _IPXE_DRBD_H
#define _IPXE_DRBD_H

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

/** @file
 *
 * DRBD boot parameters
 *
 * We currently just create an empty DRBD acpi table to signal
 * that we are booting via net. Later a complete DRBD config
 * (ideally in drbd.conf format) will be stored in the ACPI
 * DRBD table.
 */

#include <ipxe/acpi.h>

/** DRBD ACPI Table signature */
#define DRBD_SIG ACPI_SIGNATURE ( 'D', 'R', 'B', 'D' )

extern struct acpi_model drbd_model __acpi_model;

#endif /* _IPXE_DRBD_H */
