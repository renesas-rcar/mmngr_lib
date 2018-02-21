/*
 * Copyright (c) 2015-2018 Renesas Electronics Corporation
 * Released under the MIT license
 * http://opensource.org/licenses/mit-license.php
 */
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "mmngr_user_public.h"

static void one_test_for_gen2(unsigned long flag, unsigned long size);
static void one_test_for_gen3(unsigned int flag, size_t size);
static void one_test_for_gen3_lossy(unsigned int flag, size_t size,
				    unsigned int tc);
static void test_for_gen2(unsigned long flag, unsigned long size);
static void test_for_gen3(unsigned int flag, unsigned long size);
static void test_for_gen3_lossy(unsigned int flag, unsigned long size,
				unsigned int tc);

static void one_test_for_gen2(unsigned long flag, unsigned long size)
{
	int		ret;
	MMNGR_ID	id;
	MMNGR_ID	ids;
	unsigned long	i;
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
			if (p[i] != 0xCC) {
				printf("<ERROR> Data mismatch happen!!\n");
				break;
			}
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
			if (p[i] != 0xCC) {
				printf("<ERROR> Data mismatch happen!!\n");
				break;
			}
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

static void one_test_for_gen3(unsigned int flag, size_t size)
{
	int		ret;
	MMNGR_ID	id;
	MMNGR_ID	ids;
	size_t		i;
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
			if (p[i] != 0xCC) {
				printf("<ERROR> Data mismatch happen!!\n");
				break;
			}
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
			if (p[i] != 0xCC) {
				printf("<ERROR> Data mismatch happen!!\n");
				break;
			}
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

static void one_test_for_gen3_lossy(unsigned int flag, size_t size,
				    unsigned int tc)
{
	int		ret;
	MMNGR_ID	id;
	MMNGR_ID	ids;
	size_t		i;
	unsigned char	*p;
	unsigned int	hard_addr;
	unsigned int	conf;
	void		*user_virt_addr;
	void		*user_virt_addr_s;
	struct		MM_FUNC mem;

	printf("Lossy START-------------\n");

	if (flag == MMNGR_PA_SUPPORT) {
		ret = mmngr_alloc_in_user_ext(&id, size, &hard_addr,
					      &user_virt_addr, flag, NULL);
		if (ret)
			goto exit;

		printf("hard_addr %x, user_virt_addr %lx\n",
			hard_addr, (unsigned long)user_virt_addr);

		ret = mmngr_free_in_user_ext(id);
		if (ret)
			goto exit;
	}

	if (flag == MMNGR_PA_SUPPORT_LOSSY) {
		memset(&mem, 0, sizeof(mem));
		if (tc == 1) { /* Case 1: Lossy Disable */
			mem.func = MM_FUNC_LOSSY_DISABLE;

			ret = mmngr_alloc_in_user_ext(&id, size, &hard_addr,
						      &user_virt_addr, flag,
						      &mem);
			if (ret)
				goto exit;

			printf("hard_addr %x, user_virt_addr %lx\n",
				hard_addr, (unsigned long)user_virt_addr);

			ret = mmngr_free_in_user_ext(id);
			if (ret)
				goto exit;
		} else if (tc == 2) { /* Case 2: Wrong Parameters */
			mem.func = MM_FUNC_LOSSY_ENABLE;
			mem.type = MM_FUNC_TYPE_LOSSY_SHADOW;

			/* Reset the value for next test */
			hard_addr = 0;
			user_virt_addr = NULL;

			ret = mmngr_alloc_in_user_ext(&id, size, &hard_addr,
						      &user_virt_addr, flag,
						      &mem);
			if (ret != R_MM_PARE)
				goto exit;
		} else if (tc == 3) { /* Case 3: Correct parameter */
			mem.func = MM_FUNC_LOSSY_ENABLE;
			mem.type = MM_FUNC_TYPE_LOSSY_AREA;
			mem.attr = MM_FUNC_FMT_LOSSY_YUVPLANAR;
			mem.conf = &conf;

			/* Reset the value for next test */
			hard_addr = 0;
			user_virt_addr = NULL;
			conf = 2; /* dummy value */

			ret = mmngr_alloc_in_user_ext(&id, size, &hard_addr,
						      &user_virt_addr, flag,
						      &mem);
			if ((ret)
			|| (conf != MM_FUNC_STAT_LOSSY_SUPPORT))
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
				if (p[i] != 0xCC) {
					printf("Data mismatch due to Lossy.\n");
					break;
				}
			}

			ret = mmngr_debug_unmap_va_ext(ids);
			if (ret)
				goto exit;

			ret = mmngr_free_in_user_ext(id);
			if (ret)
				goto exit;
		} else if (tc == 4) { /* Case 4: STAT LOSSY NOT SUPPORT */
			/* FIXME: Assume that YUV422INTLV is not supported */
			mem.func = MM_FUNC_LOSSY_ENABLE;
			mem.type = MM_FUNC_TYPE_LOSSY_AREA;
			mem.attr = MM_FUNC_FMT_LOSSY_YUV422INTLV;
			mem.conf = &conf;

			/* Reset the value for next test */
			hard_addr = 0;
			user_virt_addr = NULL;
			conf = 2; /* dummy value */

			ret = mmngr_alloc_in_user_ext(&id, size, &hard_addr,
						      &user_virt_addr, flag,
						      &mem);
			if ((ret != R_MM_PARE)
			&& (conf != MM_FUNC_STAT_LOSSY_NOT_SUPPORT))
				goto exit;
		} else if (tc == 5) { /* Case 5: Correct parameter */
			mem.func = MM_FUNC_LOSSY_ENABLE;
			mem.type = MM_FUNC_TYPE_LOSSY_AREA;
			mem.attr = MM_FUNC_FMT_LOSSY_ARGB8888;

			mem.conf = &conf;

			/* Reset the value for next test */
			hard_addr = 0;
			user_virt_addr = NULL;
			conf = 2; /* dummy value */

			ret = mmngr_alloc_in_user_ext(&id, size, &hard_addr,
						&user_virt_addr, flag, &mem);
			if ((ret)
			|| (conf != MM_FUNC_STAT_LOSSY_SUPPORT))
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
				if (p[i] != 0xCC) {
					printf("Data mismatch due to Lossy.\n");
					break;
				}
			}

			ret = mmngr_debug_unmap_va_ext(ids);
			if (ret)
				goto exit;

			ret = mmngr_free_in_user_ext(id);
			if (ret)
				goto exit;
		} else if (tc == 6) { /* Case 6: Correct parameter */
			mem.func = MM_FUNC_LOSSY_ENABLE;
			mem.type = MM_FUNC_TYPE_LOSSY_AREA;
			mem.attr = MM_FUNC_FMT_LOSSY_YUV422INTLV;

			mem.conf = &conf;

			/* Reset the value for next test */
			hard_addr = 0;
			user_virt_addr = NULL;
			conf = 2; /* dummy value */

			ret = mmngr_alloc_in_user_ext(&id, size, &hard_addr,
						&user_virt_addr, flag, &mem);
			if ((ret)
			|| (conf != MM_FUNC_STAT_LOSSY_SUPPORT))
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
				if (p[i] != 0xCC) {
					printf("Data mismatch due to Lossy.\n");
					break;
				}
			}

			ret = mmngr_debug_unmap_va_ext(ids);
			if (ret)
				goto exit;

			ret = mmngr_free_in_user_ext(id);
			if (ret)
				goto exit;
		}
	}

	printf("test64 Lossy OK\n");
	printf("Lossy END-------------\n");
	return;

