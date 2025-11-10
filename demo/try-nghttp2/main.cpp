#include "Boot/Boot.h"
#include "Log/Log.h"
#include <nghttp2/nghttp2.h>

int main(int argc, char** argv)
{
    Boot::LogInfo(argc, argv);
    
    nghttp2_info *info = nghttp2_version(0);
    Log::InfoF("nghttp2 version: {}", info->version_str);
    
    return 0;
}
