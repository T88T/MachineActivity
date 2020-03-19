#ifndef USER_H
#define USER_H

#include <QString>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QTime>


class User
{
    public:
        User();
        void load(QSqlRecord Record);

    public:
        QString id_Name;
        QString Name;
        QString LastName;
        QString Mail;
        QString Phone;
        int PriceCat;

        bool PassI3D;
        bool PassLas;
        bool PassPlo;
        bool PassSho;
        bool PassCer;

        double CounterI3D;
        double CounterLas;
        double CounterPlo;
        double CounterSho;
        double CounterCer;
};

#endif // USER_H
