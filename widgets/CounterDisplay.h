#ifndef COUNTERDISPLAY_H
#define COUNTERDISPLAY_H

#include <QWidget>
#include <QLineEdit>
#include <QDoubleSpinBox>

class CounterDisplay : public QDoubleSpinBox
{
    public:
        CounterDisplay(QWidget* parent = nullptr);

        void setColorSet(QColor pos, QColor zer, QColor neg);

        void setPositiveColor(QColor &color);
        void setZeroColor(QColor &color);
        void setNegativeColor(QColor &color);

        QColor getPositiveColor() const;
        QColor getZeroColor() const;
        QColor getNegativeColor() const;

        QString textFromValue(double val) const override;
        double valueFromText(const QString &text) const override;

    public slots:
        void updateColor(double value);

    private:
        QColor negColor;
        QColor zerColor;
        QColor posColor;


};

#endif // COUNTERDISPLAY_H
