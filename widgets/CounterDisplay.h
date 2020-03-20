#ifndef COUNTERDISPLAY_H
#define COUNTERDISPLAY_H

#include <QWidget>
#include <QLineEdit>
#include <QDoubleSpinBox>

class CounterDisplay : public QDoubleSpinBox
{
    public:
        CounterDisplay(QWidget* parent = nullptr);

        void setPositiveColor(QColor &color);
        void setZeroColor(QColor &color);
        void setNegativeColor(QColor &color);

        QColor getPositiveColor() const;
        QColor getZeroColor() const;
        QColor getNegativeColor() const;

    public slots:
        void updateColor(double value);

    private:
        QColor negColor;
        QColor zerColor;
        QColor posColor;


};

#endif // COUNTERDISPLAY_H
