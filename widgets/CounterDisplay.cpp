#include "CounterDisplay.h"
#include <qDebug>


CounterDisplay::CounterDisplay(QWidget *parent) : QDoubleSpinBox(parent)
{
    connect(this, QOverload<const double>::of(&QDoubleSpinBox::valueChanged), this, &CounterDisplay::updateColor, Qt::QueuedConnection);

    posColor.setRgb(90, 255, 90);
    zerColor = Qt::white;
    negColor.setRgb(255, 90, 90);

}

QString CounterDisplay::textFromValue(double val) const
{
    double h = int(val);
    double mn = (val - (int)val)*60;

    return QString::number(h)+QString(" h ")+QString::number(abs(mn))+(abs(mn) < 10 ? QString("0") : QString(""));
}

double CounterDisplay::valueFromText(const QString &text) const
{
    QString value = text;
    value.remove(QChar(' '), Qt::CaseInsensitive);
    value.replace(QChar(','), '.', Qt::CaseInsensitive);

    double temp = value.toDouble();
    double h = (int)(temp);
    double mn = temp - h;

    qDebug() << text << temp << " " << h << " " << mn << endl;

    return h+mn;
}

void CounterDisplay::updateColor(double value)
{
    if(value < 0.0)
    {
        QPalette palette;
        palette.setColor(QPalette::Base, negColor);
        this->setPalette(palette);
    }

    if(value == 0.0)
    {

        QPalette palette;
        palette.setColor(QPalette::Base, zerColor);
        this->setPalette(palette);
    }

    if(value > 0.0)
    {
        QPalette palette;
        palette.setColor(QPalette::Base, posColor);
        this->setPalette(palette);
    }

    this->findChild<QLineEdit*>()->deselect();
}

void CounterDisplay::setColorSet(QColor pos, QColor zer, QColor neg)
{
    posColor = pos;
    zerColor = zer;
    negColor = neg;

    updateColor(this->value());
}

void CounterDisplay::setPositiveColor(QColor &color)
{
    posColor = color;
}

void CounterDisplay::setZeroColor(QColor &color)
{
    zerColor = color;
}

void CounterDisplay::setNegativeColor(QColor &color)
{
    negColor = color;
}

QColor CounterDisplay::getPositiveColor() const
{
    return posColor;
}

QColor CounterDisplay::getZeroColor() const
{
    return zerColor;
}

QColor CounterDisplay::getNegativeColor() const
{
    return negColor;
}
