//
// Created by Gaetan on 01/04/2017.
//
#include <stdlib.h>
#include <stdio.h>
#include "CSP.h"

/**
 * Utilisé pour vérifier si une variable a déjà été visitée
 */

bool _callback_connected_vars(CSPVar current, CSPConfig config, CSPInfo infos) {
    if (config.connectedCallback == NULL) return true;

    for (int i = 0; i < current->constraint_count; ++i) {
        for (int j = 0; j < current->constraints[i]->scope_count; ++j) {
            if(current->constraints[i]->scope[j] == current) continue;
            if (!config.connectedCallback(current->constraints[i]->scope[j], current->constraints[i], current, infos))
                return false;
        }
    }

    return true;
}

/**
 * Gestion du cache
 */

char *cache = NULL;

void alloc_cache_and_reset(CSPVar *vars, size_t var_count, CSPConfig config) {
    if (config.cache_size == 0) return;
    cache = calloc(var_count, config.cache_size);
    for (size_t i = 0; i < var_count; ++i) {
        csp_var_reset(vars[i]);
        vars[i]->cache = cache + (i * config.cache_size);
    }
}

void free_cache() {
    free(cache);
}

// fonctionne uniquement pour des durées courtes car pas timestamp
clock_t get_time(){
    return clock() / (CLOCKS_PER_SEC / 1000);
}

/**
 * Assigned les variables manquantes en focntion de la configuration spécifiée
 * @param vars
 * @param var_count
 * @param config
 * @return
 */
CSPStats csp(CSPVar *vars, size_t var_count, CSPConfig config) {
    if (config.nextVar == NULL || config.nextValue == NULL)
        exit(1);

    /**
     * Initialisation cache
     */

    alloc_cache_and_reset(vars, var_count, config);

    /**
     *  PreProcessing
     */

    if (config.preProcessingCallback != NULL) {
        config.preProcessingCallback(vars, var_count);
    }

    /**
     * Initialisation stack
     */
    CSPStats stats = {0};
    CSPInfo infos = {0};

    CSPVar *stack = calloc(var_count, sizeof(CSPVar));
    stack[0] = config.nextVar(vars, var_count, NULL, infos);
    clock_t start = get_time();

    while (stack[0] != NULL) {
        CSPVar current_var = stack[infos.depth];
        bool endOfDomain = false;
        CSPDomainValue next_value = config.nextValue(current_var, infos, &endOfDomain);
        infos.epoch++;

        CSPDomainValue last_value = current_var->domain_value;

        /**
         * On a parcouru toutes les valeurs disponibles pour le domaine
         */
        if (endOfDomain) {
            csp_var_reset(current_var);

            if (config.assignedCallback != NULL)
                config.assignedCallback(current_var, last_value, infos);

            _callback_connected_vars(current_var, config, infos);

            stack[infos.depth] = NULL;
            infos.depth--;
            continue;
        }

        csp_var_set(current_var, next_value);
        stats.nodes++;

        if (config.assignedCallback != NULL && ! config.assignedCallback(current_var, last_value, infos))
            continue;

        if (!config.disable_consitency_check && !csp_var_consistent(current_var) || !_callback_connected_vars(
                current_var, config, infos) && config.forward_checking) {
            continue;
        }

        /**
         * On a trouvé une valeur qui rend la variable consistante
         * et on passe à la variable suivante si elle existe
         */

        CSPVar next_var = config.nextVar(vars, var_count, current_var, infos);

        if (next_var == NULL) {
            stats.success = true;
            break;
        } else {
            infos.depth++;
            stack[infos.depth] = next_var;
        }
    }

    stats.elapsed_ms = get_time() - start;

    free(stack);
    free_cache();

    return stats;
}