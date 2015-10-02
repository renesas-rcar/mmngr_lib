/*
 * Copyright (c) 2015 Renesas Electronics Corporation
 * Released under the MIT license
 * http://opensource.org/licenses/mit-license.php
 */
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "mmngr_user_public.h"

void one_test_for_gen2(unsigned long flag, unsigned long size)
{
	int		ret;
	MMNGR_ID	id;
	MMNGR_ID	ids;
	int		i;
	unsigned char	*p;
	unsigned long	phy_addr;
	unsigned long	hard_addr;
	unsigned long	user_virt_addr;
	unsigned long	user_virt_addr_s;

	printf("one START-------------\n");

	if (flag == MMNGR_VA_SUPPORT) {
		ret = mmngr_alloc_in_user(&id, size, &phy_addr,
					  &hard_addr, &user_virt_addr, flag);
		if (ret)
			goto exit;

		printf("phy_addr %lx, hard_addr %lx, user_virt_addr %lx\n",
			phy_addr, hard_addr, user_virt_addr);

		p = (unsigned char *)user_virt_addr;
		for (i = 0; i < size; i++)
			p[i] = 0xCC;

		for (i = 0; i < size; i++) {
			if (p[i] != 0xCC)
				goto exit;
		}

		ret = mmngr_free_in_user(id);
		if (ret)
			goto exit;
	}

	if (flag == MMNGR_PA_SUPPORT || flag == MMNGR_PA_SUPPORT_SSP) {
		ret = mmngr_alloc_in_user(&id, size, &phy_addr,
					  &hard_addr, &user_virt_addr, flag);
		if (ret)
			goto exit;

		ret = mmngr_debug_map_va(&ids, size, hard_addr,
					&user_virt_addr_s);
		if (ret)
			goto exit;

		printf(
		   "phy_addr %lx, hard_addr %lx, user_virt_addr %lx, share %lx\n",
		    phy_addr, hard_addr, user_virt_addr, user_virt_addr_s);

		p = (unsigned char *)user_virt_addr_s;

		for (i = 0; i < size; i++)
			p[i] = 0xCC;

		for (i = 0; i < size; i++) {
			if (p[i] != 0xCC)
				goto exit;
		}

		ret = mmngr_debug_unmap_va(ids);
		if (ret)
			goto exit;

		ret = mmngr_free_in_user(id);
		if (ret)
			goto exit;
	}

	printf("test32 OK\n");
	printf("one END-------------\n");
	return;

exit:
	printf("test32 NG\n");
	printf("one END-------------\n");
}

void one_test_for_gen3(unsigned int flag, size_t size)
{
	int		ret;
	MMNGR_ID	id;
	MMNGR_ID	ids;
	int		i;
	unsigned char	*p;
	unsigned int	hard_addr;
	void		*user_virt_addr;
	void		*user_virt_addr_s;

	printf("one START-------------\n");

	if (flag == MMNGR_VA_SUPPORT) {
		ret = mmngr_alloc_in_user_ext(&id, size, &hard_addr,
					&user_virt_addr, flag, NULL);
		if (ret)
			goto exit;

		printf("hard_addr %x, user_virt_addr %lx\n",
			hard_addr, (unsigned long)user_virt_addr);

		p = (unsigned char *)user_virt_addr;

		for (i = 0; i < size; i++)
			p[i] = 0xCC;

		for (i = 0; i < size; i++) {
			if (p[i] != 0xCC)
				goto exit;
		}

		ret = mmngr_free_in_user_ext(id);
		if (ret)
			goto exit;
	}

	if (flag == MMNGR_PA_SUPPORT || flag == MMNGR_PA_SUPPORT_SSP) {
		ret = mmngr_alloc_in_user_ext(&id, size, &hard_addr,
					      &user_virt_addr, flag, NULL);
		if (ret)
			goto exit;

		ret = mmngr_debug_map_va_ext(&ids, size, hard_addr,
					     &user_virt_addr_s, NULL);
		if (ret)
			goto exit;

		printf("hard_addr %x, user_virt_addr %lx, share %lx\n",
			hard_addr, (unsigned long)user_virt_addr,
			(unsigned long)user_virt_addr_s);

		p = (unsigned char *)user_virt_addr_s;

		for (i = 0; i < size; i++)
			p[i] = 0xCC;

		for (i = 0; i < size; i++) {
			if (p[i] != 0xCC)
				goto exit;
		}

		ret = mmngr_debug_unmap_va_ext(ids);
		if (ret)
			goto exit;

		ret = mmngr_free_in_user_ext(id);
		if (ret)
			goto exit;
	}

	printf("test64 OK\n");
	printf("one END-------------\n");
	return;

exit:
	printf("test64 NG\n");
	printf("one END-------------\n");
}

void test_for_gen2(unsigned long flag, unsigned long size)
{
	one_test_for_gen2(flag, size);
	printf("\n");
	sleep(1);
}

void test_for_gen3(unsigned int flag, unsigned long size)
{
	one_test_for_gen3(flag, (size_t)size);
	printf("\n");
	sleep(1);
}

/*
 * TP for MMNGR_PA_SUPPORT_SSP:
 *   It's able to perform this test when MMNGR kernel module
 *   is built with MMNGR_SSP_CONFIG=MMNGR_SSP_ENABLE.
 */
int main(int argc, char *argv[])
{
	char mem_type[1];
	unsigned int length;
	unsigned long size;

	if (argc == 1) {
		printf("\nWARN: Lack of mem size. Use default 256K.\n");
		printf("Test command: mmtp <mem_size><mem_type>\n");
		printf("<mem_size><mem_type>: e.g 1024B, 256K, 1M\n\n");
		size = 256 * 1024;
	} else {
		length = strlen(argv[1]);
		size = strtoul(argv[1], NULL, 0);
		mem_type[0] = argv[1][length-1];

		if (!strcmp(mem_type, "B")) {        /* Size in Bytes */
			;
		} else if (!strcmp(mem_type, "K")) { /* Size in KBytes */
			size *= 1024;
		} else if (!strcmp(mem_type, "M")) { /* Size in MBytes */
			size *= 1048576;               /* 1024 * 1024 */
		} else {
			printf("\nWARN: Lack of mem size. Use default 256K.\n");
			printf("Test command: mmtp <mem_size><mem_type>\n");
			printf("<mem_size><mem_type>: e.g 1024B, 256K, 1M\n\n");
			size = 256 * 1024;
		}
	}

	test_for_gen2(MMNGR_VA_SUPPORT, size);
	test_for_gen2(MMNGR_PA_SUPPORT, size);
	test_for_gen2(MMNGR_PA_SUPPORT_SSP, size);

	test_for_gen3(MMNGR_VA_SUPPORT, size);
	test_for_gen3(MMNGR_PA_SUPPORT, size);
	test_for_gen3(MMNGR_PA_SUPPORT_SSP, size);

	return 0;
}

