cmake_minimum_required(VERSION 3.16)
project(Book_Club)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Network Sql)

add_subdirectory(Common)
add_subdirectory(Server)
add_subdirectory(Client)

# ---------------------------------------------------------------------------
# Packaging: `cpack` or `cmake --build . --target package` produces a single
# installer bundling both executables (Client and BookClubServer) so the
# apps can be run on a machine without Qt installed.
#
# Client/CMakeLists.txt and Server/CMakeLists.txt install into their own
# top-level "Client" / "Server" folders rather than a shared bin/,
# specifically so each app's Start Menu shortcut (below) can default its
# working directory to that app's own folder. This matters for
# BookClubServer: its SQLite database and uploaded book/cover files are
# resolved from a "data/" directory relative to the working directory it's
# launched from.
# ---------------------------------------------------------------------------
set(CPACK_PACKAGE_NAME "BookClub")
set(CPACK_PACKAGE_VENDOR "BookClub")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "BookClub")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "BookClub e-book store client and server")

if(WIN32)
    set(CPACK_GENERATOR "NSIS")
    set(CPACK_NSIS_PACKAGE_NAME "BookClub")
    set(CPACK_NSIS_DISPLAY_NAME "BookClub")
    set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
    set(CPACK_NSIS_MODIFY_PATH OFF)

    # We don't use CPACK_PACKAGE_EXECUTABLES for the shortcuts: those are
    # always created with $INSTDIR as the working directory. Instead, add
    # our own CreateShortCut commands, each preceded by SetOutPath so the
    # Client shortcut starts in Client/ and the Server shortcut starts in
    # Server/.
    set(CPACK_NSIS_CREATE_ICONS_EXTRA "
  SetOutPath \"$INSTDIR\\Client\"
  CreateShortCut \"$SMPROGRAMS\\$STARTMENU_FOLDER\\BookClub Client.lnk\" \"$INSTDIR\\Client\\Client.exe\"
  SetOutPath \"$INSTDIR\\Server\"
  CreateShortCut \"$SMPROGRAMS\\$STARTMENU_FOLDER\\BookClub Server.lnk\" \"$INSTDIR\\Server\\BookClubServer.exe\"
")
    set(CPACK_NSIS_DELETE_ICONS_EXTRA "
  Delete \"$SMPROGRAMS\\$STARTMENU_FOLDER\\BookClub Client.lnk\"
  Delete \"$SMPROGRAMS\\$STARTMENU_FOLDER\\BookClub Server.lnk\"
")
endif()

include(CPack)