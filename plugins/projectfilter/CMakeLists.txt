add_definitions(-DTRANSLATION_DOMAIN=\"kdevprojectfilter\")
##############################
# PLUGIN #####################
##############################

set( projectfilter_SRCS
    projectfilterprovider.cpp
    projectfilter.cpp
    filter.cpp
    projectfilterconfigpage.cpp
    filter.cpp
    filtermodel.cpp
    comboboxdelegate.cpp
)

ecm_qt_declare_logging_category(projectfilter_SRCS
    HEADER debug.h
    IDENTIFIER PLUGIN_PROJECTFILTER
    CATEGORY_NAME "kdevelop.plugins.projectfilter"
)
ki18n_wrap_ui(projectfilter_SRCS projectfiltersettings.ui)
kconfig_add_kcfg_files(projectfilter_SRCS projectfiltersettings.kcfgc)

kdevplatform_add_plugin(kdevprojectfilter JSON kdevprojectfilter.json SOURCES ${projectfilter_SRCS})

target_link_libraries(kdevprojectfilter
    KDev::Project
    KDev::Util
    KDev::Interfaces
)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
