/* -*- C -*- */
/*
 * COPYRIGHT 2019 SEAGATE TECHNOLOGY LIMITED
 *
 * THIS DRAWING/DOCUMENT, ITS SPECIFICATIONS, AND THE DATA CONTAINED
 * HEREIN, ARE THE EXCLUSIVE PROPERTY OF SEAGATE TECHNOLOGY
 * LIMITED, ISSUED IN STRICT CONFIDENCE AND SHALL NOT, WITHOUT
 * THE PRIOR WRITTEN PERMISSION OF SEAGATE TECHNOLOGY LIMITED,
 * BE REPRODUCED, COPIED, OR DISCLOSED TO A THIRD PARTY, OR
 * USED FOR ANY PURPOSE WHATSOEVER, OR STORED IN A RETRIEVAL SYSTEM
 * EXCEPT AS ALLOWED BY THE TERMS OF SEAGATE LICENSES AND AGREEMENTS.
 *
 * YOU SHOULD HAVE RECEIVED A COPY OF SEAGATE'S LICENSE ALONG WITH
 * THIS RELEASE. IF NOT PLEASE CONTACT A SEAGATE REPRESENTATIVE
 * https://www.seagate.com/contacts
 *
 * Original author:Konstantin Nekrasov <konstantin.nekrasov@seagate.com>
 *                 Mandar Sawant <mandar.sawant@seagate.com>
 * Original creation date: 10-Jul-2019
 */

#pragma once

#ifndef __HAX_H__
#define __HAX_H__

#include "lib/mutex.h"
#include <Python.h>

struct m0_halon_interface;

struct hax_context {
	struct m0_halon_interface *hc_hi;
	/**
	 * Guards access to hx->hc_links.
	 *
	 * @see hax_lock(), hax_unlock()
	 */
	struct m0_mutex            hc_mutex;
	struct m0_tl               hc_links;
	bool                       hc_alive;
	PyObject                  *hc_handler;
	bool                       hc_rconfc_initialized;
};

struct hax_link {
	struct m0_ha_link *hxl_link;
	struct m0_tlink    hxl_tlink;
	uint64_t           hxl_magic;
};

struct hax_entrypoint_request {
	struct hax_context *hep_hc;
};

struct hax_msg {
	struct hax_context *hm_hc;
	struct m0_ha_link  *hm_hl;
	struct m0_ha_msg    hm_msg;
};

struct hax_context *init_halink(PyObject *obj, const char *node_uuid);

void destroy_halink(unsigned long long ctx);

int start(unsigned long long ctx, const char *local_rpc_endpoint,
	  const struct m0_fid *process_fid, const struct m0_fid *ha_service_fid,
	  const struct m0_fid *rm_service_fid);

int start_rconfc(unsigned long long   ctx,
		 const struct m0_fid *process_fid);

void test(unsigned long long ctx );
void m0_ha_entrypoint_reply_send(unsigned long long epr,
				 const struct m0_uint128    *req_id,
				 int                         rc,
				 uint32_t                    confd_nr,
				 const struct m0_fid        *confd_fid_data,
				 const char                **confd_eps_data,
				 uint32_t                    confd_quorum,
				 const struct m0_fid        *rm_fid,
				 const char                 *rm_eps);
void m0_ha_failvec_reply_send(unsigned long long hm, struct m0_fid *pool_fid,
			      uint32_t nr_notes);
void m0_ha_nvec_reply_send(unsigned long long hm, struct m0_ha_note *notes, uint32_t nr_notes);
void m0_ha_notify(unsigned long long ctx, struct m0_ha_note *notes, uint32_t nr_notes);
void m0_ha_broadcast_test(unsigned long long ctx);

/*
 * Invokes m0_spiel_filesystem_stats_fetch() and returns a Python object of type
 * hax.types.FsStats
 */
PyObject *m0_ha_filesystem_stats_fetch(unsigned long long ctx);

/* __HAX_H__ */
#endif

/*
 *  Local variables:
 *  c-indentation-style: "K&R"
 *  c-basic-offset: 8
 *  tab-width: 8
 *  fill-column: 80
 *  scroll-step: 1
 *  End:
 */
