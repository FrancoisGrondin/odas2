#ifndef __SCM_H
#define __SCM_H

#include "../utils/linalg.h"
#include "../signals/covs.h"
#include "../signals/freqs.h"
#include "../signals/masks.h"

typedef struct scm_t {

    unsigned int num_channels;
    unsigned int num_pairs;
    unsigned int num_bins;
    float alpha;

    cplx_t ** xcorrs;
    float ** acorrs;

} scm_t;

typedef struct iscm_t {

    unsigned int num_channels;
    unsigned int num_pairs;
    unsigned int num_bins;
    float alpha;

    cplx_t ** xcorrs;
    float ** acorrs;

    cmat_t * phiInv;
    hmat_t * phiInvSym;
    cmat_t * X;
    cmat_t * XH;
    cmat_t * phiInv_X;
    hmat_t * phiInv_X_XH_phiInvH;
    cmat_t * XH_phiInv_X;
    hmat_t * beta_phiInv;
    hmat_t * gamma_phiInv_X_XH_phiInvH;
    hmat_t * phiInvNew;

} iscm_t;

scm_t * scm_construct(const unsigned int num_channels, const unsigned int num_bins, const float alpha);

void scm_destroy(scm_t * obj);

int scm_process(scm_t * obj, const freqs_t * freqs, const masks_t * masks, covs_t * covs);

iscm_t * iscm_construct(const unsigned int num_channels, const unsigned int num_bins, const float alpha);

void iscm_destroy(iscm_t * obj);

int iscm_process(iscm_t * obj, const freqs_t * freqs, const masks_t * masks, covs_t * covs);



#endif // __SCM_H