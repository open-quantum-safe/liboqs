/*
 *
 *	@doc RESOLVE
 *
 * @module inetaton.c  |
 *
 *      from the BIND 4.9.x inetaddr.c
 *
 *	  Contains implementation of inet_aton

 *	  WSHelper DNS/Hesiod Library for WINSOCK
 *
 */

/*
 * Copyright (c) 1983, 1990 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)inet_addr.c	5.11 (Berkeley) 12/9/91";
#endif /* LIBC_SCCS and not lint */

#include <windows.h>
#include <winsock.h>
#include <ctype.h>


/*
	converts a string containing an (Ipv4) Internet Protocol dotted address into a proper address for the in_addr structure

	\param[in]	cp Null-terminated character string representing a number expressed in the
	Internet standard ".'' (dotted) notation.
	\param[in, out] addr pointer to the in_addr structure. The s_addr memeber will be populated


	\retval Returns 1 if the address is valid, 0 if not.

 */
unsigned long
WINAPI
inet_aton(register const char *cp, struct in_addr *addr)
{
    register u_long val, base;
	ULONG_PTR n;
    register char c;
    u_long parts[4], *pp = parts;

    for (;;) {
        /*
         * Collect number up to ``.''.
         * Values are specified as for C:
         * 0x=hex, 0=octal, other=decimal.
         */
        val = 0; base = 10;
        if (*cp == '0') {
            if (*++cp == 'x' || *cp == 'X')
                base = 16, cp++;
            else
                base = 8;
        }
        while ((c = *cp) != '\0') {
            if (isascii(c) && isdigit(c)) {
                val = (val * base) + (c - '0');
                cp++;
                continue;
            }
            if (base == 16 && isascii(c) && isxdigit(c)) {
                val = (val << 4) +
                    (c + 10 - (islower(c) ? 'a' : 'A'));
                cp++;
                continue;
            }
            break;
        }
        if (*cp == '.') {
            /*
             * Internet format:
             *	a.b.c.d
             *	a.b.c	(with c treated as 16-bits)
             *	a.b	(with b treated as 24 bits)
             */
            if (pp >= parts + 3 || val > 0xff)
                return (0);
            *pp++ = val, cp++;
        } else
            break;
    }
    /*
     * Check for trailing characters.
     */
    if (*cp && (!isascii(*cp) || !isspace(*cp)))
        return (0);
    /*
     * Concoct the address according to
     * the number of parts specified.
     */
    n = pp - parts + 1;
    switch (n) {

    case 1:				/* a -- 32 bits */
        break;

    case 2:				/* a.b -- 8.24 bits */
        if (val > 0xffffff)
            return (0);
        val |= parts[0] << 24;
        break;

	case 3:				/* a.b.c -- 8.8.16 bits */
            if (val > 0xffff)
                return (0);
            val |= (parts[0] << 24) | (parts[1] << 16);
            break;

	case 4:				/* a.b.c.d -- 8.8.8.8 bits */
            if (val > 0xff)
                return (0);
            val |= (parts[0] << 24) | (parts[1] << 16) | (parts[2] << 8);
            break;
    }
    if (addr)
        addr->s_addr = htonl(val);
    return (1);
}
