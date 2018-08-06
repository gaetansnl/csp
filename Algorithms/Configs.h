//
// Created by Gaetan on 27/04/2017.
//

#ifndef UNTITLED1_CONFIGS_H
#define UNTITLED1_CONFIGS_H
#include "../CSP/CSP.h"

CSPConfig backtracking_get_config();
CSPConfig backtracking_cache_mrv_get_config();
CSPConfig backtracking_cache_dom_deg_get_config();
CSPConfig backtracking_cache_mrv_deg_get_config();

CSPConfig forward_checking_get_config();
CSPConfig forward_checking_mrv_get_config();
CSPConfig forward_checking_dom_deg_get_config();
CSPConfig forward_checking_mrv_deg_get_config();
CSPConfig forward_checking_mrv_deg_lcv_get_config();
CSPConfig  forward_checking_mrv_deg_ep_get_config();
CSPConfig forward_checking_dom_deg_ep_get_config();
CSPConfig forward_checking_mrv_ep_get_config();
CSPConfig forward_checking_mrv_deg_lcv_ep_get_config();

#endif //UNTITLED1_CONFIGS_H
