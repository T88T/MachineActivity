#include "PreferencesWindow.h"
#include "ui_PreferencesWindow.h"


Settings::Settings()
{
    mAlternateRowColors = false;

    mPosColor = Qt::white;
    mNegColor = Qt::white;
    mZerColor = Qt::white;

    mBackgroundColor = Qt::white;

    mRowAColor = Qt::white;
    mRowBColor = Qt::white;

    mHasChanged = true;
}

Settings::~Settings()
{

}

void Settings::setDB_Path(QString p)
{
    mHasChanged = true;
    mDB_Path = p;
}
void Settings::setPosColor(QColor p)
{
    mHasChanged = true;
    mPosColor = p;
}
void Settings::setZerColor(QColor p)
{
    mHasChanged = true;
    mZerColor = p;
}
void Settings::setNegColor(QColor p)
{
    mHasChanged = true;
    mNegColor = p;
}
void Settings::setAlternateRowColor(bool p)
{
    mHasChanged = true;
    mAlternateRowColors = p;
}
void Settings::setRowAColor(QColor p)
{
    mHasChanged = true;
    mRowAColor = p;
}
void Settings::setRowBColor(QColor p)
{
    mHasChanged = true;
    mRowBColor = p;
}
void Settings::setBackColor(QColor p)
{
    mHasChanged = true;
    mBackgroundColor = p;
}

const QString Settings::DB_Path() const {return mDB_Path;}
const QColor Settings::posColor() const {return mPosColor;}
const QColor Settings::zerColor() const {return mZerColor;}
const QColor Settings::negColor() const {return mNegColor;}
bool   Settings::alternateRowColor() const {return mAlternateRowColors;}
const QColor Settings::rowAColor() const {return mRowAColor;}
const QColor Settings::rowBColor() const {return mRowBColor;}
const QColor Settings::backColor() const {return mBackgroundColor;}

bool Settings::edited()
{
    bool temp = mHasChanged;
    mHasChanged = false;
    return temp;

}

PreferencesWindow::PreferencesWindow(QWidget *parent, Settings *set) : QDialog(parent), ui(new Ui::PreferencesWindow)
{
    ui->setupUi(this);

    if(set == nullptr)
    {
        this->close();
        return;
    }

    ColorPicker = new QColorDialog(this);
    connect(ColorPicker, &QColorDialog::currentColorChanged, this, &PreferencesWindow::colorDialogOk);

    currentEditedLabel = nullptr;
    mSettings = set;

    ui->clr_neg->setAutoFillBackground(true);
    ui->clr_pos->setAutoFillBackground(true);
    ui->clr_zer->setAutoFillBackground(true);
    ui->clr_background->setAutoFillBackground(true);
    ui->clr_RowA->setAutoFillBackground(true);
    ui->clr_RowB->setAutoFillBackground(true);

    QPalette Palette;

    ui->ln_Path->setText(mSettings->DB_Path());

    Palette.setColor(QPalette::Window, mSettings->negColor());
    ui->clr_neg->setPalette(Palette);
    Palette.setColor(QPalette::Window, mSettings->posColor());
    ui->clr_pos->setPalette(Palette);
    Palette.setColor(QPalette::Window, mSettings->zerColor());
    ui->clr_zer->setPalette(Palette);
    Palette.setColor(QPalette::Window, mSettings->backColor());
    ui->clr_background->setPalette(Palette);
    Palette.setColor(QPalette::Window, mSettings->rowAColor());
    ui->clr_RowA->setPalette(Palette);
    Palette.setColor(QPalette::Window, mSettings->rowBColor());
    ui->clr_RowB->setPalette(Palette);

    ui->chk_RowAlternate->setChecked(mSettings->alternateRowColor());

    connect(ui->clr_neg, &ClickableLabel::clicked, this, &PreferencesWindow::colorLabelClicked);
    connect(ui->clr_pos, &ClickableLabel::clicked, this, &PreferencesWindow::colorLabelClicked);
    connect(ui->clr_zer, &ClickableLabel::clicked, this, &PreferencesWindow::colorLabelClicked);
    connect(ui->clr_background, &ClickableLabel::clicked, this, &PreferencesWindow::colorLabelClicked);
    connect(ui->clr_RowA, &ClickableLabel::clicked, this, &PreferencesWindow::colorLabelClicked);
    connect(ui->clr_RowB, &ClickableLabel::clicked, this, &PreferencesWindow::colorLabelClicked);
}

PreferencesWindow::~PreferencesWindow()
{
    emit settingsClosed();
    delete ui;
}

void PreferencesWindow::colorLabelClicked()
{
    currentEditedLabel = dynamic_cast<QLabel*>(QObject::sender());
    ColorPicker->show();
}

void PreferencesWindow::browseForDatabase()
{
    ui->ln_Path->setText(QFileDialog::getOpenFileName(this));
    mSettings->setDB_Path(ui->ln_Path->text());

    //emit settingChanged();
}

void PreferencesWindow::colorDialogOk(QColor newColor)
{
    if(currentEditedLabel != nullptr)
    {
        QPalette Palette;
        Palette.setColor(QPalette::Window, newColor);
        currentEditedLabel->setPalette(Palette);

        if(currentEditedLabel == ui->clr_neg)
            mSettings->setNegColor(newColor);

        if(currentEditedLabel == ui->clr_zer)
            mSettings->setZerColor(newColor);

        if(currentEditedLabel == ui->clr_pos)
            mSettings->setPosColor(newColor);

        if(currentEditedLabel == ui->clr_background)
            mSettings->setBackColor(newColor);

        if(currentEditedLabel == ui->clr_RowA)
            mSettings->setRowAColor(newColor);

        if(currentEditedLabel == ui->clr_RowB)
            mSettings->setRowBColor(newColor);

        emit settingChanged();
    }
}

void PreferencesWindow::alternateRowColor(bool State)
{
    mSettings->setAlternateRowColor(State);
    emit settingChanged();
}
