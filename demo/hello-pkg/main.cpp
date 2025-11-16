#include "Boot/Boot.h"

int main(int argc, char** argv)
{
    Boot::LogHeader(argc, argv);
    return argc - 1;
}
