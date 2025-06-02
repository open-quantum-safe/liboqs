// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/stat.h>

#define OQS_STORE_DIR "tmp"
#define OQS_STORE_PREFIX "/oqs-temp-file-"
#define MAXPATHLEN 128

#if (defined(_WIN32) || defined(__WIN32__))
#include <windows.h>
#endif

static OQS_STATUS oqs_fstore_init(void) {
#ifndef _WIN32
	int ret;
	ret = mkdir(OQS_STORE_DIR, 0755);
	if ((!ret) || (errno == EEXIST)) {
		return OQS_SUCCESS;
	} else {
		return OQS_ERROR;
	}
#else
	if (!CreateDirectory(OQS_STORE_DIR, NULL)) {
		DWORD err = GetLastError();
		if (err == ERROR_ALREADY_EXISTS) {
			return OQS_SUCCESS;
		} else {
			return OQS_ERROR;
		}
	} else {
		return OQS_SUCCESS;
	}
#endif
}

static OQS_STATUS oqs_fstore(const char *fname, const char *mname, uint8_t *data, size_t len) {
	char fpath[MAXPATHLEN];
	strcpy(fpath, OQS_STORE_DIR);
	strcat(fpath, OQS_STORE_PREFIX);
	strcat(fpath, mname);
	strcat(fpath, fname);
	FILE *fp = fopen(fpath, "wb");
	if (!fp) {
		fprintf(stderr, "Couldn't open %s for writing.\n", fpath);
		return OQS_ERROR;
	}
	fwrite(data, len, 1, fp);
	fclose(fp);
	return OQS_SUCCESS;
}

static OQS_STATUS oqs_fload(const char *fname, const char *mname, uint8_t *data, size_t len, size_t *rcvd) {
	size_t len_read = 0, r = 0;
	uint8_t *dr = NULL;
	char fpath[MAXPATHLEN];
	strcpy(fpath, OQS_STORE_DIR);
	strcat(fpath, OQS_STORE_PREFIX);
	strcat(fpath, mname);
	strcat(fpath, fname);
	FILE *fp = fopen(fpath, "rb");
	if (!fp) {
		fprintf(stderr, "Couldn't open %s for reading.\n", fpath);
		return OQS_ERROR;
	}
	do { // assume some OSs don't deliver all data in one go...
		dr = (uint8_t *)(data + len_read);
		r = fread(dr, 1, len - len_read, fp);
		len_read += r;
	} while (r > 0);
	*rcvd = len_read;
	if (len_read <= 0) {
		fprintf(stderr, "Error reading data (operations called in proper sequence?). Expecting %zu. Exiting.\n", len);
		return OQS_ERROR;
	}
	fclose(fp);
	return OQS_SUCCESS;
}

