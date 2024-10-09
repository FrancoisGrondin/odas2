#include <stdio.h>

#include "units/test_beamformer.h"
#include "units/test_cplx.h"
#include "units/test_delaysum.h"
#include "units/test_fft.h"
#include "units/test_gcc.h"
#include "units/test_linalg.h"
#include "units/test_mics.h"
#include "units/test_mvdr.h"
#include "units/test_points.h"
#include "units/test_phat.h"
#include "units/test_scm.h"
#include "units/test_ssl.h"
#include "units/test_steering.h"
#include "units/test_stft.h"
#include "units/test_xyz.h"

static int test(const char * description, const int error);

int main(int argc, char * argv[]) {

    int error = 0;

    error += test("Testing xyz type................. ", test_xyz());
    error += test("Testing cplx type................ ", test_cplx());
    error += test("Testing fft util................. ", test_fft());
    error += test("Testing linalg util.............. ", test_linalg());
    error += test("Testing mics util................ ", test_mics());
    error += test("Testing points util.............. ", test_points());
    error += test("Testing beamformer system........ ", test_beamformer());
    error += test("Testing delaysum system.......... ", test_delaysum());
    error += test("Testing gcc system............... ", test_gcc());
    error += test("Testing mvdr system.............. ", test_mvdr());
    error += test("Testing phat system.............. ", test_phat());
    error += test("Testing scm system............... ", test_scm());
    error += test("Testing ssl system............... ", test_ssl());
    error += test("Testing steering system.......... ", test_steering());
    error += test("Testing stft system.............. ", test_stft());

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





