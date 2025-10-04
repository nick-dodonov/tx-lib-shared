#include "Boot/Boot.h"

int main(int argc, char** argv)
{
    Boot::LogInfo(argc, argv);
    return argc - 1;
}
