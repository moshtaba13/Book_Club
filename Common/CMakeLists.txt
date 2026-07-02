# Create common library
add_library(common_lib
    Book.h
    Book.cpp
add_library(common STATIC
    member.h,member.cpp,Review.h,Review.cpp
)

# Export include directory
target_include_directories(common_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link Qt libraries
target_link_libraries(common_lib PUBLIC Qt6::Core)