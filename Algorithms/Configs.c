//
// Created by Gaetan on 27/04/2017.
//

#include <limits.h>
#include "Configs.h"
#include "Common.h"
#include "Cached.h"
#include "EPPreprocess.h"
#include "Uncached.h"

/**
 * Configurations
 */

CSPConfig backtracking_get_config() {
    CSPConfig result = {0};
    result.nextVar = &next_var_unassigned;
    result.nextValue = &next_value_default;
    return result;
}

CSPConfig _backtracking_cache_get_config() {
    CSPConfig result = {0};
    result.nextVar = &next_var_unassigned;
    result.nextValue = &next_value_default;

    result.cache_size = sizeof(struct Cache);
    result.preProcessingCallback = &preprocessing_cached;
    result.connectedCallback = &connected_update_cached;

    return result;
}

CSPConfig backtracking_cache_mrv_get_config() {
    CSPConfig result = _backtracking_cache_get_config();
    result.nextVar = &next_var_unassigned_mrv_cached;
    return result;
}

CSPConfig backtracking_cache_dom_deg_get_config() {
    CSPConfig result = _backtracking_cache_get_config();
    result.nextVar = &next_var_unassigned_dom_deg_cached;
    result.assignedCallback = &assigned_update_cached;
    return result;
}

CSPConfig backtracking_cache_mrv_deg_get_config() {
    CSPConfig result = _backtracking_cache_get_config();
    result.nextVar = &next_var_unassigned_mrv_deg_cached;
    result.assignedCallback = &assigned_update_cached;
    return result;
}

/**
 * Forward Checking
 * @return
 */

CSPConfig forward_checking_get_config() {
    CSPConfig result = {0};
    result.forward_checking = true;
    result.disable_consitency_check = true;
    result.nextVar = &next_var_unassigned;
    result.nextValue = &next_value_cached;

    result.cache_size = sizeof(struct Cache);
    result.preProcessingCallback = &preprocessing_cached;
    result.connectedCallback = &connected_update_cached;

    return result;
}

CSPConfig forward_checking_mrv_get_config() {
    CSPConfig result = forward_checking_get_config();
    result.nextVar = &next_var_unassigned_mrv_cached;
    return result;
}

CSPConfig forward_checking_dom_deg_get_config() {
    CSPConfig result = forward_checking_get_config();
    result.nextVar = &next_var_unassigned_dom_deg_cached;
    result.assignedCallback = &assigned_update_cached;
    return result;
}

CSPConfig forward_checking_mrv_deg_get_config() {
    CSPConfig result = forward_checking_get_config();
    result.nextVar = &next_var_unassigned_mrv_deg_cached;
    result.assignedCallback = &assigned_update_cached;
    return result;
}

CSPConfig forward_checking_mrv_deg_lcv_get_config() {
    CSPConfig result = forward_checking_mrv_deg_get_config();
    result.nextValue = &next_value_lcv_cached;
    return result;
}

CSPConfig forward_checking_mrv_deg_ep_get_config() {
    CSPConfig result = forward_checking_mrv_deg_get_config();
    result.preProcessingCallback = &cache_preprocessing_ep;
    return result;
}

CSPConfig forward_checking_mrv_ep_get_config() {
    CSPConfig result = forward_checking_mrv_get_config();
    result.preProcessingCallback = &cache_preprocessing_ep;
    return result;
}

CSPConfig forward_checking_dom_deg_ep_get_config() {
    CSPConfig result = forward_checking_dom_deg_get_config();
    result.preProcessingCallback = &cache_preprocessing_ep;
    return result;
}

CSPConfig forward_checking_mrv_deg_lcv_ep_get_config() {
    CSPConfig result = forward_checking_mrv_deg_lcv_get_config();
    result.preProcessingCallback = &cache_preprocessing_ep;
    return result;
}

