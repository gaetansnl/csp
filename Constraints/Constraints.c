//
// Created by Gaetan on 04/04/2017.
//

#include "Constraints.h"

bool constraint_allDifferent(CSPConstraint constraint, CSPVar current){
    for (int i = 0; i < constraint->scope_count; ++i) {
        CSPVar var = constraint->scope[i];
        if(var != current && var->is_assigned && var->domain_value == current->domain_value){
            return false;
        }
    }
    return true;
}

bool constraint_lt(CSPConstraint constraint, CSPVar current){
    if(constraint->scope_count != 2 || csp_constraint_has_unassigned(constraint))return true;
    return (int)(csp_constraint_get_scope_value(constraint, 0)) < (int)(csp_constraint_get_scope_value(constraint, 1));
}