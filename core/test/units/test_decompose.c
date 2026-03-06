#include "test_decompose.h"

int test_decompose(void) {

    const float eps = 0.001f;

    {

        const unsigned int num_diags = 3;
        const unsigned int num_trius = 3;

        const float in_diag[3] = { 4.0f, 37.0f, 98.0f };
        const float in_triu[3] = { 12.0f, -16.0f, -43.0f };

        float out_diag[3];
        float out_triu[3];

        const float rtn_diag[3] = { +49.361111f, +3.777777f, +0.111111f };
        const float rtn_triu[3] = { -13.555556f, +2.111111f, -0.555556f };

        invpds_t * invpds = invpds_construct(num_diags);

        invpds_process(invpds, in_diag, in_triu, out_diag, out_triu);

        for (unsigned int index_diag = 0; index_diag < num_diags; index_diag++) {
            if (!(fabsf(out_diag[index_diag] - rtn_diag[index_diag]) < eps)) {
                return -1;
            }
        }

        for (unsigned int index_triu = 0; index_triu < num_trius; index_triu++) {
            if (!(fabsf(out_triu[index_triu] - rtn_triu[index_triu]) < eps)) {
                return -1;
            }            
        }

        invpds_destroy(invpds);

    }

    return 0;

}