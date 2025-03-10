#pragma once


#include <KXmlGuiWindow>
#include <KRecentFilesAction>
#include "essayeditor.h"
#include <map>
#include "essaysconfig.h"

class KJob;

namespace Ui {
    class MainWindow;
}


class MainWindow : public KXmlGuiWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    void openFileFromUrl(const QUrl &inputFileName);
private:
    Ui::MainWindow *ui;
    int m_CurrentIndex;
    std::map<QString, EssayEditor*> m_files;
    QString fileName;
    KRecentFilesAction* recentsAction;
    EssaysConfig* settings;
private:
    void saveFileToDisk(const QString &outputFileName);
    void setupActions();
private Q_SLOTS:
    void openFile();
    void saveFile();
    void saveFileAs();
    void redoEdit();
    void undoEdit();
    void cutEditor();
    void pasteEditor();
    void copyEditor();
    void showSettings();
    void loadSettings(const QString &dialogName);
    void printCurrentDocument();
    void downloadFinished(KJob *job);
private:
    void setupNewDoc();
    void setTabIndex(int index);
    void destroyEditor(int index);
    void setTabTitle(const QString& title, int index);

};

