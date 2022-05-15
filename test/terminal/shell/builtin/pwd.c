#include "pwd.h"

#ifndef MAX_BUF
#define MAX_BUF 2042
#endif

int pwd(void) {
    char path[MAX_BUF];

    if (getcwd(path, MAX_BUF) == NULL) {
        exit(EXIT_FAILURE);
        return -1;
    }
    printf("\033[0;33m");
    printf("[ʕ•́ᴥ•̀ʔっ...] you are at:");
    printf("\033[0m"); 
    printf("%s", path);
    return 0;
}

int main(void){ 
    int res = pwd();
    return res;
}
