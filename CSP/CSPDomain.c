#include "CSP.h"

CSPDomain csp_domain_create(CSPDomainValue *values, size_t size) {
    CSPDomain result = malloc(sizeof(struct CSPDomain));

    result->values = malloc(size * sizeof(CSPDomainValue));
    memcpy(result->values, values, size * sizeof(CSPDomainValue));
    result->size = size;

    return result;
}

void csp_domain_delete(CSPDomain domain) {
    free(domain->values);
    free(domain);
}