/*
 * Copyright (c) 2015 Renesas Electronics Corporation
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

#include "mmngr_buf_user_public.h"
#include "mmngr_buf_user_private.h"

/* Gen3 API */
int mmngr_export_start_in_user_ext(int *pid,
			size_t	size,
			unsigned int hard_addr,
			int *pbuf,
			void *mem_param)
{
	int		fd = -1;
	int		ret;
	struct MM_BUF_PARAM	p;

	if ((pid == NULL) || (pbuf == NULL) || (size == 0)) {
		ret = R_MM_PARE;
		goto exit;
	}

	fd = open(DEVFILE, O_RDWR);
	if (fd == -1) {
		perror("MMI open");
		ret = R_MM_FATAL;
		goto exit;
	}

	memset(&p, 0, sizeof(p));
	p.size = size;
	p.hard_addr = hard_addr;
	ret = ioctl(fd, MM_IOC_EXPORT_START, &p);
	if (ret) {
		ret = R_MM_FATAL;
		perror("MMI EXPORT_START");
		goto exit;
	}

	*pid = fd;
	*pbuf = p.buf;

	return R_MM_OK;
exit:
	if (fd != -1) {
		if (close(fd))
			perror("MMI close");
	}
	return ret;
}

int mmngr_export_end_in_user_ext(int id)
{
	int		ret;
	struct MM_BUF_PARAM	p;

	memset(&p, 0, sizeof(p));
	ret = ioctl(id, MM_IOC_EXPORT_END, &p);
	if (ret) {
		ret = R_MM_FATAL;
		perror("MMI EXPORT_END");
		goto exit;
	}

	ret = close(p.buf);
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

int mmngr_import_start_in_user_ext(int *pid,
			size_t *psize,
			unsigned int *phard_addr,
			int buf,
			void *mem_param)
{
	int		fd = -1;
	int		ret;
	struct MM_BUF_PARAM	p;

	if ((pid == NULL) || (psize == NULL)
	|| (phard_addr == NULL)) {
		ret = R_MM_PARE;
		goto exit;
	}

	fd = open(DEVFILE, O_RDWR);
	if (fd == -1) {
		perror("MMI open");
		ret = R_MM_FATAL;
		goto exit;
	}

	memset(&p, 0, sizeof(p));
	p.buf = buf;
	ret = ioctl(fd, MM_IOC_IMPORT_START, &p);
	if (ret) {
		ret = R_MM_FATAL;
		perror("MMI IMPORT_START");
		goto exit;
	}

	*pid = fd;
	*psize = p.size;
	*phard_addr = p.hard_addr;

	return R_MM_OK;
exit:
	if (fd != -1) {
		if (close(fd))
			perror("MMI close");
	}
	return ret;
}

int mmngr_import_end_in_user_ext(int id)
{
	int		ret;
	struct MM_BUF_PARAM	p;

	memset(&p, 0, sizeof(p));
	ret = ioctl(id, MM_IOC_IMPORT_END, &p);
	if (ret) {
		ret = R_MM_FATAL;
		perror("MMI IMPORT_END");
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
int mmngr_export_start_in_user(int *pid,
			unsigned long size,
			unsigned long hard_addr,
			int *pbuf)
{
	int		ret;
	size_t tmp_size = (size_t)size;
	unsigned int tmp_hard_addr = (unsigned int)hard_addr;

	ret = mmngr_export_start_in_user_ext(pid, tmp_size,
					     tmp_hard_addr, pbuf, NULL);

	return ret;
}

int mmngr_export_end_in_user(int id)
{
	int		ret;

	ret = mmngr_export_end_in_user_ext(id);

	return ret;
}

int mmngr_import_start_in_user(int *pid,
			unsigned long *psize,
			unsigned long *phard_addr,
			int buf)
{
	int		ret;
	size_t *tmp_psize = (size_t *)psize;
	unsigned int *tmp_phard_addr = (unsigned int *)phard_addr;

	ret = mmngr_import_start_in_user_ext(pid, tmp_psize,
					     tmp_phard_addr, buf, NULL);

	return ret;
}

int mmngr_import_end_in_user(int id)
{
	int		ret;

	ret = mmngr_import_end_in_user_ext(id);

	return ret;
}
