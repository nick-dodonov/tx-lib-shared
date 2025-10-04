#define _POSIX_C_SOURCE 200809L // for getcwd todo: move to bazel copts
#include <stdio.h>
#include <unistd.h>
#include "Boot/Boot.h"

int main(int argc, char** argv)
{
    Boot::LogInfo(argc, argv);
    return argc - 1;
}
