#include <systems/fcc.h>

#include <math.h>
#include <stdlib.h>
#include <string.h>

//static float * fcc_taus(void);
//static float ** fcc_bases(void);
//static cplx_t ** fcc_dicts(void);

fcc_t * fcc_construct(const unsigned int num_sources, const unsigned int num_channels, const unsigned int num_bins) {

	return NULL;

}

void fcc_destroy(fcc_t * obj) {

}

int fcc_process(fcc_t * obj, const covs_t * covs, tdoas_t * tdoas) {

	return -1;

}

/*
static float * fcc_taus(void) {

	return (float *) NULL;

}

static float ** fcc_bases(void) {

	return (float **) NULL;

}

static cplx_t ** fcc_dicts(void) {

	return (cplx_t **) NULL;

}
*/