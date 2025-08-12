#include <ios/msg.h>
#include <utils/error.h>

#include <stdlib.h>
#include <string.h>

#define CHECK_FPRINTF(rtn)                                                                                             \
    do                                                                                                                 \
    {                                                                                                                  \
        if ((rtn) < 0)                                                                                                 \
        {                                                                                                              \
            odas2_set_error_number(ODAS2_ERROR_MSGOUT_WRITE);                                                          \
            return -1;                                                                                                 \
        }                                                                                                              \
    } while (0)

msgout_t * msgout_construct(const char * file_name) {

    msgout_t * obj = (msgout_t *) malloc(sizeof(msgout_t));

    obj->file_pointer = fopen(file_name, "wb");
    if (obj->file_pointer == NULL) {
        odas2_set_error_number(ODAS2_ERROR_MSGOUT_CONSTRUCT_FILE_CREATION);
        free((void*)obj);
        return NULL;
    }

    return obj;

}

void msgout_destroy(msgout_t * obj) {

    fclose(obj->file_pointer);

    free(obj);

}

int msgout_write_hops(msgout_t * obj, const hops_t * hops) {

    CHECK_FPRINTF(fprintf(obj->file_pointer, "{"));
    CHECK_FPRINTF(fprintf(obj->file_pointer, "\"type\":\"hops\""));
    CHECK_FPRINTF(fprintf(obj->file_pointer, ","));
    CHECK_FPRINTF(fprintf(obj->file_pointer, "\"label\":\"%s\"", hops->label));
    CHECK_FPRINTF(fprintf(obj->file_pointer, ","));
    CHECK_FPRINTF(fprintf(obj->file_pointer, "\"samples\":["));

    for (unsigned int index_channel = 0; index_channel < hops->num_channels; index_channel++) {
        CHECK_FPRINTF(fprintf(obj->file_pointer, "["));
        for (unsigned int index_shift = 0; index_shift < hops->num_shifts; index_shift++) {
            CHECK_FPRINTF(fprintf(obj->file_pointer, "%1.4f", hops->samples[index_channel][index_shift]));
            if (index_shift != (hops->num_shifts-1)) {
                CHECK_FPRINTF(fprintf(obj->file_pointer, ","));
            }
        }
        CHECK_FPRINTF(fprintf(obj->file_pointer, "]"));
        if (index_channel != (hops->num_channels-1)) {
            CHECK_FPRINTF(fprintf(obj->file_pointer, ","));
        }
    }

    CHECK_FPRINTF(fprintf(obj->file_pointer, "]"));
    CHECK_FPRINTF(fprintf(obj->file_pointer, "}\n"));
    fflush(obj->file_pointer);

    return 0;

}

int msgout_write_freqs(msgout_t * obj, const freqs_t * freqs) {

    CHECK_FPRINTF(fprintf(obj->file_pointer, "{"));
    CHECK_FPRINTF(fprintf(obj->file_pointer, "\"type\":\"freqs\""));
    CHECK_FPRINTF(fprintf(obj->file_pointer, ","));
    CHECK_FPRINTF(fprintf(obj->file_pointer, "\"label\":\"%s\"", freqs->label));
    CHECK_FPRINTF(fprintf(obj->file_pointer, ","));
    CHECK_FPRINTF(fprintf(obj->file_pointer, "\"bins\":["));

    for (unsigned int index_channel = 0; index_channel < freqs->num_channels; index_channel++) {
        CHECK_FPRINTF(fprintf(obj->file_pointer, "["));
        for (unsigned int index_bin = 0; index_bin < freqs->num_bins; index_bin++) {
            CHECK_FPRINTF(fprintf(obj->file_pointer, "[%1.4f,%1.4f]", freqs->bins[index_channel][index_bin].real, freqs->bins[index_channel][index_bin].imag));
            if (index_bin != (freqs->num_bins-1)) {
                CHECK_FPRINTF(fprintf(obj->file_pointer, ","));
            }
        }
        CHECK_FPRINTF(fprintf(obj->file_pointer, "]"));
        if (index_channel != (freqs->num_channels-1)) {
            CHECK_FPRINTF(fprintf(obj->file_pointer, ","));
        }
    }

    CHECK_FPRINTF(fprintf(obj->file_pointer, "]"));
    CHECK_FPRINTF(fprintf(obj->file_pointer, "}\n"));
    fflush(obj->file_pointer);

    return 0;

}

