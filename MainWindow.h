#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlRelationalTableModel>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDataWidgetMapper>
#include <QFile>
#include <QColor>

#include "User.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        MainWindow( QString Path, QWidget *parent = nullptr);
        ~MainWindow();

        bool openDB(QString Path);

    public slots:
        void ValidateEntry();
        void QuantityChanged(double Qty);

    private:
        bool UserCommit(int id);

    private:

        enum UserTable{id_Name = 0,
                       Name = 1,
                       LastName = 2,
                       Mail = 3,
                       Phone = 4,
                       PriceCat = 5,
                       Pass_I3D = 6,
                       Pass_Las = 7,
                       Pass_Plo = 8,
                       Pass_Sho = 9,
                       Pass_Cer = 10,
                       Counter_I3D = 11,
                       Counter_Las = 12,
                       Counter_Plo = 13,
                       Counter_Sho = 14,
                       Counter_Cer = 15};

        enum ProdTable{ProdId = 0,
                       ProdName = 1};

        enum ActivityTable{ActivityId = 0,
                           Date = 1,
                           User_id = 2,
                           Product_id = 3,
                           Quantity = 4,
                           Comment = 5};

        Ui::MainWindow *ui;

        QString DB_Path;
        QSqlDatabase DB;

        bool isConnected;
        QDataWidgetMapper *Mapper;

        QSqlRelationalTableModel *ActivityModel;
        QSqlTableModel *ProdModel;
        QSqlTableModel *UserModel;
};
#endif // MAINWINDOW_H
