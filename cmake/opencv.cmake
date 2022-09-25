
# 制定 opencv 的搜索路径
# set(OpenCV_DIR "/home/sunny/3rdparty/OpenCV-3.3.1/share/OpenCV")

find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )
link_libraries(${OpenCV_LIBS})