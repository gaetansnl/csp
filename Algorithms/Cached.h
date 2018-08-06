//
// Created by Gaetan on 24/04/2017.
//

#ifndef UNTITLED1_FORWARDCHECKING_H
#define UNTITLED1_FORWARDCHECKING_H

#include "../CSP/CSP.h"

CSPDomainValue next_value_cached(CSPVar var, CSPInfo infos, bool *endOfDomain);
CSPVar next_var_unassigned_mrv_cached(CSPVar *vars, size_t size, CSPVar last, CSPInfo infos);

CSPVar next_var_unassigned_mrv_deg_cached(CSPVar *vars, size_t size, CSPVar last, CSPInfo infos);

CSPVar next_var_unassigned_dom_deg_cached(CSPVar *vars, size_t size, CSPVar last, CSPInfo infos);
CSPDomainValue next_value_lcv_cached(CSPVar var, CSPInfo infos, bool *endOfDomain);

bool connected_update_cached(CSPVar var, CSPConstraint constraint, CSPVar current, CSPInfo infos);
void preprocessing_cached(CSPVar *vars, size_t size);

bool assigned_update_cached(CSPVar var, CSPDomainValue last_value, CSPInfo infos);
#endif //UNTITLED1_FORWARDCHECKING_H
