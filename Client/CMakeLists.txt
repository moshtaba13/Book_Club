cmake_minimum_required(VERSION 3.19)
project(Client LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_resources(RESOURCES
    resources.qrc
)

qt_add_executable(Client
    WIN32
    MACOSX_BUNDLE

    general/login.cpp
    general/login.h

    general/main.cpp

    ${RESOURCES}
    
    user/home.cpp
    user/home.h
    general/mainwindow.h
    general/mainwindow.cpp
    user/register.h
    user/register.cpp

    ../Common/models/Book.h
    ../Common/models/Book.cpp

    ../Common/models/Review.h
    ../Common/models/Review.cpp
    user/bookdetailwindow.h
    user/bookdetailwindow.cpp
    
    ../Common/models/Cart.cpp
    ../Common/models/Cart.h

    user/cartwidget.h
    user/cartwidget.cpp
    
    ../Common/users/User.h
    ../Common/users/User.cpp
    ../Common/models/Purchase.h
    ../Common/models/Purchase.cpp
      ../Common/users/Member.h
    ../Common/users/Member.cpp
    user/usermanager.h
    user/usermanager.cpp



      ../Common/models/Shelf.h
    ../Common/models/Shelf.cpp
    ../Common/models/Notification.h
    ../Common/models/Notification.cpp
    user/genreselectionwidget.h
    user/genreselectionwidget.cpp
    user/forgotpasswordwidget.h
    user/forgotpasswordwidget.cpp
    user/profilewidget.h
    user/profilewidget.cpp
    user/personallibrarywidget.h
    user/personallibrarywidget.cpp
    publisher/publishermanager.h
    publisher/publishermanager.cpp
    publisher/registerpublisher.h
    publisher/registerpublisher.cpp


    ../Common/users/Publisher.h
    ../Common/users/Publisher.cpp
    admin/adminloginwidget.h
    admin/adminloginwidget.cpp

    ../Common/users/Admin.h
    ../Common/users/Admin.cpp
    user/notificationwidget.h
    user/notificationwidget.cpp
    publisher/addeditbookdialog.h
    publisher/addeditbookdialog.cpp
    publisher/publisherdashboardwidget.h
    publisher/publisherdashboardwidget.cpp
    admin/adminpanelwidget.h
    admin/adminpanelwidget.cpp


)
target_link_libraries(Client
    PRIVATE
        Qt::Core
        Qt::Widgets
)
target_include_directories(Client PRIVATE
    ../Common/models
    ../Common/users
    ${CMAKE_CURRENT_SOURCE_DIR}/general
    ${CMAKE_CURRENT_SOURCE_DIR}/user
    ${CMAKE_CURRENT_SOURCE_DIR}/admin
    ${CMAKE_CURRENT_SOURCE_DIR}/publisher

)

install(TARGETS Client
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET Client
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)

install(SCRIPT ${deploy_script})