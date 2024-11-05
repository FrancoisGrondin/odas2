#include <systems/sst.h>

#include <math.h>
#include <stdlib.h>
#include <string.h>

sst_t * sst_construct(const unsigned int num_tracks, const unsigned int num_directions, const float delta_time, const float energy_threshold) {

    sst_t * obj = (sst_t *) malloc(sizeof(sst_t));

    obj->num_tracks = num_tracks;
    obj->num_directions = num_directions;
    obj->num_pasts = (int) ((0.040f / delta_time) * num_directions);

    obj->delta_time = delta_time;
    obj->energy_threshold = energy_threshold;

    obj->score_min = 0.25f;
    obj->energy_new_threshold = energy_threshold * (0.040f / delta_time);
    obj->energy_delete_threshold = energy_threshold / 4.0f;
    obj->energy_decay = 0.95f;
    obj->id_counter = 0;

    obj->pasts = (pot_t *) calloc(obj->num_pasts, sizeof(pot_t));
    obj->tracks = (pot_t *) calloc(obj->num_tracks, sizeof(pot_t));

    return obj;

}

void sst_destroy(sst_t * obj) {

    free(obj->pasts);
    free(obj->tracks);

    free(obj);

}

int sst_process(sst_t * obj, doas_t * in, doas_t * out) {

    //
    // Loop for each potential source, and decide if associated
    // to an existing tracked source or if it is a new source
    //

    for (unsigned int index_direction = 0; index_direction < obj->num_directions; index_direction++) {

        //
        // Get current potential source
        //

        pot_t pot = in->pots[index_direction];

        //
        // Update the tracked sources, and also return the best score
        // that indicates fit between potential source and tracked sources
        //

        float best_score = 0.0f;

        {

            const float sigma2 = 0.05f;

            for (unsigned int index_track = 0; index_track < obj->num_tracks; index_track++) {

                if (obj->tracks[index_track].id != 0) {

                    float dist2 = xyz_l2(xyz_sub(pot.direction, obj->tracks[index_track].direction));
                    float score = expf(-1.0f * dist2 / sigma2);

                    obj->tracks[index_track].direction = xyz_unit(xyz_add(obj->tracks[index_track].direction, xyz_scale(pot.direction, score)));
                    obj->tracks[index_track].energy += score * pot.energy;

                    if (score > best_score) {
                        best_score = score;
                    }

                }

            }

        }

        //
        // Set the energy to zero if the best score is too high, as this
        // potential source should not be used to produce a new source since
        // there is already a match with a tracked source
        //

        if (best_score > obj->score_min) {
            pot.energy = 0.0f;
        }

        //
        // Check if there is a new source to be added
        //

        if (pot.energy > 0.0f) {

            //
            // Compute the match of this source with past sources
            //

            const float sigma2 = 0.01f;

            pot_t new_source = { .id = 0, .direction = xyz_cst(0.0f, 0.0f, 0.0f), .energy = 0.0f };

            for (unsigned int index_past = 0; index_past < obj->num_pasts; index_past++) {

                pot_t past = obj->pasts[index_past];

                float dist2 = xyz_l2(xyz_sub(pot.direction, past.direction));
                float score = expf(-1.0f * dist2 / sigma2);

                new_source.energy += score * past.energy;
                new_source.direction = xyz_add(new_source.direction, xyz_scale(past.direction, score));

            }

            new_source.direction = xyz_unit(new_source.direction);

            //
            // If the score is good enough, then create a new tracked source
            //

            if (new_source.energy > obj->energy_new_threshold) {

                for (unsigned int index_track = 0; index_track < obj->num_tracks; index_track++) {

                    if (obj->tracks[index_track].id == 0) {

                        obj->id_counter++;
                        obj->tracks[index_track].id = obj->id_counter;
                        obj->tracks[index_track].direction = new_source.direction;
                        obj->tracks[index_track].energy = 1.0f;

                        break;

                    }

                }

            }

        }

        //
        // Add the potential source to the buffer
        //        

        for (unsigned int index_past = 0; index_past < obj->num_pasts - 1; index_past++) {
            obj->pasts[index_past] = obj->pasts[index_past+1];
        }
        obj->pasts[obj->num_pasts-1] = pot;

    }

    //
    // Update tracked sources and delete sources inactive for too long
    //

    for (unsigned int index_track = 0; index_track < obj->num_tracks; index_track++) {

        if (obj->tracks[index_track].id != 0) {
    
            obj->tracks[index_track].energy *= obj->energy_decay;
            
            if (obj->tracks[index_track].energy < obj->energy_delete_threshold) {

                obj->tracks[index_track].id = 0;
                obj->tracks[index_track].direction = xyz_cst(0.0f, 0.0f, 0.0f);
                obj->tracks[index_track].energy = 0.0f;

            }

        }

    }

    //
    // Export doas to output
    //
    
    for (unsigned int index_track = 0; index_track < obj->num_tracks; index_track++) {

        if (obj->tracks[index_track].id != 0) {
            out->pots[index_track] = obj->tracks[index_track];
        }
        else {
            out->pots[index_track].id = 0;
            out->pots[index_track].direction = xyz_cst(0.0f, 0.0f, 0.0f);
            out->pots[index_track].energy = 0.0f;
        }

    }

    return 0;

}

void sst_printf(const sst_t * obj) {

    for (unsigned int index_track = 0; index_track < obj->num_tracks; index_track++) {

        pot_t pot = obj->tracks[index_track];

        printf("[%02u]: { .id = %08u, { .x = %+1.3f, .y = %+1.3f, .z = %+1.3f }, .energy = %+1.3f }\n", 
            index_track, pot.id, pot.direction.x, pot.direction.y, pot.direction.z, pot.energy);

    }

    printf("\n");

    for (unsigned int index_past = 0; index_past < obj->num_pasts; index_past++) {

        pot_t pot = obj->pasts[index_past];

        printf("[%02u]: { .id = %08u, { .x = %+1.3f, .y = %+1.3f, .z = %+1.3f }, .energy = %+1.3f }\n", 
            index_past, pot.id, pot.direction.x, pot.direction.y, pot.direction.z, pot.energy);

    }

    printf("\n");

}

