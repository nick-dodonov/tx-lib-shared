add_rules("mode.debug", "mode.release")

target("tx-lib-shared")
    set_kind("static")
    add_files("src/foo.cpp")
    add_headerfiles("src/foo.h")
    add_includedirs("src", {public = true})

-- target("tx-lib-shared-demo")
--     set_kind("binary")
--     add_deps("tx-lib-shared")
--     add_files("src/main.cpp")
