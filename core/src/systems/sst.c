#include <systems/sst.h>

#include <math.h>
#include <stdlib.h>
#include <string.h>

sst_t * sst_construct(const unsigned int num_tracks, const unsigned int num_directions, const unsigned int num_pasts) {

    sst_t * obj = (sst_t *) malloc(sizeof(sst_t));

    obj->num_tracks = num_tracks;
    obj->num_directions = num_directions;
    obj->num_pasts = num_pasts;

    obj->pasts = (dir_t *) calloc(obj->num_pasts, sizeof(dir_t));
    obj->tracks = (dir_t *) calloc(obj->num_tracks, sizeof(dir_t));

    return obj;

}

void sst_destroy(sst_t * obj) {

    free(obj->pasts);
    free(obj->tracks);

    free(obj);

}

int sst_process(sst_t * obj, const dsf_t * dsf, const doas_t * in, doas_t * out) {

    //
    // Loop for each potential source, and decide if associated
    // to an existing tracked source or if it is a new source
    //

    for (unsigned int index_direction = 0; index_direction < obj->num_directions; index_direction++) {

        //
        // Get current potential source and fix the energy according to sigmoid
        //

        dir_t pot = in->dirs[index_direction];
        pot.energy = 1.0f / (1.0f + expf(-1.0f * dsf->sigmoid_slope * (pot.energy - dsf->sigmoid_mean)));

        //
        // Update the tracked sources, and also return the best score
        // that indicates fit between potential source and tracked sources
        //

        float best_score = 0.0f;
        unsigned int best_match = 0;

        {

            for (unsigned int index_track = 0; index_track < obj->num_tracks; index_track++) {

                if (obj->tracks[index_track].type == TRACKED) {

                    float dist2 = xyz_l2(xyz_sub(pot.coord, obj->tracks[index_track].coord));
                    float score = expf(-1.0f * dist2 / dsf->tracked_source_sigma2);

                    if (score > best_score) {
                        best_score = score;
                        best_match = index_track;
                    }

                }

            }

        }

        //
        // If the score is high enough, then update the corresponding
        // tracked source, otherwise check for a new source to be added
        //

        if (best_score > dsf->tracked_source_threshold) {

            // Update the corresponding tracked source

            float adapt = pot.energy * dsf->tracked_source_rate;
            obj->tracks[best_match].coord = xyz_unit(xyz_add(obj->tracks[best_match].coord, xyz_scale(pot.coord, adapt)));
            obj->tracks[best_match].energy += pot.energy;

            if (obj->tracks[best_match].energy > 1.0f) {
                obj->tracks[best_match].energy = 1.0f;
            }

            // And then reset the energy
            pot.energy = 0.0f;

        }
        else {

            //
            // Compute the match of this source with past sources
            //

            dir_t new_source = { .type = TRACKED, .coord = xyz_cst(0.0f, 0.0f, 0.0f), .energy = 0.0f };

            for (unsigned int index_past = 0; index_past < obj->num_pasts; index_past++) {

                dir_t past = obj->pasts[index_past];

                float dist2 = xyz_l2(xyz_sub(pot.coord, past.coord));
                float score = expf(-1.0f * dist2 / dsf->new_source_sigma2);

                new_source.energy += score * past.energy;
                new_source.coord = xyz_add(new_source.coord, xyz_scale(past.coord, score));

            }

            new_source.coord = xyz_unit(new_source.coord);
            new_source.energy /= obj->num_pasts;
            new_source.energy *= obj->num_directions;

            //
            // If the score is good enough, then create a new tracked source
            //

            if (new_source.energy > dsf->new_threshold) {

                for (unsigned int index_track = 0; index_track < obj->num_tracks; index_track++) {

                    if (obj->tracks[index_track].type == UNDEFINED) {

                        obj->tracks[index_track].type = TRACKED;
                        obj->tracks[index_track].coord = new_source.coord;
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

        if (obj->tracks[index_track].type == TRACKED) {

            obj->tracks[index_track].energy *= dsf->delete_decay;

            if (obj->tracks[index_track].energy < dsf->delete_threshold) {

                obj->tracks[index_track].type = UNDEFINED;
                obj->tracks[index_track].coord = xyz_cst(0.0f, 0.0f, 0.0f);
                obj->tracks[index_track].energy = 0.0f;

            }

        }

    }

    //
    // Export doas to output
    //

    for (unsigned int index_track = 0; index_track < obj->num_tracks; index_track++) {

        if (obj->tracks[index_track].type != UNDEFINED) {
            out->dirs[index_track] = obj->tracks[index_track];
        }
        else {
            out->dirs[index_track].type = UNDEFINED;
            out->dirs[index_track].coord = xyz_cst(0.0f, 0.0f, 0.0f);
            out->dirs[index_track].energy = 0.0f;
        }

    }

    return 0;

}

void sst_printf(const sst_t * obj) {

    for (unsigned int index_track = 0; index_track < obj->num_tracks; index_track++) {

        dir_t dir = obj->tracks[index_track];

        printf("[%02u]: { { .x = %+1.3f, .y = %+1.3f, .z = %+1.3f }, .energy = %+1.3f }\n",
            index_track, dir.coord.x, dir.coord.y, dir.coord.z, dir.energy);

    }

    printf("\n");

    for (unsigned int index_past = 0; index_past < obj->num_pasts; index_past++) {

        dir_t dir = obj->pasts[index_past];

        printf("[%02u]: { { .x = %+1.3f, .y = %+1.3f, .z = %+1.3f }, .energy = %+1.3f }\n",
            index_past, dir.coord.x, dir.coord.y, dir.coord.z, dir.energy);

    }

    printf("\n");

}
