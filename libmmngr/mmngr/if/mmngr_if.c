/*
 * Copyright (c) 2015-2016 Renesas Electronics Corporation
 * Released under the MIT license
 * http://opensource.org/licenses/mit-license.php
 */
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

#include "mmngr_user_public.h"
#include "mmngr_user_private.h"

static int mm_alloc_kh_in_user(MMNGR_ID *pid, size_t size,
				unsigned int *phard_addr,
				void **puser_virt_addr,
				unsigned int flag)
{
	int		fd = -1;
	int		ret;
	void		*virt_addr;
	struct MM_PARAM	p;

	fd = open(DEVFILE, O_RDWR);
	if (fd == -1) {
		perror("MMI open");
		ret = R_MM_FATAL;
		goto exit;
	}

	memset(&p, 0, sizeof(p));
	p.flag = flag;
	p.size = size;
	ret = ioctl(fd, MM_IOC_ALLOC, &p);
	if (ret) {
		if (errno == ENOMEM)
			ret = R_MM_NOMEM;
		else
			ret = R_MM_FATAL;
		perror("MMI ALLOC");
		goto exit;
	}

	virt_addr = mmap((void *)0, size, PROT_READ | PROT_WRITE,
			MAP_SHARED, fd, 0);
	if (virt_addr == MAP_FAILED) {
		perror("MMI mmap()");
		ret = R_MM_FATAL;
		goto exit;
	}

	p.user_virt_addr = (unsigned long)virt_addr;
	ret = ioctl(fd, MM_IOC_SET, &p);
	if (ret) {
		perror("MMI SET");
		ret = R_MM_FATAL;
		goto exit;
	}

	ret = ioctl(fd, MM_IOC_GET, &p);
	if (ret) {
		perror("MMI GET");
		ret = R_MM_FATAL;
		goto exit;
	}

	*puser_virt_addr = virt_addr;
	*phard_addr = p.hard_addr;
	*((MMNGR_ID *)pid) = fd;

	return R_MM_OK;

exit:
	if (fd != -1) {
		if (close(fd))
			perror("MMI close");
	}
	return ret;
}

static int mm_free_kh_in_user(MMNGR_ID id)
{
	int		ret;
	struct MM_PARAM	p;

	memset(&p, 0, sizeof(p));

	ret = ioctl(id, MM_IOC_GET, &p);
	if (ret) {
		perror("MMI GET");
		ret = R_MM_FATAL;
		goto exit;
	}

	ret = munmap((void *)p.user_virt_addr, p.size);
	if (ret) {
		perror("MMI munmap");
		ret = R_MM_FATAL;
		goto exit;
	}

	ret = ioctl(id, MM_IOC_FREE, 0);
	if (ret) {
		perror("MMI FREE");
		ret = R_MM_FATAL;
		goto exit;
	}

	ret = close(id);
	if (ret) {
		perror("MMI close");
		ret = R_MM_FATAL;
		goto exit;
	}

	return R_MM_OK;

exit:
	return ret;
}

static int mm_alloc_co_in_user(MMNGR_ID *pid, size_t size,
				unsigned int *phard_addr,
				void **puser_virt_addr,
				unsigned int flag)
{
	int		fd = -1;
	int		ret;
	struct MM_PARAM	p;

	fd = open(DEVFILE, O_RDWR);
	if (fd == -1) {
		perror("MMI open");
		ret = R_MM_FATAL;
		goto exit;
	}

	memset(&p, 0, sizeof(p));
	p.flag = flag;
	p.size = size;
	ret = ioctl(fd, MM_IOC_ALLOC_CO, &p);
	if (ret) {
		if (errno == ENOMEM)
			ret = R_MM_NOMEM;
		else if (errno == EINVAL)
			ret =  R_MM_PARE;
		else
			ret = R_MM_FATAL;
		perror("MMI ALLOC");
		goto exit;
	}

	ret = ioctl(fd, MM_IOC_GET, &p);
	if (ret) {
		perror("MMI GET");
		ret = R_MM_FATAL;
		goto exit;
	}

	*phard_addr = p.hard_addr;
	*puser_virt_addr = NULL;
	*((MMNGR_ID *)pid) = fd;

	return R_MM_OK;

exit:
	if (fd != -1) {
		if (close(fd))
			perror("MMI close");
	}
	return ret;
}

static int mm_free_co_in_user(MMNGR_ID id)
{
	int		ret;

	ret = ioctl(id, MM_IOC_FREE_CO, 0);
	if (ret) {
		perror("MMI FREE");
		ret = R_MM_FATAL;
		goto exit;
	}

	ret = close(id);
	if (ret) {
		perror("MMI close");
		ret = R_MM_FATAL;
		goto exit;
	}

	return R_MM_OK;
exit:
	return ret;
}

