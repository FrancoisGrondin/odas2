#include <ios/wav.h>

wavhdr_t wavhdr_cst(const unsigned int num_channels, const unsigned int sample_rate, const unsigned int bits_per_sample) {

    wavhdr_t hdr;

    // Chunk descriptor (corresponds to characters "RIFF")
    hdr.chunk_id[0] = 'R';
    hdr.chunk_id[1] = 'I';
    hdr.chunk_id[2] = 'F';
    hdr.chunk_id[3] = 'F';
    // Size of the file (set to max as we don't know the number of samples)
    hdr.chunk_size = 0xFFFFFFFF;
    // Contains the characters "WAVE"
    hdr.format[0] = 'W';
    hdr.format[1] = 'A';
    hdr.format[2] = 'V';
    hdr.format[3] = 'E';
    // Contains the characters "fmt "
    hdr.subchunk1_id[0] = 'f';
    hdr.subchunk1_id[1] = 'm';
    hdr.subchunk1_id[2] = 't';
    hdr.subchunk1_id[3] = ' ';
    // Corresponds to 16 for PCM
    hdr.subchunk1_size = 16;
    // PCM = 1 for linear quantization
    hdr.audio_format = 1;
    // Number of channels
    hdr.num_channels = num_channels;
    // Sample rate in samples/sec
    hdr.sample_rate = sample_rate;
    // Equals sample_rate * num_channels * bits_per_sample / 8
    hdr.byte_rate = sample_rate * num_channels * bits_per_sample / 8;
    // Equals num_channels * bits_per_sample / 8
    hdr.block_align = num_channels * bits_per_sample / 8;
    // Number of bits per sample
    hdr.bits_per_sample = bits_per_sample;
    // Contains the characters "data"
    hdr.subchunk2_id[0] = 'd';
    hdr.subchunk2_id[1] = 'a';
    hdr.subchunk2_id[2] = 't';
    hdr.subchunk2_id[3] = 'a';
    // Equals num_samples * num_channels * bits_per_sample / 8
    hdr.subchunk2_size = 0xFFFFFFFF;    

    return hdr;

}

int wavhdr_cmp(const wavhdr_t hdr1, const wavhdr_t hdr2) {

    // Compare chunk ID, format, subchunk1 id, subchunk2 id
    for (int index_byte = 0; index_byte < 4; index_byte++) {
        if (hdr1.chunk_id[index_byte] != hdr2.chunk_id[index_byte]) { return -1; }
        if (hdr1.format[index_byte] != hdr2.format[index_byte]) { return -1; }
        if (hdr1.subchunk1_id[index_byte] != hdr2.subchunk1_id[index_byte]) { return -1; }
        if (hdr1.subchunk2_id[index_byte] != hdr2.subchunk2_id[index_byte]) { return -1; }
    }

    // Compare other relevant fields
    if (hdr1.subchunk1_size != hdr2.subchunk1_size) { return -1; }
    if (hdr1.audio_format != hdr2.audio_format) { return -1; }
    if (hdr1.num_channels != hdr2.num_channels) { return -1; }
    if (hdr1.sample_rate != hdr2.sample_rate) { return -1; }
    if (hdr1.byte_rate != hdr2.byte_rate) { return -1; }
    if (hdr1.block_align != hdr2.block_align) { return -1; }
    if (hdr1.bits_per_sample != hdr2.bits_per_sample) { return -1; }

    return 0;

}

