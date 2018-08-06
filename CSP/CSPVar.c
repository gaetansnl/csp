#include "CSP.h"

CSPVar csp_var_create(CSPDomain domain) {
    CSPVar result = malloc(sizeof(struct CSPVar));
    result->domain = domain;

    result->constraint_count = 0;
    result->constraints = NULL;

    result->cache = NULL;

    csp_var_reset(result);

    return result;
}

void csp_var_set(CSPVar var, CSPDomainValue domainValue) {
    var->is_assigned = true;
    var->domain_value = domainValue;
}

bool csp_var_consistent(CSPVar var) {
    for (int i = 0; i < var->constraint_count; ++i) {
        if (!csp_constraint_solved(var->constraints[i], var))
            return false;
    }
    return true;
}

void csp_var_reset(CSPVar var) {
    var->is_assigned = false;
    var->domain_value = 0;
}