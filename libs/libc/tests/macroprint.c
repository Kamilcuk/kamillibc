/*
 * macroprint.c
 *
 *  Created on: 4 lut 2020
 *      Author: Kamil
 */
#include <macroprint.h>
#include <stdio.h>

#define B2(a)   4##a
#define B(...)  B2(__VA_ARGS__)
#define D()   2
#define A() B(D())
MACROPRINT(A())

int main() {

}
