#pragma once

#include <QWidget>
#include <QObject>
#include <QUrl>


namespace Ui {
    class EssaysGeneralPage;
}


class EssaysGeneralPage : public QWidget
{
    Q_OBJECT
public:
    explicit EssaysGeneralPage(QWidget *parent = nullptr);
private:
    Ui::EssaysGeneralPage *ui;
};

