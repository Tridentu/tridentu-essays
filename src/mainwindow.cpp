#include <QApplication>
#include <QAction>
#include <KLocalizedString>
#include <KActionCollection>
#include <KStandardAction>
#include <QSaveFile>
#include <QFileDialog>
#include <QTextStream>
#include <QByteArray>
#include <KMessageBox>
#include <KF6/KIOCore/KIO/StoredTransferJob>
#include "mainwindow.h"
#include "essayeditor.h"
#include <KConfigDialog>
#include "essaysgeneralpage.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : KXmlGuiWindow(parent), ui(new Ui::MainWindow), fileName(QString())
{
    ui->setupUi(this);
    {
        connect(ui->docTabs, &QTabWidget::currentChanged, this, &MainWindow::setTabIndex);
        connect(ui->docTabs, &QTabWidget::tabCloseRequested, this, &MainWindow::destroyEditor);
        connect(ui->newCommand, &QAbstractButton::clicked, this, &MainWindow::setupNewDoc);
        connect(ui->openCommand, &QAbstractButton::clicked, this, &MainWindow::openFile);
    }
    setupActions();

}

void MainWindow::setupNewDoc(){
    EssayEditor* editor = new EssayEditor();

    int index = ui->docTabs->addTab(editor, QIcon::fromTheme(QString::fromStdString("application-msword")), QString::fromStdString("New Document"));
    editor->setIndex(index);
    m_files[QString::number(index)] = editor;
    connect(editor, &EssayEditor::titleChanged, this, &MainWindow::setTabTitle);
    ui->docTabs->setCurrentIndex(index);

    editor->setSpacingStr(settings->defaultSpacing());
}

void MainWindow::setTabTitle(const QString& title, int index){
    for(auto const& [key, editor] : m_files){
        if (key.toInt() == index){
            ui->docTabs->setTabText(index,title);
        }
    }
}

void MainWindow::destroyEditor(int index){
    if (m_files[QString::number(index)]){
        ui->docTabs->removeTab(index);
        delete m_files[QString::number(index)];
        m_files[QString::number(index)] = nullptr;
    } else if (index == 0){
        qApp->quit();
    }
}

void MainWindow::printCurrentDocument(){
    if(m_files[QString::number(m_CurrentIndex)]){
        EssayEditor* editor = m_files[QString::number(m_CurrentIndex)];
        editor->printDoc();
    }
}


void MainWindow::saveFileToDisk(const QString &outputFileName)
{
    if (!outputFileName.isNull() && m_CurrentIndex > 0) {
        QSaveFile file(outputFileName);
        file.open(QIODevice::WriteOnly);

        EssayEditor* editor = m_files[QString::number(m_CurrentIndex)];

        QByteArray outputByteArray;
        outputByteArray.append(editor->toCleanHtml().toUtf8());

        file.write(outputByteArray);
        file.commit();

        fileName = outputFileName;
        QUrl url(fileName);
        ui->docTabs->setTabText(m_CurrentIndex, editor->hasTitle() ? editor->getTitle() : url.fileName());
        editor->setUrl(url);
    }
}

void MainWindow::saveFileAs()
{
    saveFileToDisk(QFileDialog::getSaveFileName(this, i18n("Save Document As"), QString::fromStdString("document.html"), i18n("HTML Documents (*.html)")));
}

void MainWindow::saveFile()
{
    if (!fileName.isEmpty()) {
        saveFileToDisk(fileName);
    } else {
        saveFileAs();
    }
}

void MainWindow::openFile()
{
    openFileFromUrl(QFileDialog::getOpenFileUrl(this, i18n("Open Document"), QUrl(QDir::homePath()), i18n("HTML Documents (*.html)")));

}

void MainWindow::openFileFromUrl(const QUrl &inputFileName)
{
    if (!inputFileName.isEmpty()) {
        KIO::Job *job = KIO::storedGet(inputFileName);
        fileName = inputFileName.toLocalFile();
        connect(job, &KJob::result, this, &MainWindow::downloadFinished);
        job->exec();
    }
}

