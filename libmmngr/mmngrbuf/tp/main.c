/*
 * Copyright (c) 2015 Renesas Electronics Corporation
 * Released under the MIT license
 * http://opensource.org/licenses/mit-license.php
 */
#include <stdio.h>
#include <stddef.h>
#include <unistd.h>
#include <pthread.h>
#include "mmngr_buf_user_public.h"

int test_for_gen3(void)
{
	int id_export[64];
	int id_import[64];
	int buf;
	int ret;
	size_t size;
	unsigned int hard_addr;
	unsigned long i;

	printf("case 1: export repeat test\n");

	for (i = 0; i < 64; i++) {
		ret = mmngr_export_start_in_user_ext(
				&id_export[0], 1, 2, &buf, NULL);
		if (ret)
			goto exit;

		ret = mmngr_export_end_in_user_ext(id_export[0]);
		if (ret)
			goto exit;
	}


	for (i = 0; i < 64; i++) {
		ret = mmngr_export_start_in_user_ext(
				&id_export[i], 1, 2, &buf, NULL);
		if (ret)
			goto exit;
	}

	for (i = 0; i < 64; i++) {
		ret = mmngr_export_end_in_user_ext(id_export[i]);
		if (ret)
			goto exit;
	}


	printf("case 2: import repeat test\n");

	ret = mmngr_export_start_in_user_ext(
				&id_export[0], 1, 2, &buf, NULL);
	if (ret)
		goto exit;

	for (i = 0; i < 64; i++) {
		ret = mmngr_import_start_in_user_ext(&id_import[0],
					&size, &hard_addr, buf, NULL);
		if (ret)
			goto exit;

		if ((size != 1) && (hard_addr != 2))
			goto exit;

		ret = mmngr_import_end_in_user_ext(id_import[0]);
		if (ret)
			goto exit;
	}

	for (i = 0; i < 64; i++) {
		ret = mmngr_import_start_in_user_ext(&id_import[i],
					&size, &hard_addr, buf, NULL);
		if (ret)
			goto exit;

		if ((size != 1) && (hard_addr != 2))
			goto exit;
	}

	for (i = 0; i < 64; i++) {
		ret = mmngr_import_end_in_user_ext(id_import[i]);
		if (ret)
			goto exit;
	}

	ret = mmngr_export_end_in_user_ext(id_export[0]);
	if (ret)
		goto exit;


	printf("case 3: export and import repeat test\n");

	for (i = 0; i < 64; i++) {
		ret = mmngr_export_start_in_user_ext(&id_export[0],
					i + 1, i + 2, &buf, NULL);
		if (ret)
			goto exit;

		ret = mmngr_import_start_in_user_ext(&id_import[0],
					&size, &hard_addr, buf, NULL);
		if (ret)
			goto exit;

		if ((size != i + 1) && (hard_addr != i + 2))
			goto exit;

		ret = mmngr_import_end_in_user_ext(id_import[0]);
		if (ret)
			goto exit;

		ret = mmngr_export_end_in_user_ext(id_export[0]);
		if (ret)
			goto exit;
	}


	printf("case 4: parameter test\n");

	ret = mmngr_export_start_in_user_ext(
			NULL, 1, 2, &buf, NULL);
	if (ret != R_MM_PARE)
		goto exit;

	ret = mmngr_export_start_in_user_ext(
			&id_export[0], 0, 2, NULL, NULL);
	if (ret != R_MM_PARE)
		goto exit;

	ret = mmngr_export_start_in_user_ext(
			&id_export[0], 1, 2, NULL, NULL);
	if (ret != R_MM_PARE)
		goto exit;

	ret = mmngr_import_start_in_user_ext(
			NULL, &size, &hard_addr, buf, NULL);
	if (ret != R_MM_PARE)
		goto exit;

	ret = mmngr_import_start_in_user_ext(
			&id_import[0], NULL, &hard_addr, buf, NULL);
	if (ret != R_MM_PARE)
		goto exit;

	ret = mmngr_import_start_in_user_ext(
			&id_import[0], &size, NULL, buf, NULL);
	if (ret != R_MM_PARE)
		goto exit;


	printf("dmabuf test64 ok\n");
	return 0;
exit:
	printf("dmabuf test64 ng\n");
	return -1;
}

