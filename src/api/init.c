/*****************************************************************************\
 *  init.c - libslurm library initialization
 *****************************************************************************
 *  Copyright (C) SchedMD LLC.
 *
 *  This file is part of Slurm, a resource management program.
 *  For details, see <https://slurm.schedmd.com/>.
 *  Please also read the included file: DISCLAIMER.
 *
 *  Slurm is free software; you can redistribute it and/or modify it under
 *  the terms of the GNU General Public License as published by the Free
 *  Software Foundation; either version 2 of the License, or (at your option)
 *  any later version.
 *
 *  In addition, as a special exception, the copyright holders give permission
 *  to link the code of portions of this program with the OpenSSL library under
 *  certain conditions as described in each individual source file, and
 *  distribute linked combinations including the two. You must obey the GNU
 *  General Public License in all respects for all of the code used other than
 *  OpenSSL. If you modify file(s) with this exception, you may extend this
 *  exception to your version of the file(s), but you are not obligated to do
 *  so. If you do not wish to do so, delete this exception statement from your
 *  version.  If you delete this exception statement from all source files in
 *  the program, then also delete it here.
 *
 *  Slurm is distributed in the hope that it will be useful, but WITHOUT ANY
 *  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 *  FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 *  details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with Slurm; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA.
\*****************************************************************************/

#include "src/common/read_config.h"

#include "src/interfaces/accounting_storage.h"
#include "src/interfaces/auth.h"
#include "src/interfaces/certgen.h"
#include "src/interfaces/cred.h"
#include "src/interfaces/gres.h"
#include "src/interfaces/hash.h"
#include "src/interfaces/select.h"
#include "src/interfaces/tls.h"

extern void slurm_init(const char *conf)
{
	slurm_conf_init(conf);

	if (auth_g_init() != SLURM_SUCCESS)
		fatal("failed to initialize auth plugin");

	if (certgen_g_init() != SLURM_SUCCESS)
		fatal("failed to initialize certgen plugin");

	if (hash_g_init() != SLURM_SUCCESS)
		fatal("failed to initialize hash plugin");

	if (tls_g_init() != SLURM_SUCCESS)
		fatal("failed to initialize tls plugin");

	if (acct_storage_g_init() != SLURM_SUCCESS)
		fatal("failed to initialize the accounting storage plugin");

	if (gres_init() != SLURM_SUCCESS)
		fatal("failed to initialize gres plugin");

	if (cred_g_init() != SLURM_SUCCESS)
		fatal("failed to initialize cred plugin");
}

extern void slurm_fini(void)
{
	cred_g_fini();
	gres_fini();
	acct_storage_g_fini();
	tls_g_fini();
	hash_g_fini();
	auth_g_fini();

	slurm_conf_destroy();
}
