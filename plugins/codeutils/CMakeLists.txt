add_definitions(-DTRANSLATION_DOMAIN=\"kdevcodeutils\")

########### install target ###############

set(kdevcodeutils_PART_SRCS
    codeutilsplugin.cpp
)

ecm_qt_declare_logging_category(kdevcodeutils_PART_SRCS
    HEADER debug.h
    IDENTIFIER PLUGIN_CODEUTILS
    CATEGORY_NAME "kdevelop.plugins.codeutils"
)
qt5_add_resources(kdevcodeutils_PART_SRCS kdevcodeutils.qrc)
kdevplatform_add_plugin(kdevcodeutils JSON kdevcodeutils.json SOURCES ${kdevcodeutils_PART_SRCS})

target_link_libraries(kdevcodeutils
    KF5::Parts
    KF5::TextEditor
    KDev::Interfaces
    KDev::Util
    KDev::Language
)

add_subdirectory(doc_templates)
