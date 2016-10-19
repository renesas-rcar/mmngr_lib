/*
 * Copyright (c) 2015-2016 Renesas Electronics Corporation
 * Released under the MIT license
 * http://opensource.org/licenses/mit-license.php
 */
#ifndef __MMNGR_USER_PRIVATE_H__
#define __MMNGR_USER_PRIVATE_H__

#include "mmngr_private_cmn.h"

static int mm_alloc_kh_in_user(MMNGR_ID *pid, size_t size,
				unsigned int *phard_addr,
				void **puser_virt_addr,
				unsigned int flag);
static int mm_free_kh_in_user(MMNGR_ID id);
static int mm_alloc_co_in_user(MMNGR_ID *pid, size_t size,
				unsigned int *phard_addr,
				void **puser_virt_addr,
				unsigned int flag);
static int mm_free_co_in_user(MMNGR_ID id);

#endif	/* __MMNGR_USER_PRIVATE_H__ */
