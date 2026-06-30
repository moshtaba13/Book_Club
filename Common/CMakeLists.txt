add_library(common STATIC
    # فایل‌های .h و .cpp رو خودت بعداً اینجا اضافه کن
)

target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(common PUBLIC Qt6::Core)