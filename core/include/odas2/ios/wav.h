#ifndef __WAV_H
#define __WAV_H

#include "../signals/hops.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct wavhdr_t {

    char chunk_id[4];
    unsigned int chunk_size;
    char format[4];
    char subchunk1_id[4];
    unsigned int subchunk1_size;
    unsigned short audio_format;
    unsigned short num_channels;
    unsigned int sample_rate;
    unsigned int byte_rate;
    unsigned short block_align;
    unsigned short bits_per_sample;
    char subchunk2_id[4];
    unsigned int subchunk2_size;

} __attribute__((packed)) wavhdr_t;

typedef struct wavin_t {

    unsigned int num_channels;
    unsigned int num_shifts;
    unsigned int sample_rate;
    
    FILE * file_pointer;
    short * buffer;

} wavin_t;

typedef struct wavout_t {

    unsigned int num_channels;
    unsigned int num_shifts;
    unsigned int sample_rate;
    
    FILE * file_pointer;
    short * buffer;

} wavout_t;

wavhdr_t wavhdr_cst(const unsigned int num_channels, const unsigned int sample_rate, const unsigned int bits_per_sample);

int wavhdr_cmp(const wavhdr_t hdr1, const wavhdr_t hdr2);

void wavhdr_printf(const wavhdr_t hdr);

wavin_t * wavin_construct(const char * file_name, const unsigned int num_shifts, const unsigned int num_channels, const unsigned int sample_rate);

void wavin_destroy(wavin_t * obj);

int wavin_read(wavin_t * obj, hops_t * hops);

wavout_t * wavout_construct(const char * file_name, const unsigned int num_shifts, const unsigned int num_channels, const unsigned int sample_rate);

void wavout_destroy(wavout_t * obj);

int wavout_write(wavout_t * obj, const hops_t * hops);

#endif // __WAV_H