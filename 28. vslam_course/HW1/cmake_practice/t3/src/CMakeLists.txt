# SET 进行显式定义
SET(src t1.cpp)

# 这里需要注意 需要添加 ${}
add_executable(t1 ${src})

# TAGRETS 可以是（目标！）二进制文件、动态库、静态库
# 也就是说add_executable 和 add_library 的目标
install(TARGETS t1 
        RUNTIME DESTINATION bin)

# FILES 就是一般文件
install(FILES ../doc/hello.txt ../readme.md
        DESTINATION share/doc/cmake)

# PROGRAMS 非目标文件的可执行文件
install(PROGRAMS ../runhello.sh
        DESTINATION bin)

# DIRECTORY
# DIRECTORY 后面连接的是所在 Source 目录的相对路径，但务必注意：
# abc 和 abc/有很大的区别。
# 如果目录名不以/结尾，那么这个目录将被安装为目标路径下的 abc，如果目录名以/结尾，
# 代表将这个目录中的内容安装到目标路径，但不包括这个目录本身。