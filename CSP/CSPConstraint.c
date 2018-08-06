#include "CSP.h"

//check null values in scope
CSPConstraint csp_constraint_add(CSPVar *scope, size_t scope_size, CSPConstraintFunction constraint_function) {
    CSPConstraint result = malloc(sizeof(struct CSPConstraint));

    result->scope = malloc(sizeof(CSPVar) * scope_size);
    memcpy(result->scope, scope, sizeof(CSPVar) * scope_size);
    result->scope_count = scope_size;

    result->constraint_function = constraint_function;

    // Link constraint

    for (int i = 0; i < scope_size; ++i) {
        if (scope[i]->constraints == NULL) {
            scope[i]->constraints = malloc(sizeof(CSPConstraint));
        } else {
            scope[i]->constraints = realloc(
                    scope[i]->constraints,
                    (scope[i]->constraint_count + 1) * sizeof(CSPConstraint)
            );
        }
        scope[i]->constraints[scope[i]->constraint_count] = result;
        scope[i]->constraint_count++;
    }

    return result;
}

bool csp_constraint_solved(CSPConstraint constraint, CSPVar current) {
    return constraint->constraint_function(constraint, current);
}

bool csp_constraint_has_unassigned(CSPConstraint constraint) {
    for (int i = 0; i < constraint->scope_count; ++i) {
        if (!constraint->scope[i]->is_assigned)
            return true;
    }
    return false;
}

CSPDomainValue csp_constraint_get_scope_value(CSPConstraint constraint, size_t index) {
    return constraint->scope[index]->domain_value;
}