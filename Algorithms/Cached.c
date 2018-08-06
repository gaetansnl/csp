//
// Created by Gaetan on 24/04/2017.
//

#include "Cached.h"
#include "../Constraints/Constraints.h"
#include "Common.h"
#include <limits.h>
#include <stdio.h>

#ifdef _MSC_VER
#  include <intrin.h>
#  define __builtin_popcount __popcnt
#endif

/**
 * Heuristique de valeur permettant de profiter du cache
 * @param var
 * @param infos
 * @param endOfDomain
 * @return
 */
CSPDomainValue next_value_cached(CSPVar var, CSPInfo infos, bool *endOfDomain){
    bool s = false;

    if(!var->is_assigned) s = true;
    for (int i = 0; i < var->domain->size; ++i) {
        if(s && value_to_mask(var->domain->values[i]) & getLastDomainFromCache(var->cache, infos.depth))
            return var->domain->values[i];
        if(var->domain->values[i] == var->domain_value) s = true;
    }

    *endOfDomain = true;
}

/**
 * Heuristique de variable mrv
 * @param vars
 * @param size
 * @param last
 * @param infos
 * @return
 */
CSPVar next_var_unassigned_mrv_cached(CSPVar *vars, size_t size, CSPVar last, CSPInfo infos){
    CSPVar min = NULL;
    int last_mrv = SHRT_MAX;
    for (int i = 0; i < size; ++i) {
        struct Cache *cache = vars[i]->cache;
        if(!vars[i]->is_assigned && cache->domain_size < last_mrv){
            last_mrv = cache->domain_size;
            min = vars[i];
        }
    }
    return min;
}

/**
 * Heuristique de variable. La variable ayant le plus petit domaine.
 * La séparation est faite en choisissant celle qui a le plus grand fd. (brelaz)
 * @param vars
 * @param size
 * @param last
 * @param infos
 * @return
 */
CSPVar next_var_unassigned_mrv_deg_cached(CSPVar *vars, size_t size, CSPVar last, CSPInfo infos){
    CSPVar max = NULL;
    int last_size = INT_MAX;
    for (int i = 0; i < size; ++i) {
        struct Cache *cache = vars[i]->cache;
        if(!vars[i]->is_assigned && cache->domain_size < last_size){
            last_size = cache->domain_size;
            max = vars[i];
        }
    }
    int last_deg = -1;
    for (int i = 0; i < size; ++i) {
        struct Cache *cache = vars[i]->cache;
        if(!vars[i]->is_assigned && cache->domain_size == last_size && cache->deg > last_deg){
            last_deg = cache->deg;
            max = vars[i];
        }
    }
    return max;
}

/**
 * La variable qui minimise le rapport de la taille du domaine sur le fd. (domdeg)
 * @param vars
 * @param size
 * @param last
 * @param infos
 * @return
 */
CSPVar next_var_unassigned_dom_deg_cached(CSPVar *vars, size_t size, CSPVar last, CSPInfo infos){
    CSPVar min = NULL;
    float last_value = SHRT_MAX;
    for (int i = 0; i < size; ++i) {
        if(vars[i]->is_assigned) continue;
        struct Cache *cache = vars[i]->cache;

        if(cache->domain_size <= 1) return vars[i];

        if(cache->domain_size / cache->deg < last_value){
            last_value = cache->domain_size / cache->deg;
            min = vars[i];
        }
    }
    return min;
}

/**
 * Pour mettre a jour les domaines et calculer des informations necessaires aux heuristiques
 * @param var
 * @param constraint
 * @param current
 * @param infos
 * @return
 */
