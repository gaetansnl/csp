//
// Created by Gaetan on 24/04/2017.
//

#include "../CSP/CSP.h"

#ifndef UNTITLED1_BACKTRACKING_H
#define UNTITLED1_BACKTRACKING_H

CSPVar next_var_unassigned(CSPVar *vars, size_t size, CSPVar last, CSPInfo infos);
CSPDomainValue next_value_default(CSPVar var, CSPInfo infos, bool *endOfDomain);

#endif //UNTITLED1_BACKTRACKING_H

