//
// Created by Gaetan on 24/04/2017.
//

#ifndef UNTITLED1_COMMON_H
#define UNTITLED1_COMMON_H

#include "../CSP/CSP.h"

struct Cache{
    unsigned long content[81];
    unsigned long init_content;
    unsigned long tested;
    bool not_entry_inf;
    bool not_entry_gt;
    unsigned int epoch_updated;
    int domain_size;
    int deg;
};

unsigned long value_to_mask(CSPDomainValue v);
unsigned long getLastDomainFromCache(struct Cache *cache, int depth);

#endif //UNTITLED1_COMMON_H
