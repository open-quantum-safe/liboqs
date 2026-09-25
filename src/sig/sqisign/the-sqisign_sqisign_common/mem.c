// SPDX-License-Identifier: Apache-2.0

#include <mem.h>
#include <oqs/common.h>

void
sqisign_secure_free(void *mem, size_t size)
{
    OQS_MEM_secure_free(mem, size);
}
void
sqisign_secure_clear(void *mem, size_t size)
{
    OQS_MEM_cleanse(mem, size);
}
