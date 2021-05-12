#include <stc/cvec.h>
#include <stdio.h>

using_cvec(i, int);

int main()
{
    // Create a vector containing integers
    cvec_i vec = cvec_i_init();

    // Add two integers to vector
    cvec_i_push_back(&vec, 25);
    cvec_i_push_back(&vec, 13);

    // Append a set of numbers
    c_emplace(cvec_i, vec, {7, 5, 16, 8});

    printf("initial:");
    c_foreach (k, cvec_i, vec) {
        printf(" %d", *k.ref);
    }

    // Sort the vector
    cvec_i_sort(&vec);

    printf("\nsorted:");
    c_foreach (k, cvec_i, vec) {
        printf(" %d", *k.ref);
    }

    cvec_i_del(&vec);
}
