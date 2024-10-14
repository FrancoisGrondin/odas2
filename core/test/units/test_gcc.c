#include "test_gcc.h"

#include <math.h>
#include <stdio.h>

int test_gcc(void) {

    {

        const float eps_coarse = 0.2f;
        const float eps_fine = 0.025f;

        const unsigned int num_channels = 3;
        const unsigned int num_pairs = 3;
        const unsigned int num_bins = 257;
        const unsigned int num_sources = 1;
        const float delays[3] = { +2.3f, -15.25f, +6.5f };
        const float amplitudes[3] = { +1.0f, +1.0f, +1.0f };

        covs_t * covs = covs_construct("XXs", num_channels, num_bins);
        tdoas_t * tdoas = tdoas_construct("tdoas", num_channels, num_sources);
        gcc_t * gcc = gcc_construct(num_sources, num_channels, num_bins);

        for (unsigned int index_pair = 0; index_pair < num_pairs; index_pair++) {
            for (unsigned int index_bin = 0; index_bin < num_bins; index_bin++) {
                float omega = 2.0 * M_PI * ((float) index_bin) / ((float) ((num_bins - 1) * 2));
                covs->xcorrs[index_pair][index_bin].real = cosf(-1.0f * omega * delays[index_pair]);
                covs->xcorrs[index_pair][index_bin].imag = sinf(-1.0f * omega * delays[index_pair]);
            }
        }

        gcc_process(gcc, covs, tdoas);

        for (unsigned int index_pair = 0; index_pair < num_pairs; index_pair++) {
            if (!(fabsf(tdoas->taus[0][index_pair].delay - delays[index_pair]) < eps_coarse)) {
                return -1;
            }
            if (!(fabsf(tdoas->taus[0][index_pair].amplitude - amplitudes[index_pair]) < eps_fine)) {
                return -1;
            }
        }

        covs_destroy(covs);
        tdoas_destroy(tdoas);
        gcc_destroy(gcc);       

    }

    {

        const float eps_coarse = 0.2f;
        const float eps_fine = 0.025f;

        const unsigned int num_channels = 3;
        const unsigned int num_pairs = 3;
        const unsigned int num_bins = 257;
        const unsigned int num_bins_cropped = 100;
        const unsigned int num_sources = 1;
        const float delays[3] = { +2.3f, -15.25f, +6.5f };
        const float amplitudes[3] = { +0.39f, +0.39f, +0.39f };

        covs_t * covs = covs_construct("XXs", num_channels, num_bins);
        tdoas_t * tdoas = tdoas_construct("tdoas", num_channels, num_sources);
        gcc_t * gcc = gcc_construct(num_sources, num_channels, num_bins);

        for (unsigned int index_pair = 0; index_pair < num_pairs; index_pair++) {
            for (unsigned int index_bin = 0; index_bin < num_bins_cropped; index_bin++) {
                float omega = 2.0 * M_PI * ((float) index_bin) / ((float) ((num_bins - 1) * 2));
                covs->xcorrs[index_pair][index_bin].real = cosf(-1.0f * omega * delays[index_pair]);
                covs->xcorrs[index_pair][index_bin].imag = sinf(-1.0f * omega * delays[index_pair]);
            }
        }

        gcc_process(gcc, covs, tdoas);

        for (unsigned int index_pair = 0; index_pair < num_pairs; index_pair++) {
            if (!(fabsf(tdoas->taus[0][index_pair].delay - delays[index_pair]) < eps_coarse)) {
                return -2;
            }
            if (!(fabsf(tdoas->taus[0][index_pair].amplitude - amplitudes[index_pair]) < eps_fine)) {
                return -2;
            }
        }

        covs_destroy(covs);
        tdoas_destroy(tdoas);
        gcc_destroy(gcc);       

    }    

    {

        const float eps_coarse = 0.2f;

        const unsigned int num_channels = 3;
        const unsigned int num_pairs = 3;
        const unsigned int num_bins = 257;
        const unsigned int num_sources = 2;
        const float delays[2][3] = { { +2.3f, -15.25f, +6.5f }, { +10.7f, +2.25f, -1.5f } };

        covs_t * covs = covs_construct("XXs", num_channels, num_bins);
        tdoas_t * tdoas = tdoas_construct("tdoas", num_channels, num_sources);
        gcc_t * gcc = gcc_construct(num_sources, num_channels, num_bins);

        for (unsigned int index_pair = 0; index_pair < num_pairs; index_pair++) {
            for (unsigned int index_bin = 0; index_bin < num_bins; index_bin++) {
                float omega = 2.0 * M_PI * ((float) index_bin) / ((float) ((num_bins - 1) * 2));
                covs->xcorrs[index_pair][index_bin].real = cosf(-1.0f * omega * delays[0][index_pair]) + 0.2f * cosf(-1.0f * omega * delays[1][index_pair]);
                covs->xcorrs[index_pair][index_bin].imag = sinf(-1.0f * omega * delays[0][index_pair]) + 0.2f * sinf(-1.0f * omega * delays[1][index_pair]);
            }
        }

        gcc_process(gcc, covs, tdoas);

        for (unsigned int index_pair = 0; index_pair < num_pairs; index_pair++) {
            if (!(fabsf(tdoas->taus[0][index_pair].delay - delays[0][index_pair]) < eps_coarse)) {
                return -3;
            }
            if (!(fabsf(tdoas->taus[1][index_pair].delay - delays[1][index_pair]) < eps_coarse)) {
                return -3;
            }
        }

        covs_destroy(covs);
        tdoas_destroy(tdoas);
        gcc_destroy(gcc);       

    }    

    {

        const float eps_coarse = 0.75f;

        const unsigned int num_channels = 3;
        const unsigned int num_pairs = 3;
        const unsigned int num_bins = 257;
        const unsigned int num_bins_cropped = 100;        
        const unsigned int num_sources = 2;
        const float delays[2][3] = { { +2.3f, -15.25f, +6.5f }, { +20.7f, +2.25f, -10.5f } };

        covs_t * covs = covs_construct("XXs", num_channels, num_bins);
        tdoas_t * tdoas = tdoas_construct("tdoas", num_channels, num_sources);
        gcc_t * gcc = gcc_construct(num_sources, num_channels, num_bins);

        for (unsigned int index_pair = 0; index_pair < num_pairs; index_pair++) {
            for (unsigned int index_bin = 0; index_bin < num_bins_cropped; index_bin++) {
                float omega = 2.0 * M_PI * ((float) index_bin) / ((float) ((num_bins - 1) * 2));
                covs->xcorrs[index_pair][index_bin].real = cosf(-1.0f * omega * delays[0][index_pair]) + 0.2f * cosf(-1.0f * omega * delays[1][index_pair]);
                covs->xcorrs[index_pair][index_bin].imag = sinf(-1.0f * omega * delays[0][index_pair]) + 0.2f * sinf(-1.0f * omega * delays[1][index_pair]);
            }
        }

        gcc_process(gcc, covs, tdoas);

        for (unsigned int index_pair = 0; index_pair < num_pairs; index_pair++) {
            if (!(fabsf(tdoas->taus[0][index_pair].delay - delays[0][index_pair]) < eps_coarse)) {
                return -1;
            }
            if (!(fabsf(tdoas->taus[1][index_pair].delay - delays[1][index_pair]) < eps_coarse)) {
                return -1;
            }
        }

        covs_destroy(covs);
        tdoas_destroy(tdoas);
        gcc_destroy(gcc);       

    }    

    return 0;

}