bool connected_update_cached(CSPVar var, CSPConstraint constraint, CSPVar current, CSPInfo infos){
    if(var->is_assigned)
        return true;

    struct Cache *cache = var->cache;

    /**
     * Get last domain
     */
    unsigned long last_domain = getLastDomainFromCache(cache, infos.depth);

    /**
     * Reset to last domain
     */

    if(!current->is_assigned){
        cache->content[infos.depth] = 0;
        cache->domain_size = __builtin_popcount(last_domain);
        return true;
    }

    /**
     * Start updating current domain
     */

    if(cache->epoch_updated != infos.epoch){
        cache->epoch_updated = infos.epoch;
        cache->content[infos.depth] = last_domain;
    }

    /**
     * Update current domain
     */

    int reverse = constraint->scope[0] != var;
    if(constraint->constraint_function == &constraint_allDifferent){
        cache->content[infos.depth] &= ~((unsigned long)(1) << (current->domain_value - 1));
    }
    else if(constraint->constraint_function == &constraint_lt){
        cache->content[infos.depth] &= reverse ? (ULONG_MAX << (current->domain_value)) : ~(ULONG_MAX << (current->domain_value - 1));
    }

    cache->domain_size = __builtin_popcount(cache->content[infos.depth]);

    return cache->content[infos.depth] != 0;
}

/**
 * Preparation du cache
 * @param vars
 * @param size
 */
void preprocessing_cached(CSPVar *vars, size_t size){
    for (int k = 0; k < size; ++k) {
        CSPVar var = vars[k];

        struct Cache *cache = var->cache;
        cache->init_content = 0;

        /**
         * Init domain restriction and mrv
         */

        for (int i = 0; i < var->domain->size; ++i) {
            cache->init_content |= value_to_mask(var->domain->values[i]);
        }

        //cache->content[0] = cache->init_content;
        cache->domain_size = __builtin_popcount(cache->init_content);

        /**
         * Init deg
         */

        cache->deg = var->constraint_count;
    }
}

/**
 * Mise à jour du degre des variables
 * @param var
 * @param last_value
 * @param infos
 * @return
 */
bool assigned_update_cached(CSPVar var, CSPDomainValue last_value, CSPInfo infos){
    if(var->is_assigned && last_value != 0) return true;
    int diff = var->is_assigned ? -1 : 1;

    for (int i = 0; i < var->constraint_count; ++i) {
        CSPConstraint c = var->constraints[i];
        CSPVar other = c->scope[0] == var ? c->scope[1] : c->scope[0];

        if(other->is_assigned) continue;

        struct Cache *cache1 = c->scope[0]->cache;
        struct Cache *cache2 = c->scope[1]->cache;

        cache1->deg+=diff;
        cache2->deg+=diff;
    }
    return true;
}

int _getValueCountCachedDomain(CSPVar var, unsigned long mask, int depth){
    int c = 0;
    for (int i = 0; i < var->constraint_count; ++i) {
        CSPConstraint constraint = var->constraints[i];
        for (int j = 0; j < constraint->scope_count; ++j) {
            if(constraint->scope[j] != var && getLastDomainFromCache(constraint->scope[j]->cache, depth) & mask) c++;
        }
    }
    return c;
}

/**
 * Heuristique de valeur LCV.
 * Entraine un grand nombre d'appels à getLastDomainFromCache
 * @param var
 * @param infos
 * @param endOfDomain
 * @return
 */
CSPDomainValue next_value_lcv_cached(CSPVar var, CSPInfo infos, bool *endOfDomain){
    struct Cache *cache = var->cache;

    if(var->domain_value == 0)
        cache->tested = 0;

    unsigned long last_domain = getLastDomainFromCache(cache, infos.depth);

    CSPDomainValue v = 0;
    int last_value_count = INT_MAX;
    for (int j = 0; j < var->domain->size; ++j) {
        unsigned long mask = value_to_mask(var->domain->values[j]);
        if(mask & cache->tested || !(mask & last_domain))
            continue;

        int count = _getValueCountCachedDomain(var, mask, infos.depth);
        if(count < last_value_count){
            v = var->domain->values[j];
            last_value_count = count;
        }
    }

    if(v == 0){
        *endOfDomain = true;
        return 0;
    }

    cache->tested |= value_to_mask(v);
    return v;
}