void MainWindow::showSettings(){
    if (KConfigDialog::showDialog(QStringLiteral("settings"))) {
        return;
    }
    KConfigDialog *dialog = new KConfigDialog(this, QStringLiteral("settings"), settings);
    dialog->setFaceType(KPageDialog::List);
    dialog->addPage(new EssaysGeneralPage(), i18n("Document Settings") , QStringLiteral("settings"));
    connect(dialog, &KConfigDialog::settingsChanged, this, &MainWindow::loadSettings);
    dialog->show();
}

void MainWindow::loadSettings(const QString &dialogName)
{
    if(dialogName == QStringLiteral("settings")){
        settings->save();
    }
}

void MainWindow::downloadFinished(KJob *job)
{
    if (job->error()) {
        KMessageBox::error(this, job->errorString());
        fileName.clear();
        return;
    }

    const KIO::StoredTransferJob *storedJob = qobject_cast<KIO::StoredTransferJob *>(job);

    if (storedJob) {
        EssayEditor* editor = new EssayEditor();

        int index = ui->docTabs->addTab(editor, QIcon::fromTheme(QString::fromStdString("application-msword")), QString::fromStdString("New Document"));
        editor->setIndex(index);
        m_files[QString::number(index)] = editor;
        editor->setText(QTextStream(storedJob->data(), QIODevice::ReadOnly).readAll());

        QUrl url(QStringLiteral("file://") + fileName);
        editor->setUrl(url);
        setTabTitle( editor->hasTitle() ? editor->getTitle() : url.fileName(), index);
        editor->setTitleUI(editor->hasTitle() ? editor->getTitle() : url.fileName());
        recentsAction->addUrl(url, editor->getTitle());

        connect(editor, &EssayEditor::titleChanged, this, &MainWindow::setTabTitle);
    }
}

void MainWindow::setTabIndex(int index){
    m_CurrentIndex = index;
}

void MainWindow::redoEdit(){
    if (m_CurrentIndex > 0){
        m_files[QString::number(m_CurrentIndex)]->redo();
    }
}


void MainWindow::pasteEditor(){
    if (m_CurrentIndex > 0){
        m_files[QString::number(m_CurrentIndex)]->paste();
    }
}

void MainWindow::copyEditor(){
    if (m_CurrentIndex > 0){
        m_files[QString::number(m_CurrentIndex)]->copy();
    }
}

void MainWindow::cutEditor(){
    if (m_CurrentIndex > 0){
        m_files[QString::number(m_CurrentIndex)]->cut();
    }
}

void MainWindow::undoEdit(){
    if (m_CurrentIndex > 0){
        m_files[QString::number(m_CurrentIndex)]->undo();
    }
}

void MainWindow::setupActions(){
    using namespace Qt::Literals::StringLiterals;

    KStandardAction::openNew(this, &MainWindow::setupNewDoc, actionCollection());
    KStandardAction::open(this, &MainWindow::openFile, actionCollection());
    KStandardAction::save(this, &MainWindow::saveFile, actionCollection());
    KStandardAction::saveAs(this, &MainWindow::saveFileAs, actionCollection());
    KStandardAction::redo(this, &MainWindow::redoEdit, actionCollection());
    KStandardAction::undo(this, &MainWindow::undoEdit, actionCollection());
    KStandardAction::cut(this, &MainWindow::cutEditor, actionCollection());
    KStandardAction::paste(this, &MainWindow::pasteEditor, actionCollection());
    KStandardAction::copy(this, &MainWindow::copyEditor, actionCollection());
    KStandardAction::quit(qApp, &QCoreApplication::quit, actionCollection());
    KStandardAction::preferences(this, &MainWindow::showSettings, actionCollection());
     KStandardAction::print(this, &MainWindow::printCurrentDocument, actionCollection());
    recentsAction = KStandardAction::openRecent(this, &MainWindow::openFileFromUrl, actionCollection());
    settings = EssaysConfig::self();
    setupGUI(Default, u"essays.rc"_s);
}
