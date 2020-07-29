#include "MakerCreator.h"
#include "ui_MakerCreator.h"

MakerCreator::MakerCreator(QWidget *parent) : QDialog(parent), ui(new Ui::MakerCreator)
{
    ui->setupUi(this);
}

MakerCreator::MakerCreator(Maker maker, QWidget *parent) : QDialog(parent), ui(new Ui::MakerCreator)
{
    ui->setupUi(this);

    this->setWindowTitle("Modification");

    ui->ln_UserName->setText(maker.UserName);
    ui->ln_LastName->setText(maker.UserName);
    ui->ln_Mail->setText(maker.UserName);
    ui->ln_Phone->setText(maker.UserName);
    ui->chk_PriceCat->setDown(maker.PriceCat);
}


MakerCreator::~MakerCreator()
{
    delete ui;
}

void MakerCreator::accept()
{
    if(ui->ln_UserName->text().isEmpty())
        ui->ln_UserName->setFocus(Qt::FocusReason::PopupFocusReason);
    else if(ui->ln_LastName->text().isEmpty())
        ui->ln_LastName->setFocus(Qt::FocusReason::PopupFocusReason);
    else if(ui->ln_Mail->text().isEmpty())
        ui->ln_Mail->setFocus(Qt::FocusReason::PopupFocusReason);
    else if(ui->ln_Phone->text().isEmpty())
        ui->ln_Phone->setFocus(Qt::FocusReason::PopupFocusReason);
    else
        QDialog::accept();
}

Maker MakerCreator::create(QWidget *parent)
{
    MakerCreator creator(parent);

    if(creator.exec() == QDialog::Accepted)
        return creator.getMaker();
    else
        return Maker();
}

void MakerCreator::modify(Maker &maker, QWidget *parent)
{
    MakerCreator creator(maker, parent);

    if(creator.exec() == QDialog::Accepted)
    {
        maker = creator.getMaker();
        return;
    }

    else
        return;
}


Maker MakerCreator::getMaker()
{
    Maker maker;
    maker.UserName = ui->ln_UserName->text();
    maker.LastName = ui->ln_LastName->text();
    maker.Mail = ui->ln_Mail->text();
    maker.Phone = ui->ln_Phone->text();
    maker.PriceCat = ui->chk_PriceCat->isChecked();

    return maker;
}
