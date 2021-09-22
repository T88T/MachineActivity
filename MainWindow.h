#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlRelationalTableModel>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>
#include <QDataWidgetMapper>

#include <QJsonDocument>
#include <QJsonObject>

#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QColor>
#include <QDateTime>
#include <QCompleter>

#include "widgets/PreferencesWindow.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

        void openSettings();
        bool loadSettings();
        bool saveSettings();

        void selectDatabase(QString path);
        bool openDatabase(QString Path);
        QString browseForDatabase();
        void loadFromDatabase();

    public slots:
        void ValidateEntry();
        void UserFilter(bool State);
        void UserChanged();

        void applySettings();
        void closeSettings();
        void createMaker();

        void validateCombo();
        void validatePass();

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
                           Timestamp = 1,
                           Date = 2,
                           User_id = 3,
                           Quantity = 4,
                           Product_id = 5,
                           Comment = 6};

    private:

        Ui::MainWindow *ui;

        Settings mSettings;
        PreferencesWindow *SettingsWindow;

        QString DB_Path;
        QSqlDatabase DB;

        bool isConnected;
        bool UserActivityFiltering;

        QCompleter *UserCompleter;
        QCompleter *ProductCompleter;

        QDataWidgetMapper *Mapper;
        QSqlRelationalTableModel *ActivityModel;
        QSqlTableModel *ProdModel;
        QSqlTableModel *UserModel;
};
#endif // MAINWINDOW_H
