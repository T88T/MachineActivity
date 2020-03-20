#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    isConnected = false;
    UserActivityFiltering = false;

    loadSettings();

    //Connexion à la base de données
    DB = QSqlDatabase::addDatabase("QSQLITE");
    openDB(DB_Path);

    //Preloading data forms
    if(isConnected)
    {
        UserModel = new QSqlTableModel(this, DB);
        UserModel->setTable("Users");
        UserModel->setEditStrategy(QSqlTableModel::OnFieldChange);
        UserModel->setSort(UserTable::id_Name, Qt::AscendingOrder);
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

        ui->cmb_UserName->setModel(UserModel);
        //ui->viw_UserTable->setModel(UserModel);

        ProdModel = new QSqlTableModel(this, DB);
        ProdModel->setTable("Products");
        ProdModel->setSort(ProdTable::ProdName, Qt::AscendingOrder);
        ProdModel->select();
        ui->cmb_Prod->setModel(ProdModel);
        ui->cmb_Prod->setModelColumn(1);

        ActivityModel = new QSqlRelationalTableModel(this, DB);
        ActivityModel->setTable("Activity");
        ActivityModel->setRelation(3, QSqlRelation("Users", "id_Name", "id_Name"));
        ActivityModel->setRelation(5, QSqlRelation("Products", "id", "Name"));
        ActivityModel->setSort(1, Qt::DescendingOrder);
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
    }

    else
        this->setEnabled(false);

}

bool MainWindow::openDB(QString Path)
{
    DB.setDatabaseName(Path);

    if(!QFile::exists(Path))
    {
        QMessageBox::information(this, "Accès à la base de donnée", "Le fichier de la base de données n'éxiste pas. Vérifiez le chemin d'accès dans les préférences.");
        isConnected = false;
        return false;
    }
    if (!DB.open())
    {
       QMessageBox::information(this, "Accès à la base de donnée", "L'ouverture de la base de données n'as pas fonctionné. Fichier corrompu ?...");
       isConnected = false;
       return false;
    }
    else
    {
       qDebug() << "Connection à la base de données : OK";
       isConnected = true;
       return true;
    }
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

bool MainWindow::saveSettings()
{
    QFile saveFile("preferences.json");
    saveFile.open(QIODevice::ReadWrite);

    QJsonObject DatabasePath;
    DatabasePath.insert("DatabasePath", QJsonValue(DB_Path));

    QJsonDocument saveDoc(DatabasePath);
    saveFile.write(saveDoc.toJson());

    return true;
}


bool MainWindow::loadSettings()
{
    QFile saveFile("preferences.json");

    if(!saveFile.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this, "Oops", "Oopsy pas de fichier de préférences ! \n");
        DB_Path = QFileDialog::getOpenFileName(this);
        return saveSettings();
    }

    else
    {
        QJsonObject Preferences(QJsonDocument::fromJson(saveFile.readAll()).object());

        if(Preferences.contains("DatabasePath") && Preferences["DatabasePath"].isString())
        {
            DB_Path = Preferences["DatabasePath"].toString();
            return true;
        }

        else
            return false;
    }
}



void MainWindow::ValidateEntry()
{
    double Quantity;
    QSqlRecord ProdRow;
    QSqlRecord UserRow;

    Quantity = ui->dbl_Quantity->value();
    ProdRow = ProdModel->record(ui->cmb_Prod->currentIndex());
    UserRow = UserModel->record(ui->cmb_UserName->currentIndex());

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
        Question += UserRow.value("id_Name").toString();
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

            QSqlRecord Activity;
            QSqlField Timestamp("Timestamp", QVariant::DateTime);
            QSqlField Date("Date", QVariant::Date);
            QSqlField UserName_id("UserName_id", QVariant::String);
            QSqlField Qty("Quantity", QVariant::Double);
            QSqlField ProductId("Product_id", QVariant::Double);
            QSqlField Comment("Comment", QVariant::String);

            Timestamp.setValue(QDateTime::currentDateTime());
            Date.setValue(QDateTime::currentDateTime().toString("dd/MM/yy hh:mm"));
            UserName_id.setValue(UserRow.value("id_Name").toString());
            Qty.setValue(Quantity);
            ProductId.setValue(ProdRow.value("id").toDouble());
            Comment.setValue(ui->ln_Comment->text());

            Activity.append(Timestamp);
            Activity.append(Date);
            Activity.append(UserName_id);
            Activity.append(Qty);
            Activity.append(ProductId);
            Activity.append(Comment);

            ActivityModel->insertRecord(-1, Activity);
            ActivityModel->select();
        }
    }

    else
    {
        ui->dbl_Quantity->setFocus();
        ui->dbl_Quantity->findChild<QLineEdit*>()->selectAll();
    }

}

void MainWindow::UserChanged()
{
    if(UserActivityFiltering)
        ActivityModel->setFilter("UserName_id = '" + ui->cmb_UserName->currentText() + "'");
}


void MainWindow::UserFilter(bool State)
{
    UserActivityFiltering = State;

    if(State)
        ActivityModel->setFilter("UserName_id = '" + ui->cmb_UserName->currentText() + "'");

    else
        ActivityModel->setFilter("");
}
