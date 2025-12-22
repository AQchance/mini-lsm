set_project("mini-lsm")
set_version("0.1.0")

set_toolset("cxx", "g++") -- 设置 C++ 编译器为 g++
set_toolset("cc", "gcc")   -- 设置 C 编译器为 gcc
set_toolset("ld", "g++")  -- 设置链接器为 g++

add_languages("c++20")

add_rules("mode.debug", "mode.release")

target("mini-lsm")
    set_kind("binary")
    add_files("src/**.cpp") -- ** 递归匹配所有子目录
    add_includedirs("src") -- 添加头文件搜索路径
    set_targetdir("build") -- 设置输出目录
    set_objectdir("build/obj")
