add_rules("mode.debug", "mode.release")

target("tx-pkg-misc")
    set_kind("static")

    add_files("src/foo.cpp")
    add_headerfiles("src/foo.h")

    add_files("src/Shared/Version.cpp")
    add_headerfiles("src/Shared/Version.h", {prefixdir = "Shared"})

    add_includedirs("src", {public = true})

-- target("tx-pkg-misc-demo")
--     set_kind("binary")
--     add_deps("tx-pkg-misc")
--     add_files("src/main.cpp")
