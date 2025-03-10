#include <QApplication>
#include <QAction>
#include <KLocalizedString>
#include <KActionCollection>
#include <string>
#include <QPrinter>
#include <QPrintDialog>

#include "essayeditor.h"
#include "ui_essayeditor.h"

EssayEditor::EssayEditor(QWidget *parent) : QWidget(parent), ui(new Ui::EssayEditor)
{
    ui->setupUi(this);
    {
       ui->editorBox->setFont(ui->fontBox->font());
       ui->editorBox->setCheckSpellingEnabled(true);
    }
    {

        connect(ui->boldButton, &QToolButton::toggled, ui->editorBox, &KRichTextEdit::setTextBold);
        connect(ui->italicButton, &QToolButton::toggled, ui->editorBox, &KRichTextEdit::setTextItalic);
        connect(ui->underLineButton, &QToolButton::toggled, ui->editorBox, &KRichTextEdit::setTextUnderline);
        connect(ui->styleBox, &QComboBox::activated, this, &EssayEditor::setHeadingStyle);
        connect(ui->fontBox, &KFontRequester::fontSelected, ui->editorBox, &KRichTextEdit::setFont);
        connect(ui->textColor, &KColorButton::changed, ui->editorBox, &KRichTextEdit::setTextForegroundColor);
        connect(ui->bgColor, &KColorButton::changed, ui->editorBox, &KRichTextEdit::setTextBackgroundColor);
        connect(ui->strikeoutButton, &QToolButton::toggled, ui->editorBox, &KRichTextEdit::setTextStrikeOut);
        connect(ui->subScriptButton, &QToolButton::toggled, ui->editorBox, &KRichTextEdit::setTextSubScript);
        connect(ui->superScriptButton, &QToolButton::toggled, ui->editorBox, &KRichTextEdit::setTextSuperScript);
        connect(ui->alignLeftButton, &QToolButton::toggled, this, &EssayEditor::leftAlign);
        connect(ui->alignCenterButton, &QToolButton::toggled, this, &EssayEditor::centerAlign);
        connect(ui->alignRightButton, &QToolButton::toggled, this, &EssayEditor::rightAlign);
        connect(ui->alignJustifyButton, &QToolButton::toggled, this, &EssayEditor::fillAlign);
        connect(ui->documentTitleText, &QLineEdit::textChanged, this, &EssayEditor::setTitle);
        connect(ui->spacingBox, &QComboBox::currentIndexChanged, this, &EssayEditor::setSpacing);
    }
}

void EssayEditor::leftAlign(bool checked){
    if(checked){
        ui->alignCenterButton->setChecked(false);
        ui->alignRightButton->setChecked(false);
        ui->alignJustifyButton->setChecked(false);
        ui->editorBox->alignLeft();
    }
}

QString EssayEditor::toCleanHtml(){
    return ui->editorBox->textOrHtml();
}

bool EssayEditor::hasTitle(){
    return !ui->editorBox->documentTitle().isEmpty();
}

QString EssayEditor::getTitle(){
    return ui->editorBox->documentTitle();
}

void EssayEditor::setIndex(int newIndex){
    index = newIndex;
}

void EssayEditor::setUrl(const QUrl& url){
    fileUrl = url;
}

int EssayEditor::getIndex(){
    return index;
}


void EssayEditor::setTitle(const QString& title){
    ui->editorBox->setDocumentTitle(title);
    Q_EMIT titleChanged(hasTitle() ? title : fileUrl.fileName(), index);
}

void EssayEditor::setTitleUI(const QString& title){
    ui->documentTitleText->setText(title);
}

void EssayEditor::setSpacing(int index){
    switch(index){
        case 0:
           changeSpacing(100);
           break;
        case 1:
            changeSpacing(150);
            break;
        case 2:
            changeSpacing(200);
            break;
        default:
            changeSpacing(100 * index);

    }


}

void EssayEditor::printDoc(){
    QPrinter printer;
    QPrintDialog dialog(&printer, this);
    dialog.setWindowTitle(i18n("Print Selected Document"));
    if (ui->editorBox->textCursor().hasSelection())
        dialog.setOption(QAbstractPrintDialog::PrintSelection, true);
    if (dialog.exec() != QDialog::Accepted)
        return;
    ui->editorBox->print(&printer);
}

void EssayEditor::setSpacingStr(const QString& spacing){
    ui->spacingBox->setCurrentText(spacing);
    setSpacing(ui->spacingBox->findText(spacing));
}

void EssayEditor::changeSpacing(int space){
    qreal qSpacing = qreal(space);
    QTextBlockFormat* newFormat = new QTextBlockFormat();
    newFormat->setLineHeight(qSpacing, QTextBlockFormat::ProportionalHeight);
    QTextCursor textCursor = ui->editorBox->textCursor();
    textCursor.clearSelection();
    textCursor.select(QTextCursor::Document);
    textCursor.setBlockFormat(*newFormat);


}

void EssayEditor::setText(const QString& html){
    ui->editorBox->setTextOrHtml(html);
}

void EssayEditor::redo(){
    ui->editorBox->redo();
}

void EssayEditor::copy(){
    ui->editorBox->copy();
}

void EssayEditor::cut(){
    ui->editorBox->cut();
}

void EssayEditor::paste(){
    ui->editorBox->paste();
}


void EssayEditor::undo(){
    ui->editorBox->undo();
}

void EssayEditor::centerAlign(bool checked){
    if(checked){
        ui->alignLeftButton->setChecked(false);
        ui->alignRightButton->setChecked(false);
        ui->alignJustifyButton->setChecked(false);
        ui->editorBox->alignCenter();
    }
}

void EssayEditor::rightAlign(bool checked){
    if(checked){
        ui->alignCenterButton->setChecked(false);
        ui->alignLeftButton->setChecked(false);
        ui->alignJustifyButton->setChecked(false);
        ui->editorBox->alignRight();
    }
}


void EssayEditor::fillAlign(bool checked){
    if(checked){
        ui->alignLeftButton->setChecked(false);
        ui->alignCenterButton->setChecked(false);
        ui->alignRightButton->setChecked(false);
        ui->editorBox->alignJustify();
    }
}


void EssayEditor::setHeadingStyle(int index){

    ui->editorBox->setHeadingLevel(index);

}
