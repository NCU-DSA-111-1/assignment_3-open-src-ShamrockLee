#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "bench_cases.h"

int refget_file_size_and_stay(FILE *pf_in, long *p_size)
{
	int retval = EXIT_SUCCESS;
	if (retval = fseek(pf_in, 0, SEEK_END))
		return retval;
	*p_size = ftell(pf_in);
	return retval;
}

int refget_file_size(FILE *pf_in, long *p_size)
{
	int retval = EXIT_SUCCESS;
	if (retval = refget_file_size_and_stay(pf_in, p_size))
		return retval;
	if (retval = fseek(pf_in, 0, SEEK_SET))
		return retval;
	return retval;
}

int print_bench(const char *path_file_orig, const size_t *p_casenums,
		const size_t ii_case)
{
	int retval = EXIT_SUCCESS;
	FILE *pf_encoded = NULL;
	{
		FILE *pf_orig = fopen(path_file_orig, "rb");
		if (retval = !pf_orig) {
			return retval;
		}
		rewind(pf_orig);
		if (retval = !(pf_encoded = tmpfile()))
			goto close_orig_after_encoding;
		rewind(pf_encoded);
		const clock_t clock_begin = clock();
		if (retval = a_bench_cases[p_casenums[ii_case]].pfn_encode(
			    pf_orig, pf_encoded))
			goto close_orig_after_encoding;
		const clock_t clock_end = clock();
		const clock_t clock_diff = clock_end - clock_begin;
		printf("%ld, ", clock_diff);
		long size_file_encoded = 0;
		if (retval = refget_file_size(pf_encoded, &size_file_encoded))
			goto close_orig_after_encoding;
		printf("%ld, ", size_file_encoded);
close_orig_after_encoding:
		retval = fclose(pf_orig) || retval;
	}
	if (retval)
		return retval;

	{
		rewind(pf_encoded);
		FILE *pf_decoded = tmpfile();
		if (retval = !pf_decoded)
			goto close_encoded_after_decoding;
		rewind(pf_decoded);
		const clock_t clock_begin = clock();
		if (retval = a_bench_cases[p_casenums[ii_case]].pfn_decode(
			    pf_encoded, pf_decoded))
			goto close_encoded_after_decoding;
		const clock_t clock_end = clock();
		const clock_t clock_diff = clock_end - clock_begin;
		printf("%ld, ", clock_diff);
close_decoded_after_decoding:
		retval = fclose(pf_decoded) || retval;
close_encoded_after_decoding:
		retval = fclose(pf_encoded) || retval;
	}

	return retval;
}

int main(int argc, char *argv[])
{
	if (argc <= 2) {
		fputs("bench: Expect FILE N_ROUND or FILE N_ROUND ICASE1 [ICASE2 ...]\n",
		      stderr);
		return EXIT_FAILURE;
	}
	const char *path_file_orig = argv[1];
	const size_t n_round = (size_t)atol(argv[2]);

	size_t n_cases = argc - 3;
	size_t *p_casenums = NULL;
	if (n_cases) {
		p_casenums = (size_t *)malloc(sizeof(size_t) * n_cases);
		for (size_t i = 0; i < n_cases; ++i)
			p_casenums[i] = (size_t)atol(argv[i + 3]);
	} else {
		n_cases = N_BENCH_CASES_AVAILABLE;
		p_casenums = (size_t *)malloc(sizeof(size_t) * n_cases);
		for (size_t i = 0; i < n_cases; ++i)
			p_casenums[i] = i;
	}

	int retval = EXIT_SUCCESS;

	long size_file_orig = 0;
	{
		FILE *pf_orig = fopen(path_file_orig, "r");
		if (retval = !pf_orig)
			goto close_orig_after_size;
		if (retval = fseek(pf_orig, 0, SEEK_END))
			goto close_orig_after_size;
		size_file_orig = ftell(pf_orig);
		if (retval = size_file_orig < 0)
			goto close_orig_after_size;
close_orig_after_size:
		retval = fclose(pf_orig) || retval;
	}
	if (retval)
		return retval;
	printf("CLOCKS_PER_SEC: %ld, size_file_orig: %ld, n_round: %ld, ",
	       CLOCKS_PER_SEC, size_file_orig, n_round);
	for (size_t ii_case = 0; ii_case < n_cases; ++ii_case) {
		printf("%ld (%s), ", p_casenums[ii_case],
		       a_bench_cases[p_casenums[ii_case]].name);
	}
	putc('\n', stdout);
	puts("encoding time (1/CLOCKS_PER_SEC second), encoded size (byte), decoding time (1/CLOCKS_PER_SEC second),\n");

	for (size_t i_round = 0; i_round < n_round; ++i_round) {
		for (size_t ii_case = 0; ii_case < n_cases; ++ii_case) {
			if (retval = print_bench(path_file_orig, p_casenums,
						 ii_case))
				return retval;
		}
		putc('\n', stdout);
	}
	return retval;
}
