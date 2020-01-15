cmake_minimum_required(VERSION 2.8)
#cmake_policy(SET CMP0015 OLD)
set(PROJ_NAME IMCP_test_client)

############################################################
# include tars_dir.cmake
INCLUDE("tars_dir.cmake")

############################################################
# 自定义的配置文件，多个配置文件用空格分割，运行make tar将一起打包到安装包中
set(CONFIG "") 
# compile attribute
set(CXX_FLAGS "")
set(C_FLAGS "")

# 项目依赖的其他tars文件, 当前目录下的tars文件不用填写在这里
# 每行一个tars文件路径
set(DEPENDS_TARS_FILES
    "Hello.tars"
    )

# include directors
include_directories(../base
)

message(${CMAKE_CURRENT_BINARY_DIR})
message(${CMAKE_CURRENT_SOURCE_DIR})

# link directors
link_directories(
)

# link libraries
link_libraries(
)

############################################################
# include tars.cmake
INCLUDE("${TARS_INSTALL_DIR}/cmake/tars.cmake")
