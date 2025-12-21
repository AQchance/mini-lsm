add_project("mini-lsm")
add_version("0.1.0")

add_languages("c++20")

add_rules("mode.debug", "mode.release")

target("mini-lsm")
    set_kind("binary")
    add_files("src/*.cpp")
