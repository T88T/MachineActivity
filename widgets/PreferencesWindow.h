#ifndef PREFERENCESWINDOW_H
#define PREFERENCESWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QColorDialog>
#include <QFileDialog>

namespace Ui {
class PreferencesWindow;
}

class Settings
{
    public:
        Settings();
        ~Settings();

        void setDB_Path(QString p);
        void setPosColor(QColor p);
        void setZerColor(QColor p);
        void setNegColor(QColor p);
        void setAlternateRowColor(bool p);
        void setRowAColor(QColor p);
        void setRowBColor(QColor p);
        void setBackColor(QColor p);

        const QString DB_Path() const;
        const QColor posColor() const;
        const QColor zerColor() const;
        const QColor negColor() const;
        bool alternateRowColor() const;
        const QColor rowAColor() const;
        const QColor rowBColor() const;
        const QColor backColor() const;

        bool edited();

    private:
        QString mDB_Path;
        QColor mPosColor;
        QColor mZerColor;
        QColor mNegColor;

        bool mAlternateRowColors;
        QColor mRowAColor;
        QColor mRowBColor;

        QColor mBackgroundColor;

        bool mHasChanged;
};


class PreferencesWindow : public QDialog
{
    Q_OBJECT

    public:
        PreferencesWindow(QWidget *parent, Settings *set);
        ~PreferencesWindow();

    public slots:
        void colorLabelClicked();
        void colorDialogOk(QColor newColor);
        void alternateRowColor(bool State);
        void browseForDatabase();

    signals:
        void settingChanged();
        void settingsClosed();

    private:
        Ui::PreferencesWindow *ui;
        Settings *mSettings;
        QLabel *currentEditedLabel;
        QColorDialog *ColorPicker;
};

#endif // PREFERENCESWINDOW_H
