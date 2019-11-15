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
 * WinDRBD boot parameters consist of a single string containing
 * key value pairs. This string is transparent to iPXE it is just
 * passed via ACPI to the Windows kernel and to the WinDRBD driver.
 *
 * For a complete reference, please download
 * the windrbd_boot documentation from http://downloads.linbit.com
 */

#include <config/general.h>
#include <ipxe/acpi.h>

/** DRBD ACPI Table signature */
#define DRBD_SIG ACPI_SIGNATURE ( 'D', 'R', 'B', 'D' )

extern struct acpi_model windrbd_model __acpi_model;

#ifdef SANBOOT_WINDRBD
extern int is_booting_windrbd ( void );
#define IS_BOOTING_WINDRBD() is_booting_windrbd ()
#else
#define IS_BOOTING_WINDRBD() (0)
#endif


#endif /* _IPXE_DRBD_H */
