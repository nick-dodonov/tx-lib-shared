add_rules("mode.debug", "mode.release")
set_languages("c++17")

target("tx-pkg-misc", function()
    set_kind("static")

    add_files("src/Shared/Version.cpp")
    add_headerfiles("src/Shared/Version.h", {prefixdir = "Shared"})

    add_includedirs("src", {public = true})
end)