exit:
	printf("test64 Lossy NG\n");
	printf("Lossy END-------------\n");
}

static void test_for_gen2(unsigned long flag, unsigned long size)
{
	one_test_for_gen2(flag, size);
	printf("\n");
	sleep(1);
}

static void test_for_gen3(unsigned int flag, unsigned long size)
{
	one_test_for_gen3(flag, (size_t)size);
	printf("\n");
	sleep(1);
}

static void test_for_gen3_lossy(unsigned int flag, unsigned long size,
				unsigned int tc)
{
	one_test_for_gen3_lossy(flag, (size_t)size, tc);
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
	char	is_support_ssp;

	printf("Test command: mmtp <mem_size><mem_type> <ssp>\n");
		printf("<mem_size><mem_type>: e.g 1024B, 256K, 1M\n");
		printf("<ssp>: [optional] enable SSP test execution\n\n");

	if (argc == 1) {
		printf("WARN: Lack of mem size/mem type. Use 256K.\n\n");
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
			printf("WARN: Incorrect mem type. Use 256K.\n\n");
			size = 256 * 1024;
		}

		if ((argc == 3) && (!strcmp(argv[2], "ssp")))
			is_support_ssp = 1;
		else
			is_support_ssp = 0;
	}
	test_for_gen2(MMNGR_VA_SUPPORT, size);
	test_for_gen2(MMNGR_PA_SUPPORT, size);
	if (is_support_ssp)
		test_for_gen2(MMNGR_PA_SUPPORT_SSP, size);

	test_for_gen3(MMNGR_VA_SUPPORT, size);
	test_for_gen3(MMNGR_PA_SUPPORT, size);
	if (is_support_ssp)
		test_for_gen3(MMNGR_PA_SUPPORT_SSP, size);

	test_for_gen3_lossy(MMNGR_PA_SUPPORT, size, 0);
	test_for_gen3_lossy(MMNGR_PA_SUPPORT_LOSSY, size, 1);
	test_for_gen3_lossy(MMNGR_PA_SUPPORT_LOSSY, size, 2);
	test_for_gen3_lossy(MMNGR_PA_SUPPORT_LOSSY, size, 3);
	/* test_for_gen3_lossy(MMNGR_PA_SUPPORT_LOSSY, size, 4); */
	/* test_for_gen3_lossy(MMNGR_PA_SUPPORT_LOSSY, size, 5); */
	/* test_for_gen3_lossy(MMNGR_PA_SUPPORT_LOSSY, size, 6); */

	return 0;
}
