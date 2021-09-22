#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "iostream"
#include <QDebug>
#include "widgets/MakerCreator.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    isConnected = false;
    UserActivityFiltering = false;
    UserModel = nullptr;
    ActivityModel = nullptr;
    Mapper = nullptr;
    ProdModel = nullptr;
    SettingsWindow = nullptr;
    DB = QSqlDatabase::addDatabase("QSQLITE");

    QLocale::setDefault(QLocale::French);

    connect(ui->act_Settings, &QAction::triggered, this, &MainWindow::openSettings);
    //connect(ui->act_AdvancedModifications, &QAction::toggled, this->ui., &ui::openSettings);

    //Loading preferences from preferences.json and accessing database
    loadSettings();
    applySettings();
}

void MainWindow::selectDatabase(QString path)
{
    isConnected = openDatabase(path);

    //Loading data from database
    if(isConnected)
    {
        DB_Path = path;
        loadFromDatabase();
        ui->menuFichier->setEnabled(true);
        ui->groupBox->setEnabled(true);
        ui->groupBox_2->setEnabled(true);
        ui->groupBox_3->setEnabled(true);
        ui->groupBox_4->setEnabled(true);
        ui->groupBox_5->setEnabled(true);
    }

    else
    {
        ui->menuFichier->setEnabled(true);
        ui->groupBox->setEnabled(false);
        ui->groupBox_2->setEnabled(false);
        ui->groupBox_3->setEnabled(false);
        ui->groupBox_4->setEnabled(false);
        ui->groupBox_5->setEnabled(false);
    }
}

bool MainWindow::openDatabase(QString Path)
{
    QFileInfo file(Path);

    if(!file.isFile() || Path == QString(""))
    {
        QMessageBox::information(this, "Accès à la base de donnée", "Le fichier de la base de données n'éxiste pas. Vérifiez le chemin d'accès dans les préférences.");
        isConnected = false;
        return false;
    }

    DB.setDatabaseName(Path);

    if (!DB.open())
    {
       QMessageBox::information(this, "Accès à la base de donnée", "L'ouverture de la base de données n'as pas fonctionné. Fichier corrompu ?...");
       isConnected = false;
       return false;
    }

    else
    {
       qDebug() << "Ouverture de la base de données : OK";
       isConnected = true;
       return true;
    }
}

