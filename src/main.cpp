#include <QApplication>
#include <QCommandLineParser>
#include <KAboutData>
#include <KLocalizedString>
#include <QDir>
#include <QUrl>
#include "mainwindow.h"


int main (int argc, char *argv[])
{
    using namespace Qt::Literals::StringLiterals;

    QApplication app(argc, argv);
    QApplication::setDesktopFileName(QStringLiteral("io.github.tridentu.essays"));

    KLocalizedString::setApplicationDomain("tridentu-essays");

    KAboutData aboutData(
        u"tridentu-essays"_s,
        i18n("Essays"),
                         u"1.0"_s,
                         i18n("Tridentu Essays Word Processor"),
                         KAboutLicense::GPL,
                         i18n("(c) 2025"),
                         i18n("Designed mainly for writing school documents such as essays."),
                         u"https://tridentu.github.io/essays-program/"_s);


    aboutData.setDesktopFileName(QStringLiteral("io.github.tridentu.essays"));
    KAboutData::setApplicationData(aboutData);

    QCommandLineParser parser;
    aboutData.setupCommandLine(&parser);

    parser.addPositionalArgument(u"file"_s, i18n("Document to open"));


    parser.process(app);
    aboutData.processCommandLine(&parser);


    MainWindow *window = new MainWindow();



    window->show();


    if (parser.positionalArguments().count() > 0) {
        window->openFileFromUrl(QUrl::fromUserInput(
            parser.positionalArguments().at(0),
                                                    QDir::currentPath()));
    }



    return app.exec();

}
