//
// Created by Gaetan on 31/03/2017.
//
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifndef UNTITLED1_CSP_H
#define UNTITLED1_CSP_H

typedef int CSPDomainValue;

typedef struct CSPDomain *CSPDomain; // Mauvaise pratique de masquer le pointeur mais pour simplifier ecriture
typedef struct CSPVar *CSPVar;
typedef struct CSPConstraint *CSPConstraint;

typedef struct CSPInfo{
    unsigned int epoch;
    size_t depth;
} CSPInfo;

typedef bool (*CSPConstraintFunction)(CSPConstraint constraint, CSPVar current);

typedef CSPDomainValue (*CSPDomainNextFunction)(CSPVar var, CSPInfo infos, bool * endOfDomain);
typedef CSPVar (*CSPVarNextFunction)(CSPVar *vars, size_t size, CSPVar last, CSPInfo infos);

typedef bool (*CSPConnectedCallback)(CSPVar var, CSPConstraint constraint, CSPVar current, CSPInfo infos);
typedef bool (*CSPAssignedCallback)(CSPVar var, CSPDomainValue last_value, CSPInfo infos);
typedef void (*CSPPreProcessingCallback)(CSPVar *vars, size_t var_count);

typedef struct CSPConfig {
    CSPVarNextFunction nextVar;
    CSPDomainNextFunction nextValue;
    CSPConnectedCallback connectedCallback;
    CSPAssignedCallback assignedCallback;
    CSPPreProcessingCallback preProcessingCallback;
    size_t cache_size;
    bool forward_checking;
    bool disable_consitency_check;
} CSPConfig;

typedef struct CSPStats{
    int nodes;
    bool success;
    clock_t elapsed_ms;
} CSPStats;

struct CSPDomain {
    CSPDomainValue* values;
    size_t size;
};

struct CSPVar {
    CSPDomain domain;

    CSPConstraint *constraints;
    size_t constraint_count;

    bool is_assigned;
    CSPDomainValue domain_value;

    //short last_epoch;
    void* cache;
};

struct CSPConstraint {
    CSPVar *scope;
    size_t scope_count;
    CSPConstraintFunction constraint_function;
};

CSPDomain csp_domain_create(CSPDomainValue *values, size_t size);
void csp_domain_delete(CSPDomain domain);

CSPVar csp_var_create(CSPDomain domain);
void csp_var_set(CSPVar var, CSPDomainValue domainValue);
bool csp_var_assigned(CSPVar var);
void csp_var_reset(CSPVar var);
void csp_var_delete(CSPVar var);
bool csp_var_consistent(CSPVar var);

CSPConstraint csp_constraint_add(CSPVar *scope, size_t scope_size, CSPConstraintFunction constraint_function);
bool csp_constraint_solved(CSPConstraint constraint, CSPVar current);
bool csp_constraint_has_unassigned(CSPConstraint constraint);
CSPDomainValue csp_constraint_get_scope_value(CSPConstraint constraint, size_t index);

CSPStats csp(CSPVar *vars, size_t var_count, CSPConfig config);

#endif //UNTITLED1_CSP_H
