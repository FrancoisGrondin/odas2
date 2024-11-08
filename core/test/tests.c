#include <stdio.h>

#include "units/test_beamformer.h"
#include "units/test_cplx.h"
#include "units/test_delaysum.h"
#include "units/test_fft.h"
#include "units/test_gcc.h"
#include "units/test_linalg.h"
#include "units/test_mics.h"
#include "units/test_mixer.h"
#include "units/test_mvdr.h"
#include "units/test_points.h"
#include "units/test_phat.h"
#include "units/test_scm.h"
#include "units/test_ssl.h"
#include "units/test_sst.h"
#include "units/test_steering.h"
#include "units/test_stft.h"
#include "units/test_xyz.h"

static int test(const char * description, const int error);

int main(int argc, char * argv[]) {

    int error = 0;

    error += test("Testing system beamformer........ ", test_beamformer());
    error += test("Testing system delaysum.......... ", test_delaysum());
    error += test("Testing system gcc............... ", test_gcc());
    error += test("Testing system mixer............. ", test_mixer());
    error += test("Testing system mvdr.............. ", test_mvdr());
    error += test("Testing system phat.............. ", test_phat());
    error += test("Testing system scm............... ", test_scm());
    error += test("Testing system ssl............... ", test_ssl());
    error += test("Testing system sst............... ", test_sst());
    error += test("Testing system steering.......... ", test_steering());
    error += test("Testing system stft.............. ", test_stft());
    error += test("Testing type xyz................. ", test_xyz());
    error += test("Testing type cplx................ ", test_cplx());
    error += test("Testing util fft................. ", test_fft());
    error += test("Testing util linalg.............. ", test_linalg());
    error += test("Testing util mics................ ", test_mics());
    error += test("Testing util points.............. ", test_points());

    return error;

}

static int test(const char * description, const int error) {

    printf("%s", description);
    if (error == 0) {
        printf("[\033[1;32mPASSED\033[0m]\n");
    }
    else {
        printf("[\033[1;31mFAILED with error code %d\033[0m]\n", error);
    }

    return error;

}





