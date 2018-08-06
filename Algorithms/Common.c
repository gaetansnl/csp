//
// Created by Gaetan on 24/04/2017.
//

#include "Common.h"

/**
 * Convertir une valeur en masque binaire.
 * Ne pas utiliser en cas d'appels trop nombreux
 * @param v
 * @return
 */
unsigned long value_to_mask(CSPDomainValue v){
    return (unsigned long)(1) << (v - 1);
}

/**
 * Obtient le dernier domaine à partir du cache
 * @param cache
 * @param depth
 * @return
 */
unsigned long getLastDomainFromCache(struct Cache *cache, int depth){
    for (int i = depth - 1; i >= 0 ; --i) {
        if(cache->content[i]) return cache->content[i];
    }
    return cache->init_content;
}