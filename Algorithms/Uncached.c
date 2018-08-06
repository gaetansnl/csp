//
// Created by Gaetan on 24/04/2017.
//

#include "Uncached.h"

/**
 * Heuristique de variable qui retourne la première varable non assignée trouvée
 * @param vars
 * @param size
 * @param last
 * @return
 */
CSPVar next_var_unassigned(CSPVar *vars, size_t size, CSPVar last, CSPInfo infos){
    for (int i = 0; i < size; ++i) {
        if(!vars[i]->is_assigned) return vars[i];
    }
    return NULL;
}

/**
 * Heuristique de valeur qui retourne la valoeur qui sur la courante.
 * @param var
 * @param infos
 * @param endOfDomain
 * @return
 */
CSPDomainValue next_value_default(CSPVar var, CSPInfo infos, bool *endOfDomain){
    if(!var->is_assigned) return var->domain->values[0];

    for (int i = 1; i < var->domain->size; ++i)
        if(var->domain->values[i - 1] == var->domain_value)
            return var->domain->values[i];

    *endOfDomain = true;
}