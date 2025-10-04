#define _POSIX_C_SOURCE 200809L // for getcwd todo: move to bazel copts
#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv)
{
    printf("TODO: hello-pkg (dep to personal bazel module)\n");
    // to verify args and cwd from launch.json
    for (int i = 0; i < argc; i++) {
        printf("  [%d]: %s\n", i, argv[i]);
    }
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("  dir: %s\n", cwd);
    } else {
        perror("  dir: getcwd() error");
    }
    return 0;
}