void MainWindow::loadFromDatabase()
{
    UserModel = new QSqlTableModel(this, DB);
    UserModel->setTable("Users");
    UserModel->setEditStrategy(QSqlTableModel::OnFieldChange);
    UserModel->setSort(UserTable::LastName, Qt::AscendingOrder);
    UserModel->select();

    Mapper = new QDataWidgetMapper(this);
    Mapper->setModel(UserModel);
    Mapper->addMapping(ui->ln_UserName, UserTable::Name);
    Mapper->addMapping(ui->ln_LastName, UserTable::LastName);
    Mapper->addMapping(ui->ln_Mail, UserTable::Mail);
    Mapper->addMapping(ui->ln_Phone, UserTable::Phone);
    Mapper->addMapping(ui->chk_PriceCat, UserTable::PriceCat);
    Mapper->addMapping(ui->chk_PassI3D, UserTable::Pass_I3D);
    Mapper->addMapping(ui->chk_Las, UserTable::Pass_Las);
    Mapper->addMapping(ui->chk_Plo, UserTable::Pass_Plo);
    Mapper->addMapping(ui->chk_Sho, UserTable::Pass_Sho);
    Mapper->addMapping(ui->chk_Cer, UserTable::Pass_Cer);
    Mapper->addMapping(ui->tm_I3D, UserTable::Counter_I3D);
    Mapper->addMapping(ui->tm_Las, UserTable::Counter_Las);
    Mapper->addMapping(ui->tm_Plo, UserTable::Counter_Plo);
    Mapper->addMapping(ui->tm_Sho, UserTable::Counter_Sho);
    Mapper->addMapping(ui->tm_Cer, UserTable::Counter_Cer);

    connect(ui->cmb_UserName, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), Mapper, &QDataWidgetMapper::setCurrentIndex);
    connect(ui->cmb_UserName->lineEdit(), &QLineEdit::returnPressed, this, &MainWindow::validateCombo);

    ui->cmb_UserName->setModel(UserModel);
    ui->cmb_UserName->setModelColumn(UserTable::LastName);
    UserCompleter = new QCompleter(UserModel, this);
    UserCompleter->setCompletionColumn(UserTable::LastName);
    UserCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    UserCompleter->setCompletionMode(QCompleter::InlineCompletion);
    ui->cmb_UserName->setCompleter(UserCompleter);

    ProdModel = new QSqlTableModel(this, DB);
    ProdModel->setTable("Products");
    ProdModel->setSort(ProdTable::ProdName, Qt::AscendingOrder);
    ProdModel->select();
    ui->cmb_Prod->setModel(ProdModel);
    ui->cmb_Prod->setModelColumn(1);
    ProductCompleter = new QCompleter(ProdModel, this);
    ProductCompleter->setCompletionColumn(ProdTable::ProdName);
    ProductCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    ProductCompleter->setCompletionMode(QCompleter::InlineCompletion);
    ui->cmb_Prod->setCompleter(ProductCompleter);

    ActivityModel = new QSqlRelationalTableModel(this, DB);
    ActivityModel->setTable("Activity");
    ActivityModel->setRelation(3, QSqlRelation("Users", "id_Name", "LastName"));
    ActivityModel->setRelation(5, QSqlRelation("Products", "id", "Name"));
    ActivityModel->setSort(1, Qt::DescendingOrder);
    ActivityModel->setJoinMode(QSqlRelationalTableModel::LeftJoin);
    ActivityModel->select();
    ui->lst_Activity->setModel(ActivityModel);
    ui->lst_Activity->hideColumn(0);
    ui->lst_Activity->hideColumn(1);

    ActivityModel->setHeaderData(2, Qt::Horizontal, tr("Date"));
    ActivityModel->setHeaderData(3, Qt::Horizontal, tr("Utilisateur"));
    ActivityModel->setHeaderData(4, Qt::Horizontal, tr("Quantité"));
    ActivityModel->setHeaderData(5, Qt::Horizontal, tr("Produit"));
    ActivityModel->setHeaderData(6, Qt::Horizontal, tr("Commentaire"));
    ui->lst_Activity->resizeColumnsToContents();

    ui->tbl_advancedUser->setModel(UserModel);
    ui->tbl_advancedProducts->setModel(ProdModel);
    ui->tbl_advancedActivity->setModel(ActivityModel);

}

MainWindow::~MainWindow()
{
    saveSettings();

    delete ActivityModel;
    delete UserModel;
    delete ProdModel;

    delete Mapper;

    delete ui;
}


void MainWindow::openSettings()
{
    if(SettingsWindow != nullptr)
        SettingsWindow->setFocus();

    else
    {
        SettingsWindow = new PreferencesWindow(this, &mSettings);
        connect(SettingsWindow, &PreferencesWindow::settingChanged, this, &MainWindow::applySettings);
        connect(SettingsWindow, &PreferencesWindow::finished, this, &MainWindow::closeSettings);
        SettingsWindow->show();
    }
}

bool MainWindow::saveSettings()
{
    QFile saveFile("preferences.json");
    saveFile.open(QIODevice::ReadWrite);

    QJsonObject Settings;
    Settings.insert("DatabasePath", QJsonValue(mSettings.DB_Path()));

    Settings.insert("negColor", QJsonValue(mSettings.negColor().name()));
    Settings.insert("posColor", QJsonValue(mSettings.posColor().name()));
    Settings.insert("zerColor", QJsonValue(mSettings.zerColor().name()));

    Settings.insert("backColor", QJsonValue(mSettings.backColor().name()));
    Settings.insert("rowAColor", QJsonValue(mSettings.rowAColor().name()));
    Settings.insert("rowBColor", QJsonValue(mSettings.rowBColor().name()));

    QJsonDocument saveDoc(Settings);
    saveFile.write(saveDoc.toJson());

    return true;
}


