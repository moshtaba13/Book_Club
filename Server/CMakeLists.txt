add_executable(BookClubServer
    Database.h
    Database.cpp
    main.cpp
    repositories/MemberRepository.h
    repositories/MemberRepository.cpp
    repositories/BookRepository.h
    repositories/BookRepository.cpp
    repositories/PurchaseRepository.h
    repositories/PurchaseRepository.cpp
    repositories/CartRepository.h
    repositories/CartRepository.cpp
    repositories/SavedBookRepository.h
    repositories/SavedBookRepository.cpp
    repositories/ShelfRepository.h
    repositories/ShelfRepository.cpp
    repositories/NotificationRepository.h
    repositories/NotificationRepository.cpp
    managers/AuthManager.h
    managers/AuthManager.cpp
    managers/BookManager.h
    managers/BookManager.cpp
    managers/NotificationManager.h
    managers/NotificationManager.cpp
    managers/UserManager.h
    managers/UserManager.cpp
    managers/PurchaseManager.h
    managers/PurchaseManager.cpp
    managers/FileManager.h
    managers/FileManager.cpp
    network/ClientHandler.h
    network/ClientHandler.cpp
    network/Server.h
    network/Server.cpp
    network/handlers/BaseHandler.h
    network/handlers/BaseHandler.cpp
    network/handlers/AuthHandler.h
    network/handlers/AuthHandler.cpp
    network/handlers/BookHandler.h
    network/handlers/BookHandler.cpp
    network/handlers/CartHandler.h
    network/handlers/CartHandler.cpp
    network/handlers/LibraryHandler.h
    network/handlers/LibraryHandler.cpp
    network/handlers/ProfileHandler.h
    network/handlers/ProfileHandler.cpp
    network/handlers/AdminHandler.h
    network/handlers/AdminHandler.cpp
    network/handlers/NotificationHandler.h
    network/handlers/NotificationHandler.cpp
    ui/ServerWindow.h
    ui/ServerWindow.cpp
    ui/ServerWindow.ui
)

target_link_libraries(BookClubServer PRIVATE
    common
    Qt6::Core
    Qt6::Network
    Qt6::Sql
    Qt6::Widgets
)

include(GNUInstallDirs)

# Installed into its own top-level "Server" folder (rather than sharing a
# bin/ with Client) so that its NSIS Start Menu shortcut's default working
# directory is this folder - BookClubServer resolves its SQLite database
# and uploaded book/cover files under a "data/" directory relative to the
# working directory it's launched from.
install(TARGETS BookClubServer
    BUNDLE DESTINATION .
    RUNTIME DESTINATION Server
    LIBRARY DESTINATION Server
)

qt_generate_deploy_app_script(
    TARGET BookClubServer
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)

install(SCRIPT ${deploy_script})