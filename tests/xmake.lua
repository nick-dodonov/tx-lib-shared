add_rules("mode.debug", "mode.release")
set_languages("c++17")

add_requires("gtest")

-- Include parent project
includes("../")

target("tx-pkg-misc-test")
    set_kind("binary")
    set_group("tests")

    add_deps("tx-pkg-misc")
    add_packages("gtest")

    add_files("*.cpp")
    
    if is_mode("debug") then
        add_defines("DEBUG")
    end
