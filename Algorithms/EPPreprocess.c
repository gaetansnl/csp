//
// Created by Gaetan on 27/04/2017.
//

#include <limits.h>
#include <math.h>
#include <stdio.h>
#include "EPPreprocess.h"
#include "../Constraints/Constraints.h"
#include "Common.h"
#include "Cached.h"

#ifdef _MSC_VER
#  include <intrin.h>
#  define __builtin_popcount __popcnt
#endif

void applyMask(CSPVar v, unsigned long mask){
    struct Cache *cache = v->cache;
    cache->init_content &= mask;
    cache->content[0] &= mask;
    cache->domain_size = __builtin_popcount(cache->init_content);
}

/**
 * dir == 1 -> direction inf
 * dir == 0 -> direction gt
 * @param v
 * @param depth
 * @param dir
 */
void mask_ranges(CSPVar v, unsigned long depth, int dir){
    applyMask(v, dir ? ~(ULONG_MAX << (depth)) >> 1 : (ULONG_MAX << (depth)));
    for (int i = 0; i < v->constraint_count; ++i) {
        if(v->constraints[i]->constraint_function == constraint_lt && v->constraints[i]->scope[dir] == v)
            mask_ranges(v->constraints[i]->scope[!dir], depth + (dir ? -1 : 1), dir);
    }
}

/**
 * Algorithme d'optimisation des domaines basé sur contraintes d'inegalités
 * presentes sur la grille
 * @param vars
 * @param size
 */
void cache_preprocessing_ep(CSPVar *vars, size_t size){
    preprocessing_cached(vars, size);

    /**
     * On trouve les entry pour permettre un parcours plus rapide après
     */
    for (int k = 0; k < size; ++k) {
        CSPVar var = vars[k];
        struct Cache *cache = var->cache;

        for (int j = 0; j < var->constraint_count; ++j) {
            int reverse = var->constraints[j]->scope[0] != var;
            if(var->constraints[j]->constraint_function == &constraint_lt && reverse){
                cache->not_entry_gt = true;
            }
            else if (var->constraints[j]->constraint_function == &constraint_lt && !reverse){
                cache->not_entry_inf = true;
            }
        }
    }

    /**
     * On simplifie les domaines en partant des entry
     */

    for (int k = 0; k < size; ++k) {
        struct Cache *cache = vars[k]->cache;
        if(!cache->not_entry_inf) mask_ranges(vars[k], (unsigned long) (sqrt(size) + 1), 1);
        if(!cache->not_entry_gt) mask_ranges(vars[k], 0, 0);
    }
}