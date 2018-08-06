//
// Created by Gaetan on 03/04/2017.
//

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "FutoshikiReader.h"
#include "Constraints/Constraints.h"

CSPDomain create_domain(size_t size){
    CSPDomainValue *values = malloc(sizeof(int) * size);
    for (int i = 1; i <= size; ++i)
        values[i - 1] = i;
    CSPDomain d = csp_domain_create(values, size);
    free(values);
    return d;
}

size_t pos_to_index(size_t row, size_t column, size_t size){
    return row * size + column;
}

void add_constraint_horizontal(FutoshikiCSPGrid g, size_t row, size_t column, size_t size, char c){
    if(column == 0 ) return;
    if(c == '<'){
        CSPVar vars[2] = {g->vars[pos_to_index(row, column - 1, size)], g->vars[pos_to_index(row, column, size)]};
        csp_constraint_add(vars, 2, &constraint_lt);
    }else if(c == '>'){
        CSPVar vars[2] = {g->vars[pos_to_index(row, column, size)], g->vars[pos_to_index(row, column - 1, size)]};
        csp_constraint_add(vars, 2, &constraint_lt);
    }
}

void add_constraint_vertical(FutoshikiCSPGrid g, size_t row, size_t column, size_t size, char c){
    if(c == '^'){
        CSPVar vars[2] = {g->vars[pos_to_index((row - 1) / 2, column, size)], g->vars[pos_to_index((row + 1) / 2, column, size)]};
        csp_constraint_add(vars, 2, &constraint_lt);
    }else if(c == 'v'){
        CSPVar vars[2] = {g->vars[pos_to_index((row + 1) / 2, column, size)], g->vars[pos_to_index((row - 1) / 2, column, size)]};
        csp_constraint_add(vars, 2, &constraint_lt);
    }
}

CSPDomain getOneValueDomain(CSPDomainValue value){
    CSPDomain d = create_domain((size_t) 1);
    d->values[0] = value;
    return d;
}


FutoshikiCSPGrid fut_create_grid(char *fileName) {
    /**
     * Ouverture du fichier et lecture de la taille de la grille
     */

    FILE *file = fopen(fileName, "r");
    char line[256];

    if(!fgets(line, sizeof(line), file))
        return NULL;

    size_t grid_size = (size_t) atoi(line);

    /**
     * Creation du domaine et des variables
     */

    FutoshikiCSPGrid result = malloc(sizeof(FutoshikiCSPGrid));
    result->size = (size_t) grid_size * grid_size;
    result->vars = malloc(sizeof(CSPVar) * result->size);

    CSPDomain d = create_domain((size_t) grid_size);

    for (int k = 0; k < result->size; ++k) {
        result->vars[k] = csp_var_create(d);
    }

    /**
     * Initialisation des variables et contraintes
     */

    for (size_t i = 0; i < grid_size * 2 - 1; ++i) {
        fgets(line, sizeof(line), file);
        if(i % 2 == 0){
            char* pos = line;
            for (size_t j = 0; j < grid_size; ++j) {
                //Valeur
                int content = atoi(pos);
                if(content > 0){
                    result->vars[pos_to_index(i / 2, j, grid_size)]->domain = getOneValueDomain((CSPDomainValue) content);
                    //csp_var_set(result->vars[pos_to_index(i / 2, j, grid_size)], (CSPDomainValue) content);
                }
                add_constraint_horizontal(result, i / 2, j, grid_size, *(pos - 1));
                pos = strpbrk(pos, "<> ") + 1;
            }
        }else{
            for (size_t j = 0; j < grid_size; ++j) {
                add_constraint_vertical(result, i, j, grid_size, *(line + j));
            }
        }
    }

//    //Constraint
//
//    CSPVar *vars = malloc(grid_size * sizeof(CSPVar));
//    for (size_t i = 0; i < grid_size; ++i) {
//        csp_constraint_add(result->vars + i * grid_size, grid_size, &constraint_allDifferent);
//        for (size_t j = 0; j < grid_size; ++j) {
//            vars[j] = result->vars[pos_to_index(j, i, grid_size)];
//        }
//        csp_constraint_add(vars, grid_size, &constraint_allDifferent);
//    }
//    free(vars);

//    for (int i = 0; i < grid_size; ++i) {
//        for (int j = 0; j < grid_size; ++j) {
//            CSPVar current = result->vars[pos_to_index(i, j, grid_size)];
//            for (int k = 0; k < grid_size; ++k) {
//                CSPVar v1[2] = {current, result->vars[pos_to_index(k, j, grid_size)]};
//                CSPVar v2[2] = {current, result->vars[pos_to_index(i, k, grid_size)]};
//                if(v1[0] != v1[1]) csp_constraint_add(v1, 2, &constraint_allDifferent);
//                if(v2[0] != v2[1]) csp_constraint_add(v2, 2, &constraint_allDifferent);
//            }
//        }
//    }
    for (size_t l = 0; l < grid_size ; ++l) {
        for (size_t i = 0; i < grid_size; ++i) {
            for (size_t j = i + 1; j < grid_size; ++j) {
                CSPVar v1[2] = {result->vars[pos_to_index(l, i, grid_size)], result->vars[pos_to_index(l, j, grid_size)]};
                CSPVar v2[2] = {result->vars[pos_to_index(i, l, grid_size)], result->vars[pos_to_index(j, l, grid_size)]};
                csp_constraint_add(v1, 2, &constraint_allDifferent);
                csp_constraint_add(v2, 2, &constraint_allDifferent);
            }
        }
    }

    return result;
}

void fut_print_grid(FutoshikiCSPGrid g){
    for (int i = 0; i < g->size; ++i) {
        printf("%d ", g->vars[i]->domain_value);
        if((i + 1) % (int)sqrt(g->size) == 0)
            printf("\n");
    }
}
