set(screencast_SOURCES
    eglnativefence.cpp
    main.cpp
    pipewirecore.cpp
    pipewirestream.cpp
    screencastmanager.cpp
)

ecm_qt_declare_logging_category(screencast_SOURCES
    HEADER kwinscreencast_logging.h
    IDENTIFIER KWIN_SCREENCAST
    CATEGORY_NAME kwin_screencast
    DEFAULT_SEVERITY Warning
)

add_library(screencast MODULE ${screencast_SOURCES})
set_target_properties(screencast PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/kwin/plugins/")
target_link_libraries(screencast kwin PkgConfig::PipeWire)
install(TARGETS screencast DESTINATION ${PLUGIN_INSTALL_DIR}/kwin/plugins/)
