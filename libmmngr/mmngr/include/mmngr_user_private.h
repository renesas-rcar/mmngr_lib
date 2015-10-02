/*
 * Copyright (c) 2015 Renesas Electronics Corporation
 * Released under the MIT license
 * http://opensource.org/licenses/mit-license.php
 */
#ifndef __MMNGR_USER_PRIVATE_H__
#define __MMNGR_USER_PRIVATE_H__

#define DEVFILE "/dev/rgnmm"

struct MM_PARAM {
	size_t size;
	unsigned long long	phy_addr;
	unsigned int	hard_addr;
	unsigned long	user_virt_addr;
	unsigned long	kernel_virt_addr;
	unsigned int flag;
};

#define MM_IOC_MAGIC 'm'
#define MM_IOC_ALLOC	_IOWR(MM_IOC_MAGIC, 0, struct MM_PARAM)
#define MM_IOC_FREE	_IOWR(MM_IOC_MAGIC, 1, struct MM_PARAM)
#define MM_IOC_SET	_IOWR(MM_IOC_MAGIC, 2, struct MM_PARAM)
#define MM_IOC_GET	_IOWR(MM_IOC_MAGIC, 3, struct MM_PARAM)
#define MM_IOC_ALLOC_CO	_IOWR(MM_IOC_MAGIC, 4, struct MM_PARAM)
#define MM_IOC_FREE_CO	_IOWR(MM_IOC_MAGIC, 5, struct MM_PARAM)
#define MM_IOC_SHARE	_IOWR(MM_IOC_MAGIC, 6, struct MM_PARAM)

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
