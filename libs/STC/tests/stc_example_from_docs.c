#define i_type FVec    // if not defined, vector type would be cvec_float
#define i_val float    // element type
#include <stc/cvec.h>  // defines the FVec type
#include <stdio.h>

int main(void) {
    FVec vec = FVec_init();
    FVec_push_back(&vec, 10.f);
    FVec_push_back(&vec, 20.f);
    FVec_push_back(&vec, 30.f);

    for (size_t i = 0; i < FVec_size(vec); ++i)
        printf(" %g", vec.data[i]);
    FVec_drop(&vec); // free memory
}
