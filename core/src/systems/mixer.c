#include <systems/mixer.h>

#include <stdlib.h>
#include <string.h>

mixer_t * mixer_construct(const char * channels) {

    mixer_t * obj = (mixer_t *) malloc(sizeof(mixer_t));
    
    //
    // This code parses a string like:
    // 
    // channels = "2,11,7"
    //
    // and store the numbers in a list such as:
    //
    // obj->map[0] = 2
    // obj->map[1] = 11
    // obj->map[2] = 7
    //

    const char delim[2] = ",";

    //
    // Create temporatry string to store expression
    //

    char * str = (char *) malloc((strlen(channels)+1) * sizeof(char));

    //
    // Count the number of tokens in the string
    //

    strcpy(str, channels);
    obj->num_channels = 0;

    {
        char * ptr = strtok(str, delim);
        while(ptr != NULL) {
            obj->num_channels++;
            ptr = strtok(NULL, delim);
        }
    }

    //
    // Store the number in a map
    //

    obj->map = (unsigned int *) malloc(sizeof(unsigned int) * obj->num_channels);
    strcpy(str, channels);

    {
        unsigned int index_channel = 0;

        char * ptr = strtok(str, delim);
        while(ptr != NULL) {
            obj->map[index_channel] = atoi(ptr);
            ptr = strtok(NULL, delim);
            index_channel++;
        }
    }

    //
    // Free temporary string
    //

    free(str);

    return obj;

}

void mixer_destroy(mixer_t * obj) {

    free(obj->map);
    free(obj);

}

int mixer_process(mixer_t * obj, const hops_t * hops_in, hops_t * hops_out) {

    for (unsigned int index_channel = 0; index_channel < obj->num_channels; index_channel++) {

        memcpy(hops_out->samples[index_channel], 
               hops_in->samples[obj->map[index_channel]], 
               sizeof(float) * hops_in->num_shifts);

    }

    return 0;

}