void wavhdr_printf(const wavhdr_t hdr) {

    printf("Chunk ID: %c%c%c%c\n", hdr.chunk_id[0], hdr.chunk_id[1], hdr.chunk_id[2], hdr.chunk_id[3]);
    printf("Chunk size: %u\n", hdr.chunk_size);
    printf("Format: %c%c%c%c\n", hdr.format[0], hdr.format[1], hdr.format[2], hdr.format[3]);
    printf("Subchunk 1 ID: %c%c%c%c\n", hdr.subchunk1_id[0], hdr.subchunk1_id[1], hdr.subchunk1_id[2], hdr.subchunk1_id[3]);
    printf("Subchunk 1 size: %u\n", hdr.subchunk1_size);
    printf("Audio format: %u\n", hdr.audio_format);
    printf("Number of channels: %u\n", hdr.num_channels);
    printf("Sample rate: %u\n", hdr.sample_rate);
    printf("Byte rate: %u\n", hdr.byte_rate);
    printf("Block align: %u\n", hdr.block_align);
    printf("Bits per sample: %u\n", hdr.bits_per_sample);
    printf("Subchunk 2 ID: %c%c%c%c\n", hdr.subchunk2_id[0], hdr.subchunk2_id[1], hdr.subchunk2_id[2], hdr.subchunk2_id[3]);
    printf("Subchunk 2 size: %u\n", hdr.subchunk2_size);

}

wavin_t * wavin_construct(const char * file_name, const unsigned int num_shifts, const unsigned int num_channels, const unsigned int sample_rate) {

    wavin_t * obj = (wavin_t *) malloc(sizeof(wavin_t));

    obj->file_pointer = fopen(file_name, "rb");

    wavhdr_t hdr;
    fread(&hdr, sizeof(wavhdr_t), 1, obj->file_pointer);

    if (wavhdr_cmp(hdr, wavhdr_cst(num_channels, sample_rate, 16)) == -1) {
        printf("Wave file does not match format requirements\n");
        exit(EXIT_FAILURE);
    }

    obj->num_channels = num_channels;
    obj->num_shifts = num_shifts;
    obj->sample_rate = sample_rate;

    obj->buffer = (short *) calloc(obj->num_channels * obj->num_shifts, sizeof(short));

    return obj;

}

void wavin_destroy(wavin_t * obj) {

    fclose(obj->file_pointer);
    free(obj->buffer);
    free(obj);

}

int wavin_read(wavin_t * obj, hops_t * hops) {

    size_t rtn = fread(obj->buffer, sizeof(short), obj->num_channels * obj->num_shifts, obj->file_pointer);

    if (rtn != obj->num_channels * obj->num_shifts) {
        return -1;
    }

    for (unsigned int index_channel = 0; index_channel < obj->num_channels; index_channel++) {
        for (unsigned int index_shift = 0; index_shift < obj->num_shifts; index_shift++) {
            hops->samples[index_channel][index_shift] = ((float) obj->buffer[index_shift * obj->num_channels + index_channel]) / 32768.0f;
        }
    }


    return 0;

}

wavout_t * wavout_construct(const char * file_name, const unsigned int num_shifts, const unsigned int num_channels, const unsigned int sample_rate) {

    wavout_t * obj = (wavout_t *) malloc(sizeof(wavout_t));

    obj->file_pointer = fopen(file_name, "wb");

    wavhdr_t hdr = wavhdr_cst(num_channels, sample_rate, 16);
    size_t rtn = fwrite(&hdr, sizeof(wavhdr_t), 1, obj->file_pointer);

    if (rtn != 1) {
        printf("Cannot write to wave file\n");
        exit(EXIT_FAILURE);
    }

    obj->num_channels = num_channels;
    obj->num_shifts = num_shifts;
    obj->sample_rate = sample_rate;

    obj->buffer = (short *) calloc(obj->num_channels * obj->num_shifts, sizeof(short));

    return obj;

}

void wavout_destroy(wavout_t * obj) {

    fclose(obj->file_pointer);
    free(obj->buffer);
    free(obj);

}

int wavout_write(wavout_t * obj, const hops_t * hops) {

    for (unsigned int index_channel = 0; index_channel < obj->num_channels; index_channel++) {
        for (unsigned int index_shift = 0; index_shift < obj->num_shifts; index_shift++) {
            obj->buffer[index_shift * obj->num_channels + index_channel] = (short) (hops->samples[index_channel][index_shift] * 32768.0);
        }
    }

    size_t rtn = fwrite(obj->buffer, sizeof(short), obj->num_channels * obj->num_shifts, obj->file_pointer);

    if (rtn != (obj->num_channels * obj->num_shifts)) {
        return -1;
    }

    return 0;

}