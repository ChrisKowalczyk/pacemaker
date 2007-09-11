/*
 * Copyright (C) 2004 Andrew Beekhof <andrew@beekhof.net>
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef CRM_AIS_COMMON__H
#define CRM_AIS_COMMON__H

#include <glib.h>
#include <string.h>
#include <openais/ais_util.h>
#include <openais/ipc_gen.h>
#include <openais/saAis.h>

#define MAX_NAME	256
#define AIS_IPC_NAME  "ais-crm-ipc"

typedef struct crm_ais_host_s AIS_Host;
typedef struct crm_ais_msg_s AIS_Message;

enum crm_ais_msg_types {
    crm_msg_none,
    crm_msg_ais,
    crm_msg_cib,
    crm_msg_crmd,
    crm_msg_te,
    crm_msg_pe,
    crm_msg_lrmd,
};

struct crm_ais_host_s
{
	uint32_t		id;
	uint32_t		pid;
	gboolean		local;
	enum crm_ais_msg_types	type;
	uint32_t		size;
	char			uname[256];

} __attribute__((packed));

struct crm_ais_msg_s
{
	mar_req_header_t	header __attribute__((aligned(8)));
	uint32_t		id;
	gboolean		is_compressed;
	
	AIS_Host		host;
	AIS_Host		sender;
	
	uint32_t		size;
	uint32_t		compressed_size;
	/* 584 bytes */
	char			data[0];
	
} __attribute__((packed));

static inline const char *msg_type2text(enum crm_ais_msg_types type) 
{
	const char *text = "unknown";
	switch(type) {
		case crm_msg_none:
			text = "unknown";
			break;
		case crm_msg_ais:
			text = "ais";
			break;
		case crm_msg_cib:
			text = "cib";
			break;
		case crm_msg_crmd:
			text = "crmd";
			break;
		case crm_msg_pe:
			text = "pengine";
			break;
		case crm_msg_te:
			text = "tengine";
			break;
		case crm_msg_lrmd:
			text = "lrmd";
			break;
	}
	return text;
}

static inline const char *ais_dest(struct crm_ais_host_s *host) 
{
    if(host->local) {
	return "local";
    } else if(host->size > 0) {
	return host->uname;
    } else {
	return "<all>";
    }
}


#define ais_data_len(msg) (msg->is_compressed?msg->compressed_size:msg->size)

#endif
