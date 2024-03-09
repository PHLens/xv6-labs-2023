#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

void
find(char* path, char* name) {
    char buf[512], *p;
    int fd;
    struct stat st;
    struct dirent de;
    if ((fd = open(path, O_RDONLY)) < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", path);
        return;    
    }

    switch (st.type){
    case T_DEVICE:
    case T_FILE: 
    case T_DIR:
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';
        while (read(fd, &de, sizeof(de)) == sizeof(de)) {
            if (de.inum == 0) // TODO: FOR WHAT?
                continue;
            // exclude '.' and '..'
            if (!strcmp(de.name, ".") || !strcmp(de.name, "..")) continue;
            memmove(p, de.name, DIRSIZ);  // cat name after path.
            p[DIRSIZ] = 0;  // strings end with '0'
            if (stat(buf, &st) < 0) {
                fprintf(2, "find: cannot stat %s\n", buf);
                continue;
            }
            if (st.type == T_FILE && !strcmp(de.name, name)) {
                printf("%s\n", buf);
                continue;
            }
            if (st.type == T_DIR) {
                find(buf, name);
                continue;
            }
        }
        break;
    }
    close(fd);
}


int
main (int argc, char* argv[]) {
    if (argc < 3) {
        find(".", argv[1]);
        exit(0);
    }
    find(argv[1], argv[2]);
    exit(0);
}