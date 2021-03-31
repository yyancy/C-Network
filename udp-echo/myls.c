//
// Created by yyancy on 2020/3/15.
//

#include <stdio.h>
#include <dirent.h>
#include <unistd.h>

int
main(int argc, char *argv[]) {
    DIR *dp;
    struct dirent *dirp;
    if ((dp = opendir("/")) == NULL) {
        perror("error");
    }
    while ((dirp = readdir(dp)) != NULL) {
        printf("%d %s\n", dirp->d_ino, dirp->d_name);
        printf("%c\n", dirp->d_type);
    }

//    for (struct dirent *d = dirp; d != NULL; d++) {
//        printf("%s\n", d->d_name);
//    }
    return 0;
}