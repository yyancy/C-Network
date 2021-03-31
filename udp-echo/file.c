//
// Created by yyancy on 2020/3/15.
//

#include <stdio.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>

char *print_filetype(struct stat *st) {
    char *message;
    if (S_ISDIR(st->st_mode)) {
        message = "d";
    } else if (S_ISREG(st->st_mode)) {
        message = "-";

//    } else if(){
//    } else if(){
//    } else if(){
        return message;
//    } else{

    }
}

char *print_premission(struct stat *st) {
    printf("%s", print_filetype(st));
    char message[9] = {'-', '-', '-', '-', '-', '-', '-', '-', '-'};
    if ((S_IRUSR & st->st_mode) == S_IRUSR) {
        message[0] = 'r';
    }
    if ((S_IWUSR & st->st_mode) == S_IWUSR) {
        message[1] = 'w';
    }
    if ((S_IXUSR & st->st_mode) == S_IXUSR) {
        message[2] = 'x';
    }
    if ((S_IRGRP & st->st_mode) == S_IRGRP) {
        message[3] = 'r';
    }
    if ((S_IWGRP & st->st_mode) == S_IWGRP) {
        message[4] = 'w';
    }
    if ((S_IXGRP & st->st_mode) == S_IXGRP) {
        message[5] = 'x';
    }
    if ((S_IROTH & st->st_mode) == S_IROTH) {
        message[6] = 'r';
    }
    if ((S_IWOTH & st->st_mode) == S_IWOTH) {
        message[7] = 'w';
    }
    if ((S_IXOTH & st->st_mode) == S_IXOTH) {
        message[8] = 'x';
    }
    printf("%s ", message);
}

void print_user(struct stat *st) {
    struct passwd *p;
    if ((p = getpwuid(st->st_uid)) == NULL) {
        perror("errr");
    }
    printf("%s ", p->pw_name);
}

void print_group(struct stat *st) {
    struct group *g = getgrgid(st->st_gid);
    if (g == NULL) {
        perror("err");
    }

    printf(" %s", g->gr_name);

}

void print_file_information(char *name) {
    struct stat st;
    int err;
    if ((err = stat(name, &st)) < 0) {
        perror("err");
    }

    print_premission(&st);
    printf("%d ", st.st_size);

    print_user(&st);
    print_group(&st);
    printf(" %s\n", name);
//    st.st_mode
}

int main() {

    DIR *dp;
    struct dirent *dirp;
    chdir("/");
    if ((dp = opendir("/")) == NULL) {
        perror("error");
    }
    while ((dirp = readdir(dp)) != NULL) {
//        printf("%d %s\n", dirp->d_ino, dirp->d_name);
//        printf("%c\n", dirp->d_type);
        print_file_information(dirp->d_name);
    }


    return 0;
}