#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QString Path, QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    isConnected = false;

    DB = QSqlDatabase::addDatabase("QSQLITE");

    //Connexion à la base de données
    if(openDB(Path))
        DB_Path = Path;

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
        ui->viw_UserTable->setModel(UserModel);

        ProdModel = new QSqlTableModel(this, DB);
        ProdModel->setTable("Products");
        ProdModel->setSort(ProdTable::ProdName, Qt::AscendingOrder);
        ProdModel->select();
        ui->cmb_Prod->setModel(ProdModel);
        ui->cmb_Prod->setModelColumn(1);

        ActivityModel = new QSqlRelationalTableModel(this, DB);
        ActivityModel->setTable("Activity");
        ActivityModel->setRelation(2, QSqlRelation("Users", "id_Name", "id_Name"));
        ActivityModel->setRelation(4, QSqlRelation("Products", "id", "Name"));
        ActivityModel->setSort(2, Qt::DescendingOrder);
        ActivityModel->select();
        ui->lst_Activity->setModel(ActivityModel);
        ui->lst_Activity->hideColumn(0);

    }

    else
        ui->Tab_ActiviteMachine->setEnabled(false);

    connect(ui->dbl_Quantity, QOverload<const double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::QuantityChanged, Qt::QueuedConnection);

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
    delete ui;
}



void MainWindow::QuantityChanged(double Qty)
{
    if(Qty < 0.0)
    {
        QPalette palette;
        QColor Red;
        Red.setRgb(255, 90, 90);
        palette.setColor(QPalette::Base,Red);
        //palette.setColor(QPalette::Text, Qt::white);
        ui->dbl_Quantity->setPalette(palette);
    }

    if(Qty == 0.0)
    {
        QPalette palette;
        palette.setColor(QPalette::Base,Qt::white);
        //palette.setColor(QPalette::Text, Qt::black);
        ui->dbl_Quantity->setPalette(palette);
    }

    if(Qty > 0.0)
    {
        QPalette palette;
        QColor Green;
        Green.setRgb(90, 255, 90);
        palette.setColor(QPalette::Base, Green);
        //palette.setColor(QPalette::Text, Qt::white);
        ui->dbl_Quantity->setPalette(palette);
    }

    ui->dbl_Quantity->findChild<QLineEdit*>()->deselect();
}




void MainWindow::ValidateEntry()
{
    double Quantity;

    Quantity = ui->dbl_Quantity->value();

    if(Quantity > 0.0)
    {

    }

    else if(Quantity < 0.0)
    {

    }

    else
        ui->dbl_Quantity->setFocus();

}
