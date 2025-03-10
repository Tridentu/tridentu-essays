#pragma once

#include <QWidget>
#include <QObject>
#include <QUrl>


namespace Ui {
    class EssayEditor;
}


class EssayEditor : public QWidget
{
    Q_OBJECT
public:
    explicit EssayEditor(QWidget *parent = nullptr);

private:
    Ui::EssayEditor *ui;
    int index;
    QUrl fileUrl;
private:
    void setHeadingStyle(int selection);
    void leftAlign(bool checked);
    void centerAlign(bool checked);
    void rightAlign(bool checked);
    void fillAlign(bool checked);
    void changeSpacing(int spacing);
public:
    QString toCleanHtml();
    void setText(const QString& html);
    void redo();
    void undo();
    void cut();
    void paste();
    void copy();
    void setSpacing(int index);
    void setSpacingStr(const QString& spacing);
    void printDoc();

    QString getTitle();
    bool hasTitle();
    void setIndex(int newIndex);

    int getIndex();
    void setUrl(const QUrl& url);

    void setTitle(const QString& title);
    void setTitleUI(const QString& title);
Q_SIGNALS:
    void titleChanged(const QString& title, int index);
};

