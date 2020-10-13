cmake_minimum_required(VERSION 3.10)

project(ListMaker VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

link_libraries(
    gtkmm-3.0 atkmm-1.6 gdkmm-3.0 giomm-2.4 pangomm-1.4 glibmm-2.4
    gtk-3 gdk-3 pangocairo-1.0 pango-1.0 harfbuzz atk-1.0
    cairo-gobject gio-2.0 cairomm-1.0 cairo sigc-2.0 gdk_pixbuf-2.0 gobject-2.0 glib-2.0)

include_directories(
    /usr/include/gtkmm-3.0 /usr/lib/x86_64-linux-gnu/gtkmm-3.0/include /usr/include/atkmm-1.6
    /usr/include/gtk-3.0/unix-print /usr/include/gdkmm-3.0
    /usr/lib/x86_64-linux-gnu/gdkmm-3.0/include /usr/include/giomm-2.4
    /usr/lib/x86_64-linux-gnu/giomm-2.4/include /usr/include/pangomm-1.4
    /usr/lib/x86_64-linux-gnu/pangomm-1.4/include /usr/include/glibmm-2.4
    /usr/lib/x86_64-linux-gnu/glibmm-2.4/include /usr/include/gtk-3.0
    /usr/include/at-spi2-atk/2.0 /usr/include/at-spi-2.0
    /usr/include/dbus-1.0 /usr/lib/x86_64-linux-gnu/dbus-1.0/include
    /usr/include/gtk-3.0 /usr/include/gio-unix-2.0 /usr/include/cairo
    /usr/include/pango-1.0 /usr/include/fribidi /usr/include/harfbuzz /usr/include/atk-1.0
    /usr/include/cairo /usr/include/cairomm-1.0 /usr/lib/x86_64-linux-gnu/cairomm-1.0/include
    /usr/include/cairo /usr/include/pixman-1 /usr/include/uuid /usr/include/freetype2
    /usr/include/libpng16 /usr/include/sigc++-2.0 /usr/lib/x86_64-linux-gnu/sigc++-2.0/include
    /usr/include/gdk-pixbuf-2.0 /usr/include/libmount /usr/include/blkid /usr/include/glib-2.0
    /usr/lib/x86_64-linux-gnu/glib-2.0/include
)

set(CMAKE_CXX_FLAGS "-pthread -g3")

if (EXISTS ${CMAKE_SOURCE_DIR}/.git)
    execute_process(COMMAND git rev-parse HEAD OUTPUT_VARIABLE GIT_COMMIT OUTPUT_STRIP_TRAILING_WHITESPACE)
    string(PREPEND GIT_COMMIT \")
    string(APPEND GIT_COMMIT \")
    add_compile_definitions(GIT_COMMIT=${GIT_COMMIT})
    message("Git commit hash: " ${GIT_COMMIT})
else()
    message("Not a git repo")
endif()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/img/icon.svg ${CMAKE_CURRENT_BINARY_DIR}/img/icon.svg COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/img/colors.svg ${CMAKE_CURRENT_BINARY_DIR}/img/colors.svg COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/img/cross.svg ${CMAKE_CURRENT_BINARY_DIR}/img/cross.svg COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/img/floppy.svg ${CMAKE_CURRENT_BINARY_DIR}/img/floppy.svg COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/img/pencil.svg ${CMAKE_CURRENT_BINARY_DIR}/img/pencil.svg COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/img/plus.svg ${CMAKE_CURRENT_BINARY_DIR}/img/plus.svg COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/img/tick.svg ${CMAKE_CURRENT_BINARY_DIR}/img/tick.svg COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/img/info.svg ${CMAKE_CURRENT_BINARY_DIR}/img/info.svg COPYONLY)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/boards.lmf ${CMAKE_CURRENT_BINARY_DIR}/boards.lmf)

# tools/gen-src-list.py
add_executable(list-maker
    src/AboutDialog.cpp
    src/AboutDialog.h
    src/AskRGB.cpp
    src/AskRGB.h
    src/Board.cpp
    src/Board.h
    src/BoardSelectorWidget.cpp
    src/BoardSelectorWidget.h
    src/BoardViewerWidget.cpp
    src/BoardViewerWidget.h
    src/FileLoader.cpp
    src/FileLoader.h
    src/List.cpp
    src/List.h
    src/ListItem.cpp
    src/ListItem.h
    src/MainWindow.cpp
    src/MainWindow.h
    src/StringAskerDialog.cpp
    src/StringAskerDialog.h
    src/YesNoAskerDialog.cpp
    src/YesNoAskerDialog.h
    src/icon_paths.h
    src/main.cpp
    src/version.h
)
