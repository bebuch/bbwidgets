#include "../include/bbwidgets/led.hpp"

#include <QFontMetrics> 
#include <QPainter>

#include <algorithm>


namespace bbwidgets {


    Led::Led(QWidget* const parent)
        : Led(Qt::green, parent)
        {}

    Led::Led(float const hsl_hue, QWidget* const parent)
        : QWidget(parent)
    {
        setHslHueF(hsl_hue);
        setAutoFillBackground(true);
    }

    Led::Led(Qt::GlobalColor const color, QWidget* const parent)
        : Led(QColor(color), parent)
        {}

    Led::Led(QColor const& color, QWidget* const parent)
        : Led(color.hslHueF(), parent)
        {}

    float Led::hslHueF() const {
        return hsl_hue_;
    }

    void Led::setHslHueF(float const hue) {
        hsl_hue_ = std::clamp(hue, 0.f, 1.f);
        hslHueFChanged(hsl_hue_);
    }

    QColor Led::color() const {
        return QColor::fromHslF(hsl_hue_, 1.f, .4f);
    }

    void Led::setColor(QColor const& color) {
        setHslHueF(color.hslHueF());
    }

    QSize Led::sizeHint() const {
        auto const s = QFontMetrics{{}}.height();
        return { s, s };
    }

    void Led::paintEvent(QPaintEvent* event) {
        auto const [w, h] = QSizeF(size());
        auto const s = std::min(w, h);
        auto const border_square = QRectF(w / 2 - s / 2, h / 2 - s / 2, s, s);

        auto const bg_color = palette().brush(QPalette::Window).color();
        auto const border_color =
            bg_color.lightnessF() > 0.5
            ? bg_color.darker(120)
            : bg_color.lighter(120);

        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(Qt::NoPen);
        painter.setBrush(border_color);
        painter.drawEllipse(border_square);

        auto const basic_color = color();
        auto const basic_square = border_square.adjusted(s * 0.1, s * 0.1, s * -0.1, s * -0.1);

        auto const basic_from = border_square.topLeft() + QPoint(0, s * 0.1);
        auto const basic_to = border_square.bottomLeft();

        auto basic_gradient = QLinearGradient(basic_from, basic_to);
        basic_gradient.setColorAt(0.0, basic_color);
        basic_gradient.setColorAt(1.0, basic_color.lighter(220));
        painter.setBrush(basic_gradient);
        painter.drawEllipse(basic_square);

        auto const glare_rect = basic_square.adjusted(s * 0.1, s * 0.1, s * -0.1, s * -0.3);

        auto glare_gradient = QLinearGradient(basic_from, basic_to);
        glare_gradient.setColorAt(0.0, basic_color.lighter(280));
        glare_gradient.setColorAt(1.0, basic_color.lighter(180));
        painter.setBrush(glare_gradient);
        painter.drawEllipse(glare_rect);
    }


}
