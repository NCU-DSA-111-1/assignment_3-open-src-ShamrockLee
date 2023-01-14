

#include "bench_cases.h"

#include <adaptive_model.h> // From arcd examples
#include <arcd.h>

#include <stdlib.h>

static void do_write(const arcd_buf_t buf, const unsigned buf_bits,
		     void *const io)
{
	(void)buf_bits;
	FILE *const pf_w = (FILE *)io;
	fwrite(&buf, sizeof(buf), 1, pf_w);
}

static unsigned do_read(arcd_buf_t *const p_buf, void *const io)
{
	FILE *const pf_r = (FILE *)io;
	// "<< 3" means "* 8"
	return fread(p_buf, sizeof(*p_buf), 1, pf_r) << 3;
}

typedef unsigned char arcd_symbol_t;
// "<< 3" means "* 8"
static const arcd_char_t n_symbol_combination = 1
						<< (sizeof(arcd_symbol_t) << 3);

int arcd_encode_file(FILE *pf_r, FILE *pf_w)
{
	adaptive_model model;
	adaptive_model_create(&model, n_symbol_combination + 1);

	arcd_enc enc;
	arcd_enc_init(&enc, adaptive_model_getprob, &model, do_write, pf_w);

	arcd_symbol_t sym;
	while (fread(&sym, sizeof(sym), 1, pf_r)) {
		arcd_enc_put(&enc, sym);
	}
	arcd_enc_put(&enc, n_symbol_combination);
	arcd_enc_fin(&enc);

	adaptive_model_free(&model);

	return EXIT_SUCCESS;
}

int arcd_decode_file(FILE *pf_r, FILE *pf_w)
{
	adaptive_model model;
	adaptive_model_create(&model, n_symbol_combination + 1);

	arcd_dec dec;
	arcd_dec_init(&dec, adaptive_model_getch, &model, do_read, pf_r);
	arcd_char_t ch;
	while ((ch = arcd_dec_get(&dec)) != n_symbol_combination) {
		const arcd_symbol_t sym = (arcd_symbol_t)ch;
		fwrite(&sym, sizeof(sym), 1, pf_w);
	}

	adaptive_model_free(&model);

	return EXIT_SUCCESS;
}
