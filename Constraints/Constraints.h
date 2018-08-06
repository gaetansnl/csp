//
// Created by Gaetan on 04/04/2017.
//


#ifndef UNTITLED1_CONSTRAINTS_H
#define UNTITLED1_CONSTRAINTS_H

#include "../CSP/CSP.h"

bool constraint_allDifferent(CSPConstraint constraint, CSPVar current);
bool constraint_lt(CSPConstraint constraint, CSPVar current);

#endif //UNTITLED1_CONSTRAINTS_H
