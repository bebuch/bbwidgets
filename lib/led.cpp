#include "../include/bbwidgets/led.hpp"

#include <QFontMetrics> 
#include <QPainter>

#include <algorithm>
#include <cmath>


namespace bbwidgets {


    LedState::LedState(std::optional<float> const hsl_hue, bool const state, bool const enabled)
        : hsl_hue_(hsl_hue)
        , state_(state)
        , enabled_(enabled)
        {}

    LedState::LedState(float const hls_hue, bool const state, bool const enabled)
        : LedState(std::optional(hls_hue), state, enabled)
        {}

    LedState::LedState(QColor const& color, bool const state, bool const enabled)
        : LedState(toHlsHueF(color), state, enabled)
        {}

    LedState::LedState(Qt::GlobalColor const color, bool const state, bool const enabled)
        : LedState(QColor(color), state, enabled)
        {}

    std::optional<float> LedState::hslHueF() const {
        return hsl_hue_;
    }

    void LedState::unsetHslHueF() {
        hsl_hue_.reset();
    }

    void LedState::setHslHueF(std::optional<float> const hue) {
        if (hue && std::isnormal(*hue)) {
            hsl_hue_ = std::clamp(*hue, 0.f, 1.f);
        } else {
            unsetHslHueF();
        }
    }

    void LedState::unsetColor() {
        unsetHslHueF();
    }

    QColor LedState::color() const {
        if (hsl_hue_) {
            return QColor::fromHslF(*hsl_hue_, 1.f, .4f);
        } else {
            return QColor::fromHslF(0.f, 0.f, .4f);
        }
    }

    void LedState::setColor(QColor const& color) {
        setHslHueF(toHlsHueF(color));
    }

    std::optional<float> LedState::toHlsHueF(QColor const& color) {
        if (color.hslSaturationF() == 0.f) {
            return std::nullopt;
        } else {
            return color.hslHueF();
        }
    }


    Led::Led(QWidget* const parent)
        : Led(LedState{}, parent)
        {}

    Led::Led(LedState const& state, QWidget* const parent)
        : QWidget(parent)
        , state_(state)
    {
        setAutoFillBackground(true);
    }

    LedState Led::state() const {
        return state_;
    }

    void Led::setState(LedState const& state) {
        state_ = state;
        stateChanged(state_);
    }

    QSize Led::sizeHint() const {
        auto const s = QFontMetrics{ {} }.height();
        return { s, s };
    }

    void Led::paintEvent(QPaintEvent*) {
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

        auto const basic_color = state_.color();
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
