#include "../include/bbwidgets/led.hpp"

#include <QPainter>


namespace bbwidgets {


    Led::Led(QWidget* const parent)
        : QWidget(parent)
    {
        setAutoFillBackground(true);
    }

    void Led::paintEvent(QPaintEvent* event) {
        auto const [w, h] = QSizeF(size());
        auto const s = std::min(w, h);
        auto const square = QRectF(w / 2 - s / 2, h / 2 - s / 2, s, s);

        auto const bg_color = palette().brush(QPalette::Window).color();
        auto const border_color =
            bg_color.lightnessF() > 0.5
            ? bg_color.darker(120)
            : bg_color.lighter(120);

        QPainter painter(this);
        painter.setPen(Qt::NoPen);
        painter.setBrush(border_color);
        painter.drawEllipse(square);
    }


}
