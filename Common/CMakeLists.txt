add_library(common STATIC
    models/Book.h
    models/Book.cpp
    models/Review.h
    models/Review.cpp
    models/Cart.h
    models/Cart.cpp
    models/Shelf.h
    models/Shelf.cpp
    models/Notification.h
    models/Notification.cpp
    models/Purchase.h
    models/Purchase.cpp
    users/Member.h
    users/Member.cpp
    users/User.h
    users/User.cpp
    users/Publisher.h
    users/Publisher.cpp
    users/Admin.h
    users/Admin.cpp
    protocol.h
)

target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(common PUBLIC Qt6::Core)