/* Gen3 API */
int mmngr_alloc_in_user_ext(MMNGR_ID *pid, size_t size,
			unsigned int *phard_addr,
			void **puser_virt_addr,
			unsigned int flag,
			void *mem_param)
{
	int ret;
	struct MM_FUNC *p;
	unsigned int *val;

	if ((pid == NULL) || (phard_addr == NULL)
	|| (puser_virt_addr == NULL) || (size == 0)) {
		ret = R_MM_PARE;
		goto exit;
	}

	if ((flag != MM_KERNELHEAP) && (flag != MM_KERNELHEAP_CACHED) && (flag != MM_CARVEOUT)
	&& (flag != MM_CARVEOUT_SSP) && (flag != MM_CARVEOUT_LOSSY)) {
		ret = R_MM_PARE;
		goto exit;
	}

	if (flag == MM_CARVEOUT_LOSSY) {
		if (mem_param == NULL) {
			ret = R_MM_PARE;
			goto exit;
		} else { /* mem_param != NULL */
			p = (struct MM_FUNC *)mem_param;

			if (p->func == MM_FUNC_LOSSY_DISABLE) {
				flag = MM_CARVEOUT;
			} else if (p->func == MM_FUNC_LOSSY_ENABLE) {
				if ((p->type != MM_FUNC_TYPE_LOSSY_AREA)
				|| ((p->attr != MM_FUNC_FMT_LOSSY_YUVPLANAR)
				 && (p->attr != MM_FUNC_FMT_LOSSY_YUV422INTLV)
				 && (p->attr != MM_FUNC_FMT_LOSSY_ARGB8888))) {
					ret = R_MM_PARE;
					goto exit;
				}
			} else {
				ret = R_MM_PARE;
				goto exit;
			}
		}
	}

	if (flag == MM_KERNELHEAP || flag == MM_KERNELHEAP_CACHED) {
		ret = mm_alloc_kh_in_user(pid, size, phard_addr,
					puser_virt_addr, flag);
		if (ret)
			goto exit;
	} else if (flag == MM_CARVEOUT) {
		ret = mm_alloc_co_in_user(pid, size, phard_addr,
					puser_virt_addr, flag);
		if (ret)
			goto exit;
	} else if (flag == MM_CARVEOUT_SSP) {
		ret = mm_alloc_co_in_user(pid, size, phard_addr,
					puser_virt_addr, flag);
		if (ret)
			goto exit;
	} else { /* flag == MM_CARVEOUT_LOSSY */
		flag = (flag & 0xF) | (p->attr << 4);
		ret = mm_alloc_co_in_user(pid, size, phard_addr,
					puser_virt_addr, flag);

		val = (unsigned int *)p->conf;
		if (ret == R_MM_PARE) {
			*val = MM_FUNC_STAT_LOSSY_NOT_SUPPORT;
			goto exit;
		} else if (ret < 0) {
			*val = MM_FUNC_STAT_LOSSY_SUPPORT;
			goto exit;
		} else {
			*val = MM_FUNC_STAT_LOSSY_SUPPORT;
		}
	}
	return R_MM_OK;
exit:
	return ret;
}

int mmngr_free_in_user_ext(MMNGR_ID id)
{
	int		ret;
	struct MM_PARAM	p;

	memset(&p, 0, sizeof(p));

	ret = ioctl(id, MM_IOC_GET, &p);
	if (ret) {
		if (errno == EBADF)
			ret = R_MM_SEQE;
		else
			ret = R_MM_FATAL;
		perror("MMI GET");
		goto exit;
	}

	if (p.flag == MM_KERNELHEAP || p.flag == MM_KERNELHEAP_CACHED) {
		ret = mm_free_kh_in_user(id);
		if (ret)
			goto exit;
	} else if (p.flag == MM_CARVEOUT) {
		ret = mm_free_co_in_user(id);
		if (ret)
			goto exit;
	} else if (p.flag == MM_CARVEOUT_SSP) {
		ret = mm_free_co_in_user(id);
		if (ret)
			goto exit;
	} else if ((p.flag & 0xF) == MM_CARVEOUT_LOSSY) {
		ret = mm_free_co_in_user(id);
		if (ret)
			goto exit;
	}  else {
		ret = R_MM_PARE;
		goto exit;
	}

	return R_MM_OK;
exit:
	return ret;
}

