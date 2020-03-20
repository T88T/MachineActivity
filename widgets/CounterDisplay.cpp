#include "CounterDisplay.h"

CounterDisplay::CounterDisplay(QWidget *parent) : QDoubleSpinBox(parent)
{
    connect(this, QOverload<const double>::of(&QDoubleSpinBox::valueChanged), this, &CounterDisplay::updateColor, Qt::QueuedConnection);

    posColor.setRgb(90, 255, 90);
    zerColor = Qt::white;
    negColor.setRgb(255, 90, 90);

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