bool MainWindow::loadSettings()
{
    QFile saveFile("preferences.json");

    if(!saveFile.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this, "Oops", "Oopsy pas de fichier de préférences ! Allons chercher la base de donnée Activité machine !\n");
        mSettings.setDB_Path(QFileDialog::getOpenFileName(this, tr("Sélectionner une base de donnée machines"), ".", tr("(ActiviteMachine.db)")));
        return saveSettings();
    }

    else
    {
        QJsonObject Preferences(QJsonDocument::fromJson(saveFile.readAll()).object());

        if(Preferences.contains("DatabasePath") && Preferences["DatabasePath"].isString())
            mSettings.setDB_Path(Preferences["DatabasePath"].toString());
        else
            return false;

        if(Preferences.contains("negColor") && Preferences["negColor"].isString())
            mSettings.setNegColor(QColor(Preferences["negColor"].toString()));
        else
            return false;

        if(Preferences.contains("posColor") && Preferences["posColor"].isString())
            mSettings.setPosColor(QColor(Preferences["posColor"].toString()));
        else
            return false;

        if(Preferences.contains("zerColor") && Preferences["zerColor"].isString())
            mSettings.setZerColor(QColor(Preferences["zerColor"].toString()));
        else
            return false;

        if(Preferences.contains("backColor") && Preferences["backColor"].isString())
            mSettings.setBackColor(QColor(Preferences["backColor"].toString()));
        else
            return false;

        if(Preferences.contains("rowAColor") && Preferences["rowAColor"].isString())
            mSettings.setRowAColor(QColor(Preferences["rowAColor"].toString()));
        else
            return false;

        if(Preferences.contains("rowBColor") && Preferences["rowBColor"].isString())
            mSettings.setRowBColor(QColor(Preferences["rowBColor"].toString()));
        else
            return false;

        return true;

    }
}

void MainWindow::applySettings()
{
    ui->tm_I3D->setColorSet(mSettings.posColor(), mSettings.zerColor(), mSettings.negColor());
    ui->tm_Las->setColorSet(mSettings.posColor(), mSettings.zerColor(), mSettings.negColor());
    ui->tm_Plo->setColorSet(mSettings.posColor(), mSettings.zerColor(), mSettings.negColor());
    ui->tm_Sho->setColorSet(mSettings.posColor(), mSettings.zerColor(), mSettings.negColor());
    ui->tm_Cer->setColorSet(mSettings.posColor(), mSettings.zerColor(), mSettings.negColor());
    ui->dbl_Quantity->setColorSet(mSettings.posColor(), mSettings.zerColor(), mSettings.negColor());

    QPalette Palette;
    Palette.setColor(QPalette::Window, mSettings.backColor());
    this->setPalette(Palette);

    Palette = QPalette();
    ui->lst_Activity->setAlternatingRowColors(mSettings.alternateRowColor());
    Palette.setColor(QPalette::Base, mSettings.rowAColor());
    Palette.setColor(QPalette::AlternateBase, mSettings.rowBColor());
    ui->lst_Activity->setPalette(Palette);

    selectDatabase(mSettings.DB_Path());
}

void MainWindow::closeSettings()
{
    SettingsWindow = nullptr;
    saveSettings();
    applySettings();
}

