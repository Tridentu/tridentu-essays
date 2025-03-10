#include <QApplication>
#include <QAction>
#include <KLocalizedString>
#include <KActionCollection>
#include <string>
#include "essaysconfig.h"

#include "essaysgeneralpage.h"
#include "ui_essaysgeneralpage.h"

EssaysGeneralPage::EssaysGeneralPage(QWidget *parent) : QWidget(parent), ui(new Ui::EssaysGeneralPage)
{
    ui->setupUi(this);

}
