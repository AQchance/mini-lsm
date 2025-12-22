set_project("mini-lsm")
set_version("0.1.0")

set_toolset("cxx", "g++") -- 设置 C++ 编译器为 g++
set_toolset("cc", "gcc")   -- 设置 C 编译器为 gcc
set_toolset("ld", "g++")  -- 设置链接器为 g++

add_languages("c++20")

add_rules("mode.debug", "mode.release")

-- 主程序
target("mini-lsm")
    set_kind("binary")
    add_files("src/**.cpp")
    remove_files("src/test/**.cpp")  -- 排除测试目录
    add_includedirs("src")
    set_targetdir("build")
    set_objectdir("build/obj")

-- 测试程序
target("test")
    set_kind("binary")
    add_files("src/**.cpp")
    remove_files("src/main.cpp")  -- 排除主程序入口
    add_includedirs("src")
    set_targetdir("build")
    set_objectdir("build/obj/test")