int mmngr_share_in_user_ext(MMNGR_ID *pid,
			size_t size,
			unsigned int hard_addr,
			void **puser_virt_addr,
			void *mem_param)
{
	int		ret;
	int		fd = -1;
	unsigned char	*mp;
	struct MM_PARAM	p;

	if ((pid == NULL) || (puser_virt_addr == NULL)) {
		ret = R_MM_PARE;
		goto exit;
	}

	if (size == 0) {
		ret = R_MM_PARE;
		goto exit;
	}

	fd = open(DEVFILE, O_RDWR);
	if (fd == -1) {
		perror("MMI share");
		ret = R_MM_FATAL;
		goto exit;
	}

	memset(&p, 0, sizeof(p));
	p.size = size;
	p.phy_addr = (unsigned long long)hard_addr;
	ret = ioctl(fd, MM_IOC_SHARE, &p);
	if (ret) {
		ret = R_MM_FATAL;
		goto exit;
	}

	mp = mmap((void *)0, size, PROT_READ | PROT_WRITE,
		MAP_SHARED, fd, 0);
	if (mp == MAP_FAILED) {
		ret = R_MM_FATAL;
		goto exit;
	}

	*puser_virt_addr = mp;
	p.user_virt_addr = (unsigned long)mp;
	ret = ioctl(fd, MM_IOC_SET, &p);
	if (ret) {
		ret = R_MM_FATAL;
		goto exit;
	}

	*((MMNGR_ID *)pid) = fd;

	return R_MM_OK;
exit:
	if (fd != -1) {
		if (close(fd))
			perror("MMI close");
	}
	return ret;
}

int mmngr_release_in_user_ext(MMNGR_ID id)
{
	int		ret;
	unsigned char	*mp;
	struct MM_PARAM	p;

	memset(&p, 0, sizeof(p));

	ret = ioctl(id, MM_IOC_GET, &p);
	if (ret) {
		if (errno == EBADF)
			ret = R_MM_SEQE;
		else
			ret = R_MM_FATAL;
		perror("MMI GET");
		goto exit;
	}

	mp = (unsigned char *)p.user_virt_addr;

	ret = munmap(mp, p.size);
	if (ret) {
		ret = R_MM_FATAL;
		goto exit;
	}

	ret = close(id);
	if (ret) {
		perror("MMI close");
		ret = R_MM_FATAL;
		goto exit;
	}

	return R_MM_OK;
exit:
	return ret;
}

/* Gen2 Legacy API */
int mmngr_alloc_in_user(MMNGR_ID *pid, unsigned long size,
			unsigned long *pphy_addr,
			unsigned long *phard_addr,
			unsigned long *puser_virt_addr,
			unsigned long flag)
{
	int		ret;
	size_t		tmp_size = (size_t)size;
	unsigned int	hard_addr;
	void		*tmp;
	unsigned int	tmp_flag = (unsigned int)flag;

	ret = mmngr_alloc_in_user_ext(pid, tmp_size, &hard_addr,
				      &tmp, tmp_flag, NULL);

	*puser_virt_addr = (unsigned long)tmp;
	*phard_addr = (unsigned long)hard_addr;
	*pphy_addr = -1;

	return ret;
}

int mmngr_free_in_user(MMNGR_ID id)
{
	int		ret;

	ret = mmngr_free_in_user_ext(id);
	return ret;
}

int mmngr_share_in_user(MMNGR_ID *pid,
			unsigned long size,
			unsigned long hard_addr,
			unsigned long *puser_virt_addr)
{
	int		ret;
	size_t		tmp_size = (size_t)size;
	unsigned int	tmp_hard_addr = (unsigned int)hard_addr;
	void		*tmp;

	ret = mmngr_share_in_user_ext(pid, tmp_size, tmp_hard_addr,
				      &tmp, NULL);

	*puser_virt_addr = (unsigned long)tmp;

	return ret;
}

int mmngr_release_in_user(MMNGR_ID id)
{
	int		ret;

	ret = mmngr_release_in_user_ext(id);
	return ret;
}

int mmngr_inval(MMNGR_ID id, size_t offset, size_t len)
{
	int			ret;
	struct MM_CACHE_PARAM	cachep;

	cachep.offset = offset;
	cachep.len = len;

	ret = ioctl(id, MM_IOC_INVAL, &cachep);
	if (ret) {
		perror("MMI INVAL");
		ret = R_MM_FATAL;
		goto exit;
	}

	return R_MM_OK;

exit:
	return ret;

}

int mmngr_flush(MMNGR_ID id, size_t offset, size_t len)
{
	int			ret;
	struct MM_CACHE_PARAM	cachep;

	cachep.offset = offset;
	cachep.len = len;

	ret = ioctl(id, MM_IOC_FLUSH, &cachep);
	if (ret) {
		perror("MMI INVAL");
		ret = R_MM_FATAL;
		goto exit;
	}

	return R_MM_OK;

exit:
	return ret;
}
