cmake_minimum_required(VERSION 3.20)
project(tridentu-essays)
set(QT_MIN_VERSION "6.6.0")
set(KF_MIN_VERSION "6.0.0")

find_package(ECM ${KF_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(FeatureSummary)

find_package(Qt6 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS
    Core    # QCommandLineParser, QStringLiteral
    Widgets # QApplication
    PrintSupport
)

find_package(KF6 ${KF_MIN_VERSION} REQUIRED COMPONENTS
    CoreAddons      # KAboutData
    I18n            # KLocalizedString
    XmlGui          # KXmlGuiWindow
    TextWidgets     # KTextEdit
    ConfigWidgets   # KStandardActions
    Completion
    WidgetsAddons   # KMessageBox
    KIO             # KIO
)

add_executable(tridentu-essays)

target_sources(tridentu-essays
    PRIVATE
    src/main.cpp
    src/essayeditor.ui
    src/essayeditor.cpp
    src/mainwindow.ui
    src/mainwindow.cpp
    src/essaysgeneralpage.cpp
    src/essaysgeneralpage.ui

)
kconfig_add_kcfg_files(tridentu-essays essaysconfig.kcfgc)

target_link_libraries(tridentu-essays
    Qt6::Widgets
    Qt6::PrintSupport
    KF6::CoreAddons
    KF6::I18n
    KF6::XmlGui
    KF6::TextWidgets
    KF6::ConfigWidgets
    KF6::Completion
      KF6::WidgetsAddons
    KF6::KIOCore
)

install(TARGETS tridentu-essays ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES essays.rc DESTINATION ${KDE_INSTALL_KXMLGUIDIR}/tridentu-essays)
install(FILES io.github.tridentu.essays.desktop DESTINATION ${KDE_INSTALL_APPDIR})

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
