add_rules("mode.debug", "mode.release")
set_languages("c++17")

add_repositories("tx-kit-repo " .. (os.getenv("TX_REPO") or "git@github.com:nick-dodonov/tx-kit-repo.git main"))
add_requires("tx-kit")
includes(".xmake/tx-kit/includes")

add_requires("tx-pkg-aux")

target("tx-pkg-misc", function()
    set_kind("static")

    add_files("src/Shared/Version.cpp")
    add_headerfiles("src/Shared/Version.h", {prefixdir = "Shared"})

    add_includedirs("src", {public = true})

    add_packages("tx-pkg-aux")
end)