int msgout_write_covs(msgout_t * obj, const covs_t * covs) {


    return -1;

}

int msgout_write_masks(msgout_t * obj, const masks_t * masks) {


    return -1;

}

int msgout_write_doas(msgout_t * obj, const doas_t * doas) {

    CHECK_FPRINTF(fprintf(obj->file_pointer, "{"));
    CHECK_FPRINTF(fprintf(obj->file_pointer, "\"type\":\"doas\""));
    CHECK_FPRINTF(fprintf(obj->file_pointer, ","));
    CHECK_FPRINTF(fprintf(obj->file_pointer, "\"label\":\"%s\"", doas->label));
    CHECK_FPRINTF(fprintf(obj->file_pointer, ","));
    CHECK_FPRINTF(fprintf(obj->file_pointer, "\"dirs\":["));

    for (unsigned int index_direction = 0; index_direction < doas->num_directions; index_direction++) {
        CHECK_FPRINTF(fprintf(obj->file_pointer, "{\"type\": %u, \"x\": %1.3f, \"y\": %1.3f, \"z\": %1.3f, \"energy\": %1.3f}",
                    doas->dirs[index_direction].type,
                    doas->dirs[index_direction].coord.x,
                    doas->dirs[index_direction].coord.y,
                    doas->dirs[index_direction].coord.z,
                    doas->dirs[index_direction].energy));
        if (index_direction != (doas->num_directions-1)) {
            CHECK_FPRINTF(fprintf(obj->file_pointer, ","));
        }
    }

    CHECK_FPRINTF(fprintf(obj->file_pointer, "]"));
    CHECK_FPRINTF(fprintf(obj->file_pointer, "}\n"));
    fflush(obj->file_pointer);

    return 0;

}

int msgout_write_tdoas(msgout_t * obj, const tdoas_t * tdoas) {

    CHECK_FPRINTF(fprintf(obj->file_pointer, "{"));
    CHECK_FPRINTF(fprintf(obj->file_pointer, "\"type\":\"tdoas\""));
    CHECK_FPRINTF(fprintf(obj->file_pointer, ","));
    CHECK_FPRINTF(fprintf(obj->file_pointer, "\"label\":\"%s\"", tdoas->label));
    CHECK_FPRINTF(fprintf(obj->file_pointer, ","));
    CHECK_FPRINTF(fprintf(obj->file_pointer, "\"taus\":["));

    for (unsigned int index_source = 0; index_source < tdoas->num_sources; index_source++) {
        CHECK_FPRINTF(fprintf(obj->file_pointer, "["));
        for (unsigned int index_pair = 0; index_pair < tdoas->num_pairs; index_pair++) {
            CHECK_FPRINTF(fprintf(obj->file_pointer, "{\"delay\": %1.3f, \"amplitude\": %1.3f}", tdoas->taus[index_source][index_pair].delay, tdoas->taus[index_source][index_pair].amplitude));
            if (index_pair != (tdoas->num_pairs-1)) {
                CHECK_FPRINTF(fprintf(obj->file_pointer, ","));
            }
        }
        CHECK_FPRINTF(fprintf(obj->file_pointer, "]"));
        if (index_source != (tdoas->num_sources-1)) {
            CHECK_FPRINTF(fprintf(obj->file_pointer, ","));
        }
    }

    CHECK_FPRINTF(fprintf(obj->file_pointer, "]"));
    CHECK_FPRINTF(fprintf(obj->file_pointer, "}\n"));
    fflush(obj->file_pointer);

    return 0;

}
