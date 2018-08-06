#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "CSP/CSP.h"
#include "FutoshikiReader.h"
#include "Algorithms/Configs.h"

int main() {
    FutoshikiCSPGrid g = fut_create_grid("grille1_8x8.fut");
    CSPConfig config = forward_checking_mrv_deg_ep_get_config();
    CSPStats result = csp(g->vars, g->size, config);


    if(!result.success){
        exit(1);
    }

    fut_print_grid(g);
    printf("\n");
    printf("Result : %s\n", result.success ? "true" : "false");
    printf("Nodes : %d\n",result.nodes);
    printf("Elapsed ms : %d\n", result.elapsed_ms);
    getchar();
    return 0;
}