void MainWindow::ValidateEntry()
{
    double Quantity;
    QSqlRecord ProdRow;
    QSqlRecord UserRow;

    Quantity = ui->dbl_Quantity->value();
    ProdRow = ProdModel->record(ui->cmb_Prod->currentIndex());
    UserRow = UserModel->record(ui->cmb_UserName->currentIndex());

    if(ProdRow.value("Name").toString() == QString(""))
    {
        ui->cmb_Prod->setFocus();
        return;
    }


    if(Quantity != 0.0)
    {
        QString Question("Voulez vous ");

        if(Quantity > 0)
            Question += QString("ajouter ");
        else
            Question += QString("retirer ");

        Question += QString::number(abs(Quantity));
        Question += QString(" ");
        Question += ProdRow.value("Name").toString();
        Question += QString(" à ");
        Question += UserRow.value("Name").toString() + " " + UserRow.value("LastName").toString();
        Question += QString(" ?");

        if(QMessageBox::question(this, "Confirmation", Question, QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::StandardButton::Yes)
        {
            ui->tm_I3D->setValue(ui->tm_I3D->value()+ ProdRow.value("Counter_I3D").toDouble()*Quantity);
            Mapper->submit();
            ui->tm_Las->setValue(ui->tm_Las->value()+ ProdRow.value("Counter_Las").toDouble()*Quantity);
            Mapper->submit();
            ui->tm_Plo->setValue(ui->tm_Plo->value()+ ProdRow.value("Counter_Plo").toDouble()*Quantity);
            Mapper->submit();
            ui->tm_Sho->setValue(ui->tm_Sho->value()+ ProdRow.value("Counter_Sho").toDouble()*Quantity);
            Mapper->submit();
            ui->tm_Cer->setValue(ui->tm_Cer->value()+ ProdRow.value("Counter_Cer").toDouble()*Quantity);
            Mapper->submit();

            int row = ActivityModel->rowCount()-1;

            if(row < 0)
                row = 0;

            ActivityModel->insertRow(row);

            QSqlQuery query(DB);

            query.prepare("INSERT INTO Activity (Timestamp, Date, UserName_id, Quantity, Product_id, Comment) "
                          "VALUES (?, ?, ?, ?, ?, ?)");
            query.addBindValue(QDateTime::currentDateTime());
            query.addBindValue(QDateTime::currentDateTime().toString("dd/MM/yy hh:mm"));
            query.addBindValue(UserRow.value("id_Name").toInt());
            query.addBindValue(Quantity);
            query.addBindValue(ProdRow.value("id").toInt());
            query.addBindValue(ui->ln_Comment->text());

            if(!query.exec())
                QMessageBox::warning(this, "Oops..", "Impossible d'ajouter cette entrée aux activités utilisateurs...");

            ActivityModel->select();

            ui->cmb_Prod->setCurrentIndex(0);
            ui->dbl_Quantity->setValue(0.0);
            ui->ln_Comment->setText("");
        }
    }

    else
    {
        ui->dbl_Quantity->setFocus();
        ui->dbl_Quantity->findChild<QLineEdit*>()->selectAll();
    }

}

void MainWindow::createMaker()
{
    Maker maker = MakerCreator::create(this);

    if(maker.UserName == QString("Invalid"))
        return;

    QSqlRecord rec = UserModel->record();
    rec.setValue("id_name", QVariant(maker.UserName + maker.LastName));
    rec.setValue("Name", QVariant(maker.UserName));
    rec.setValue("LastName",QVariant(maker.LastName));
    rec.setValue("Mail",QVariant(maker.Mail));
    rec.setValue("Phone",QVariant(maker.Phone));
    rec.setValue("PriceCat",QVariant(maker.PriceCat));

    rec.setValue("Pass_I3D", QVariant(false));
    rec.setValue("Pass_Las", QVariant(false));
    rec.setValue("Pass_Sho", QVariant(false));
    rec.setValue("Pass_Cer", QVariant(false));
    rec.setValue("Pass_Plo", QVariant(false));

    rec.setValue("Counter_I3D", QVariant(0));
    rec.setValue("Counter_Las", QVariant(0));
    rec.setValue("Counter_Sho", QVariant(0));
    rec.setValue("Counter_Cer", QVariant(0));
    rec.setValue("Counter_Plo", QVariant(0));

    bool success = UserModel->insertRecord(-1, rec);

    if(!success)
        QMessageBox::information(this, "Erreur de création", "Impossible de créer le nouvel utilisateur...");

}


void MainWindow::validateCombo()
{
    Mapper->setCurrentIndex(ui->cmb_UserName->currentIndex());
}

void MainWindow::UserChanged()
{
    if(UserActivityFiltering)
        ActivityModel->setFilter("UserName_id = '" + UserModel->record(ui->cmb_UserName->currentIndex()).field(UserTable::id_Name).value().toString() + "'");
}


void MainWindow::UserFilter(bool State)
{
    UserActivityFiltering = State;

    if(State)
        ActivityModel->setFilter("UserName_id = '" + UserModel->record(ui->cmb_UserName->currentIndex()).field(UserTable::id_Name).value().toString() + "'");

    else
        ActivityModel->setFilter("");
}
