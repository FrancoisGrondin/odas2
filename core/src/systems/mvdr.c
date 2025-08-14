#include <systems/mvdr.h>
#include <utils/error.h>

#include <math.h>
#include <stdlib.h>
#include <string.h>

mvdr_t * mvdr_construct(const unsigned int num_channels, const unsigned int num_bins) {

    if (num_channels < 2) {
        odas2_set_error_number(ODAS2_ERROR_MVDR_CONSTRUCT_NUM_CHANNELS);
        return NULL;
    }
    if (num_bins < 1) {
        odas2_set_error_number(ODAS2_ERROR_MVDR_CONSTRUCT_NUM_BINS);
        return NULL;
    }

    mvdr_t * obj = (mvdr_t *) malloc(sizeof(mvdr_t));

    obj->num_channels = num_channels;
    obj->num_bins = num_bins;

    return obj;

}

void mvdr_destroy(mvdr_t * obj) {

    free((void *) obj);

}

int mvdr_process(mvdr_t * obj, const covs_t * covs, weights_t * weights) {

    if (weights->num_sources != 1) {
        odas2_set_error_number(ODAS2_ERROR_MVDR_PROCESS_WEIGHTS_NUM_SOURCES);
        return -1;
    }
    if (obj->num_channels != covs->num_channels) {
        odas2_set_error_number(ODAS2_ERROR_MVDR_PROCESS_COVS_NUM_CHANNELS);
        return -1;
    }
    if (obj->num_channels != weights->num_channels) {
        odas2_set_error_number(ODAS2_ERROR_MVDR_PROCESS_WEIGHTS_NUM_CHANNELS);
        return -1;
    }
    if (obj->num_bins != covs->num_bins) {
        odas2_set_error_number(ODAS2_ERROR_MVDR_PROCESS_COVS_NUM_BINS);
        return -1;
    }
    if (obj->num_bins != weights->num_bins) {
        odas2_set_error_number(ODAS2_ERROR_MVDR_PROCESS_WEIGHTS_NUM_BINS);
        return -1;
    }


    const float eps = 1e-20;

    // Here we have only one target source

    unsigned int index_source = 0;

    //
    // For each frequency bin:
    //
    // w[k] = Phi[k] @ u / Tr{Phi[k]}
    //
    // For instance we have a 4x4 cov matrix for Phi[k]:
    //
    // +-                    -+
    // |  a_1  x_1  x_2  x_3  |
    // |  x_1' a_2  x_4  x_5  |
    // |  x_2' x_4' a_3  x_6  |
    // |  x_3' x_5' x_6' a_4  |
    // +-                    -+
    //
    // We get Tr{Phi[k]} = a_1 + a_2 + a_3 + a_4
    //
    // And then with u as a one-hot vector (we use channel 1 as reference by default):
    //
    // +-   -+
    // |  1  |
    // |  0  |
    // |  0  |
    // |  0  |
    // +-   -+
    //
    // We obtain the following weights w[k]:
    //
    // +-                                -+
    // |  a_1  / (a_1 + a_2 + a_3 + a_4)  |
    // |  x_1' / (a_1 + a_2 + a_3 + a_4)  |
    // |  x_2' / (a_1 + a_2 + a_3 + a_4)  |
    // |  x_3' / (a_1 + a_2 + a_3 + a_4)  |
    // +-                                -+
    //

    #pragma omp parallel for num_threads(get_thread_count())
    for (unsigned int index_bin = 0; index_bin < obj->num_bins; index_bin++) {

        float trace = 0.0f;
        #pragma omp simd
        for (unsigned int index_channel = 0; index_channel < obj->num_channels; index_channel++) {
            trace += covs->acorrs[index_channel][index_bin];
        }

        {

            unsigned int index_channel = 0;
            weights->bins[index_source][0][index_bin] = cplx_cst(covs->acorrs[index_channel][index_bin] / (trace + eps), 0.0f);

        }

        #pragma omp simd
        for (unsigned int index_channel = 1; index_channel < obj->num_channels; index_channel++) {

            unsigned int index_pair = index_channel - 1;
            weights->bins[index_source][index_channel][index_bin].real = covs->xcorrs[index_pair][index_bin].real / (trace + eps);
            weights->bins[index_source][index_channel][index_bin].imag = -1.0f * covs->xcorrs[index_pair][index_bin].imag / (trace + eps);

        }

    }

    return 0;

}
