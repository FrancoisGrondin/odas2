#include <ios/msg.h>
#include <stdlib.h>
#include <string.h>

msgout_t * msgout_construct(const char * file_name) {

    msgout_t * obj = (msgout_t *) malloc(sizeof(msgout_t));

    obj->file_pointer = fopen(file_name, "wb");

    return obj;

}

void msgout_destroy(msgout_t * obj) {

    fclose(obj->file_pointer);

    free(obj);

}

int msgout_write_hops(msgout_t * obj, const hops_t * hops) {

    fprintf(obj->file_pointer, "{");
    fprintf(obj->file_pointer, "\"type\":\"hops\"");
    fprintf(obj->file_pointer, ",");
    fprintf(obj->file_pointer, "\"label\":\"%s\"", hops->label);
    fprintf(obj->file_pointer, ",");
    fprintf(obj->file_pointer, "\"samples\":[");

    for (unsigned int index_channel = 0; index_channel < hops->num_channels; index_channel++) {
        fprintf(obj->file_pointer, "[");
        for (unsigned int index_shift = 0; index_shift < hops->num_shifts; index_shift++) {
            fprintf(obj->file_pointer, "%1.4f", hops->samples[index_channel][index_shift]);
            if (index_shift != (hops->num_shifts-1)) {
                fprintf(obj->file_pointer, ",");
            }
        }
        fprintf(obj->file_pointer, "]");
        if (index_channel != (hops->num_channels-1)) {
            fprintf(obj->file_pointer, ",");
        }
    }

    fprintf(obj->file_pointer, "]");
    fprintf(obj->file_pointer, "}\n");

    return 0;

}

int msgout_write_freqs(msgout_t * obj, const freqs_t * freqs) {

    fprintf(obj->file_pointer, "{");
    fprintf(obj->file_pointer, "\"type\":\"freqs\"");
    fprintf(obj->file_pointer, ",");
    fprintf(obj->file_pointer, "\"label\":\"%s\"", freqs->label);
    fprintf(obj->file_pointer, ",");
    fprintf(obj->file_pointer, "\"bins\":[");

    for (unsigned int index_channel = 0; index_channel < freqs->num_channels; index_channel++) {
        fprintf(obj->file_pointer, "[");
        for (unsigned int index_bin = 0; index_bin < freqs->num_bins; index_bin++) {
            fprintf(obj->file_pointer, "[%1.4f,%1.4f]", freqs->bins[index_channel][index_bin].real, freqs->bins[index_channel][index_bin].imag);
            if (index_bin != (freqs->num_bins-1)) {
                fprintf(obj->file_pointer, ",");
            }
        }
        fprintf(obj->file_pointer, "]");
        if (index_channel != (freqs->num_channels-1)) {
            fprintf(obj->file_pointer, ",");
        }
    }

    fprintf(obj->file_pointer, "]");
    fprintf(obj->file_pointer, "}\n");

    return 0;

}

int msgout_write_covs(msgout_t * obj, const covs_t * covs) {


    return -1;

}

int msgout_write_masks(msgout_t * obj, const masks_t * masks) {


    return -1;

}

int msgout_write_doas(msgout_t * obj, const doas_t * doas) {

    fprintf(obj->file_pointer, "{");
    fprintf(obj->file_pointer, "\"type\":\"doas\"");
    fprintf(obj->file_pointer, ",");
    fprintf(obj->file_pointer, "\"label\":\"%s\"", doas->label);
    fprintf(obj->file_pointer, ",");
    fprintf(obj->file_pointer, "\"pots\":[");

    for (unsigned int index_direction = 0; index_direction < doas->num_directions; index_direction++) {
        fprintf(obj->file_pointer, "{\"x\": %1.2f, \"y\": %1.2f, \"z\": %1.2f, \"energy\": %1.2f}", 
                    doas->pots[index_direction].direction.x,
                    doas->pots[index_direction].direction.y,
                    doas->pots[index_direction].direction.z,
                    doas->pots[index_direction].energy);
        if (index_direction != (doas->num_directions-1)) {
            fprintf(obj->file_pointer, ",");
        }        
    }

    fprintf(obj->file_pointer, "]");
    fprintf(obj->file_pointer, "}\n");

    return 0;  

}

int msgout_write_tdoas(msgout_t * obj, const tdoas_t * tdoas) {

    fprintf(obj->file_pointer, "{");
    fprintf(obj->file_pointer, "\"type\":\"tdoas\"");
    fprintf(obj->file_pointer, ",");
    fprintf(obj->file_pointer, "\"label\":\"%s\"", tdoas->label);
    fprintf(obj->file_pointer, ",");
    fprintf(obj->file_pointer, "\"taus\":[");

    for (unsigned int index_source = 0; index_source < tdoas->num_sources; index_source++) {
        fprintf(obj->file_pointer, "[");
        for (unsigned int index_pair = 0; index_pair < tdoas->num_pairs; index_pair++) {
            fprintf(obj->file_pointer, "{delay: %+1.2f, amplitude: %+1.2f}", tdoas->taus[index_source][index_pair].delay, tdoas->taus[index_source][index_pair].amplitude);
            if (index_pair != (tdoas->num_pairs-1)) {
                fprintf(obj->file_pointer, ",");
            }
        }
        fprintf(obj->file_pointer, "]");
        if (index_source != (tdoas->num_sources-1)) {
            fprintf(obj->file_pointer, ",");
        }        
    }

    fprintf(obj->file_pointer, "]");
    fprintf(obj->file_pointer, "}\n");

    return 0;    

}

