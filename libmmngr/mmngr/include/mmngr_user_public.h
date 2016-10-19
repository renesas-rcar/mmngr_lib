/*
 * Copyright (c) 2015-2016 Renesas Electronics Corporation
 * Released under the MIT license
 * http://opensource.org/licenses/mit-license.php
 */
#ifndef	__MMNGR_USER_PUBLIC_H__
#define __MMNGR_USER_PUBLIC_H__

#include "mmngr_public_cmn.h"

typedef int MMNGR_ID;

struct MM_FUNC {
	unsigned int func;
	unsigned int type;
	unsigned int attr;
	unsigned int *conf;
};

/* Gen3 API */
int mmngr_alloc_in_user_ext(MMNGR_ID *pid,
			size_t size,
			unsigned int *phard_addr,
			void **puser_virt_addr,
			unsigned int flag,
			void *mem_param);
int mmngr_free_in_user_ext(MMNGR_ID id);
#define mmngr_debug_map_va_ext(a, b, c, d, e) \
		mmngr_share_in_user_ext(a, b, c, d, e)
#define mmngr_debug_unmap_va_ext(a) \
		mmngr_release_in_user_ext(a)
int mmngr_share_in_user_ext(MMNGR_ID *pid,
			size_t size,
			unsigned int hard_addr,
			void **puser_virt_addr,
			void *mem_param);
int mmngr_release_in_user_ext(MMNGR_ID id);

/* Gen2 Legacy API */
int mmngr_alloc_in_user(MMNGR_ID *pid,
			unsigned long size,
			unsigned long *pphy_addr,
			unsigned long *phard_addr,
			unsigned long *puser_virt_addr,
			unsigned long flag);
int mmngr_free_in_user(MMNGR_ID id);
#define mmngr_debug_map_va(a, b, c, d) \
		mmngr_share_in_user(a, b, c, d)
#define mmngr_debug_unmap_va(a) \
		mmngr_release_in_user(a)
int mmngr_share_in_user(MMNGR_ID *pid,
			unsigned long size,
			unsigned long hard_addr,
			unsigned long *puser_virt_addr);
int mmngr_release_in_user(MMNGR_ID id);

#define R_MM_OK			0
#define R_MM_FATAL		-1
#define R_MM_SEQE		-2
#define R_MM_PARE		-3
#define R_MM_NOMEM		-4

#define MMNGR_VA_SUPPORT	MM_KERNELHEAP
#define MMNGR_PA_SUPPORT	MM_CARVEOUT
#define MMNGR_PA_SUPPORT_SSP	MM_CARVEOUT_SSP
#define MMNGR_PA_SUPPORT_LOSSY	MM_CARVEOUT_LOSSY

#define MM_FUNC_LOSSY_DISABLE		0x0
#define MM_FUNC_LOSSY_ENABLE		0x1
#define MM_FUNC_TYPE_LOSSY_AREA		0x1
#define MM_FUNC_TYPE_LOSSY_SHADOW	0x2
#define MM_FUNC_FMT_LOSSY_YUVPLANAR	0x1
#define MM_FUNC_FMT_LOSSY_YUV422INTLV	0x2
#define MM_FUNC_FMT_LOSSY_ARGB8888	0x3
#define MM_FUNC_STAT_LOSSY_NOT_SUPPORT	0x0
#define MM_FUNC_STAT_LOSSY_SUPPORT	0x1

#endif	/* __MMNGR_USER_PUBLIC_H__ */
