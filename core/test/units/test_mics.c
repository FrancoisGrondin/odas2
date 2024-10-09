#include "test_mics.h"

int test_mics(void) {

    const float eps = 0.001f;

    {

        mics_t * mics1 = mics_construct("respeaker_usb");
        mics_t * mics2 = mics_construct("minidsp_uma");
        mics_t * mics3 = mics_construct("wrong_name");

        if (mics1 == NULL) {
            return -1;
        }
        if (mics2 == NULL) {
            return -1;
        }
        if (!(mics3 == NULL)) {
            return -1;
        }

        mics_destroy(mics1);
        mics_destroy(mics2);

    }

    {

        mics_t * mics = mics_construct("respeaker_usb");
        float dist_edge = 0.045f;
        float dist_diag = 0.064f;
        xyz_t direction_top  = xyz_cst(0.0f, 0.0f, 1.0f);
        xyz_t direction_side  = xyz_cst(1.0f, 0.0f, 0.0f);
        float gain_top = 1.0f;
        float gain_side = 0.5f;

        if (!(fabsf(xyz_mag(xyz_sub(mics->mics[0].position, mics->mics[1].position)) - dist_edge) < eps)) {
            return -2;
        }
        if (!(fabsf(xyz_mag(xyz_sub(mics->mics[1].position, mics->mics[2].position)) - dist_edge) < eps)) {
            return -2;
        }
        if (!(fabsf(xyz_mag(xyz_sub(mics->mics[2].position, mics->mics[3].position)) - dist_edge) < eps)) {
            return -2;
        }
        if (!(fabsf(xyz_mag(xyz_sub(mics->mics[3].position, mics->mics[0].position)) - dist_edge) < eps)) {
            return -2;
        }
        if (!(fabsf(xyz_mag(xyz_sub(mics->mics[0].position, mics->mics[2].position)) - dist_diag) < eps)) {
            return -2;
        }
        if (!(fabsf(xyz_mag(xyz_sub(mics->mics[1].position, mics->mics[3].position)) - dist_diag) < eps)) {
            return -2;
        }
        if (!(fabsf(mic_gain(mics->mics[0], direction_top) - gain_top) < eps)) {
            return -2;
        }
        if (!(fabsf(mic_gain(mics->mics[0], direction_side) - gain_side) < eps)) {
            return -2;
        }
        if (!(fabsf(mic_gain(mics->mics[1], direction_top) - gain_top) < eps)) {
            return -2;
        }
        if (!(fabsf(mic_gain(mics->mics[1], direction_side) - gain_side) < eps)) {
            return -2;
        }
        if (!(fabsf(mic_gain(mics->mics[2], direction_top) - gain_top) < eps)) {
            return -2;
        }
        if (!(fabsf(mic_gain(mics->mics[2], direction_side) - gain_side) < eps)) {
            return -2;
        }
        if (!(fabsf(mic_gain(mics->mics[3], direction_top) - gain_top) < eps)) {
            return -2;
        }
        if (!(fabsf(mic_gain(mics->mics[3], direction_side) - gain_side) < eps)) {
            return -2;
        }

        mics_destroy(mics);

    }

    return 0;

}