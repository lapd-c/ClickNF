/*
 * util.{cc,hh} -- generic functions
 * Rafael Laufer, Massimo Gallo, Myriana Rifai
 *
 * Copyright (c) 2019 Nokia Bell Labs
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer 
 *    in the documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *
 */

#ifndef CLICK_UTIL_HH
#define CLICK_UTIL_HH

#include <click/string.hh>
#include <linux/types.h>

#define MIN(a,b)     (((a) < (b)) ?    (a)    :    (b))
#define MAX(a,b)     (((a) > (b)) ?    (a)    :    (b))
#define absdiff(a,b) (((a) > (b)) ? ((a)-(b)) : ((b)-(a)))
#define mod(a,b)     (a-((a/b)*b))

int get_shift(String &s);

inline void prefetch0(const volatile void *p) {
	asm volatile ("prefetcht0 %[p]" : : [p] "m" (*(const volatile char*)p));
}

#ifndef MINMAX_H
#define MINMAX_H

/* A single data point for our parameterized min-max tracker */
struct minmax_sample {
	uint32_t	t;	/* time measurement was taken */
	uint32_t	v;	/* value measured */
};

/* State for the parameterized min-max tracker */
struct minmax {
	struct minmax_sample s[3];
};

static inline uint32_t minmax_get(const struct minmax *m)
{
	return m->s[0].v;
}

static inline uint32_t minmax_reset(struct minmax *m, uint32_t t, uint32_t meas)
{
	struct minmax_sample val = { t,  meas };
	m->s[2] = m->s[1] = m->s[0] = val;

	return m->s[0].v;
}

uint32_t minmax_running_max(struct minmax *m, uint32_t win, uint32_t t, uint32_t meas);
uint32_t minmax_running_min(struct minmax *m, uint32_t win, uint32_t t, uint32_t meas);

#endif
#endif
