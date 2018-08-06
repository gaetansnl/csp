//
// Created by Gaetan on 03/04/2017.
//

#ifndef UNTITLED1_FUTOSHIKIREADER_H
#define UNTITLED1_FUTOSHIKIREADER_H

#include <stdlib.h>
#include "CSP/CSP.h"

typedef struct {
    size_t size;
    CSPVar *vars;
} *FutoshikiCSPGrid;

FutoshikiCSPGrid fut_create_grid(char* file);
void fut_print_grid(FutoshikiCSPGrid g);

#endif //UNTITLED1_FUTOSHIKIREADER_H
