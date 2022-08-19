# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html


# todo 指定cmake最低支持版本
cmake_minimum_required(VERSION 3.10.2)

# todo ==========  添加头文件自动加载路径，该路径下的头文件会自动寻找到（相对于本文件路径）
include_directories(include)

project("basejnipro")

# todo ==========  我们编写的代码需要在这里声明
# todo 定义一个全局变量，自动引入文件
file(GLOB ALL_FILE *.c *.h *.cpp)

add_library( # Sets the name of the library.
        basejnipro
        # Sets the library as a shared library.
        SHARED
        # Provides a relative path to your source file(s).
        #todo 如果没有全局定义的自动引入，则需要手动一个一个引入。
#        native-lib.cpp
#        Cdemo.cpp
#        CPlusDemo.cpp

        #todo 自动全部引入
        ${ALL_FILE}
        )

# todo ==========  so库路径的指定方式一，手动指定。
# todo #1#指定so库的目录路径。
# todo 通常如果编写代码时，调用so库的函数没有报红，但编译时报错找不到，则需要考虑so库路径是否有误，导致函数编译无法链接。
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${CMAKE_SOURCE_DIR}/../jniLibs/${CMAKE_ANDROID_ARCH_ABI}")


# todo #2#当然也支持指定单个so库路径，进行导入，这样的方式可以修改link时库的名字。
## 1. 设置ffmpeg库所在路径的变量
#set(FFSO ${CMAKE_CURRENT_SOURCE_DIR}/../libs/${ANDROID_ABI})
## 2. 把ffmpeg指定的动态库加进来。
#add_library(avcodec SHARED IMPORTED)
#set_target_properties(avcodec PROPERTIES IMPORTED_LOCATION ${FFSO}/libavcodec.so)
## 3. 同样需要将avcodec添加到target_link_libraries


# todo ==========  so库路径的指定方式二，自动加载NDK下的库。
# todo 自动寻找NDK下的库 -- AndroidSDK\Sdk\ndk\21.0.xxx
find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

# todo 具体添加so库到编译中
target_link_libraries( # Specifies the target library.
        basejnipro
        # Links the target library to the log library
        # included in the NDK.
        ${log-lib}
        avcodec avformat avutil swscale swresample
        )