int test_for_gen2(void)
{
	int id_export[64];
	int id_import[64];
	int buf;
	int ret;
	unsigned long size;
	unsigned long hard_addr;
	unsigned long i;

	printf("case 1: export repeat test\n");

	for (i = 0; i < 64; i++) {
		ret = mmngr_export_start_in_user(&id_export[0], 1, 2, &buf);
		if (ret)
			goto exit;

		ret = mmngr_export_end_in_user(id_export[0]);
		if (ret)
			goto exit;
	}


	for (i = 0; i < 64; i++) {
		ret = mmngr_export_start_in_user(&id_export[i], 1, 2, &buf);
		if (ret)
			goto exit;
	}

	for (i = 0; i < 64; i++) {
		ret = mmngr_export_end_in_user(id_export[i]);
		if (ret)
			goto exit;
	}


	printf("case 2: import repeat test\n");

	ret = mmngr_export_start_in_user(&id_export[0], 1, 2, &buf);
	if (ret)
		goto exit;

	for (i = 0; i < 64; i++) {
		ret = mmngr_import_start_in_user(&id_import[0],
					&size, &hard_addr, buf);
		if (ret)
			goto exit;

		if ((size != 1) && (hard_addr != 2))
			goto exit;

		ret = mmngr_import_end_in_user(id_import[0]);
		if (ret)
			goto exit;
	}

	for (i = 0; i < 64; i++) {
		ret = mmngr_import_start_in_user(&id_import[i],
					&size, &hard_addr, buf);
		if (ret)
			goto exit;

		if ((size != 1) && (hard_addr != 2))
			goto exit;
	}

	for (i = 0; i < 64; i++) {
		ret = mmngr_import_end_in_user(id_import[i]);
		if (ret)
			goto exit;
	}

	ret = mmngr_export_end_in_user(id_export[0]);
	if (ret)
		goto exit;


	printf("case 3: export and import repeat test\n");

	for (i = 0; i < 64; i++) {
		ret = mmngr_export_start_in_user(&id_export[0],
					i + 1, i + 2, &buf);
		if (ret)
			goto exit;

		ret = mmngr_import_start_in_user(&id_import[0],
					&size, &hard_addr, buf);
		if (ret)
			goto exit;

		if ((size != i + 1) && (hard_addr != i + 2))
			goto exit;

		ret = mmngr_import_end_in_user(id_import[0]);
		if (ret)
			goto exit;

		ret = mmngr_export_end_in_user(id_export[0]);
		if (ret)
			goto exit;
	}


	printf("case 4: parameter test\n");

	ret = mmngr_export_start_in_user(NULL, 1, 2, &buf);
	if (ret != R_MM_PARE)
		goto exit;

	ret = mmngr_export_start_in_user(&id_export[0], 0, 2, NULL);
	if (ret != R_MM_PARE)
		goto exit;

	ret = mmngr_export_start_in_user(&id_export[0], 1, 2, NULL);
	if (ret != R_MM_PARE)
		goto exit;

	ret = mmngr_import_start_in_user(NULL, &size, &hard_addr, buf);
	if (ret != R_MM_PARE)
		goto exit;

	ret = mmngr_import_start_in_user(&id_import[0], NULL, &hard_addr, buf);
	if (ret != R_MM_PARE)
		goto exit;

	ret = mmngr_import_start_in_user(&id_import[0], &size, NULL, buf);
	if (ret != R_MM_PARE)
		goto exit;


	printf("dmabuf test32 ok\n");
	return 0;
exit:
	printf("dmabuf test32 ng\n");
	return -1;
}

int main(int argc, char *argv[])
{
	test_for_gen2();
	test_for_gen3();

	return 0;
}
