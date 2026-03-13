#include "parser.tab.h"
#include <stdio.h>

int main() {
    int result = -1;
    int returned = yyparse(&result);
    printf("returned: %d\nresult: %d\n", returned, result);
}
