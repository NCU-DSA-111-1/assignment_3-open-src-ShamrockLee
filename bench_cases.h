#ifndef __BENCH_CASES_H__
#define __BENCH_CASES_H__

#include <arcd.h>
#include <huffman/huffman.h>

#include <stdio.h>

#include "bench_case_arcd.h"

#define N_BENCH_CASES_AVAILABLE 2

#ifndef BENCH_CASES_VISIBILITY
#define BENCH_CASES_VISIBILITY
#endif

struct bench_case_properties {
	const char *name;
	int (*pfn_encode)(FILE *pf_r, FILE *pf_w);
	int (*pfn_decode)(FILE *pf_r, FILE *pf_w);
};

static const struct bench_case_properties
	a_bench_cases[N_BENCH_CASES_AVAILABLE] = {
		{
			"arcd_mice_adaptime_model",
			arcd_encode_file,
			arcd_decode_file,
		},
		{
			"huffman_drichardson",
			huffman_encode_file,
			huffman_decode_file,
		},
	};

#endif /* __BENCH_CASES_H__ */
