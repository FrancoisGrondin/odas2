#include <signals/dsf.h>

#include <stdlib.h>
#include <string.h>

dsf_t * dsf_construct(const char * label) {

	dsf_t * obj = (dsf_t *) malloc(sizeof(dsf_t));

    memset(obj->label, 0x00, 64);
    strcpy(obj->label, label);

	obj->sigmoid_mean = 0.3f;
	obj->sigmoid_slope = 40.0f;
	obj->tracked_source_sigma2 = 0.05f;
	obj->tracked_source_threshold = 0.25f;
	obj->tracked_source_rate = 0.1f;
	obj->new_source_sigma2 = 0.01f;
	obj->new_threshold = 0.4f;
	obj->delete_threshold = 0.2f;
	obj->delete_decay = 0.98f;
	
	return obj;

}

void dsf_destroy(dsf_t * obj) {

	free(obj);

}
