#ifndef MAKERCREATOR_H
#define MAKERCREATOR_H

#include <QDialog>

namespace Ui {
class MakerCreator;
}

class Maker
{
    public:
        Maker() : UserName(QString("Invalid")) {}

    public:
        QString UserName;
        QString LastName;
        QString Mail;
        QString Phone;
        bool PriceCat;
};

class MakerCreator : public QDialog
{
    Q_OBJECT

    public:
        explicit MakerCreator(QWidget *parent = nullptr);
        explicit MakerCreator(Maker maker, QWidget *parent = nullptr);

        ~MakerCreator();

        static Maker create(QWidget* parent);
        static void modify(Maker &maker, QWidget *parent);

        Maker getMaker();
        void accept() override;

    private:
        Ui::MakerCreator *ui;

};

#endif // MAKERCREATOR_H
