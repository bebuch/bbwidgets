#include "../include/bbwidgets/led.hpp"

#include <QFontMetrics> 
#include <QPainter>
#include <QEvent>

#include <algorithm>
#include <cmath>


namespace bbwidgets {


    static_assert(cyclic_adapt(0, 0, 3) == 0);
    static_assert(cyclic_adapt(1, 0, 3) == 1);
    static_assert(cyclic_adapt(2, 0, 3) == 2);
    static_assert(cyclic_adapt(3, 0, 3) == 0);
    static_assert(cyclic_adapt(4, 0, 3) == 1);
    static_assert(cyclic_adapt(5, 0, 3) == 2);
    static_assert(cyclic_adapt(6, 0, 3) == 0);
    static_assert(cyclic_adapt(7, 0, 3) == 1);
    static_assert(cyclic_adapt(-1, 0, 3) == 2);
    static_assert(cyclic_adapt(-2, 0, 3) == 1);
    static_assert(cyclic_adapt(-3, 0, 3) == 0);
    static_assert(cyclic_adapt(-4, 0, 3) == 2);
    static_assert(cyclic_adapt(-5, 0, 3) == 1);
    static_assert(cyclic_adapt(-6, 0, 3) == 0);
    static_assert(cyclic_adapt(-7, 0, 3) == 2);

    LedState::LedState(std::optional<int> const hue, bool const check, bool const enabled)
        : hue_(normalized(hue))
        , checked_(check)
        , enabled_(enabled)
        {}

    LedState::LedState(int const hue, bool const state, bool const enabled)
        : LedState(std::optional(hue), state, enabled)
        {}

    LedState::LedState(QColor const& color, bool const state, bool const enabled)
        : LedState(toHue(color), state, enabled)
        {}

    LedState::LedState(Qt::GlobalColor const color, bool const state, bool const enabled)
        : LedState(QColor(color), state, enabled)
        {}

    void LedState::unsetHue() {
        hue_.reset();
    }

    void LedState::setHue(std::optional<int> const hue) {
        hue_ = normalized(hue);
    }

    void LedState::setHueBy(QColor const& color) {
        setHue(toHue(color));
    }

    std::optional<int> LedState::hue() const {
        return hue_;
    }

    bool LedState::isChecked() const {
        return checked_;
    }

    bool LedState::isEnabled() const {
        return enabled_;
    }

    std::optional<int> LedState::toHue(QColor const& color) {
        if (color.hslSaturation() == 0) {
            return std::nullopt;
        } else {
            return color.hue();
        }
    }

    std::optional<int> LedState::normalized(std::optional<int> const hue) {
        if (hue) {
            return cyclic_adapt(*hue, 0, 360);
        } else {
            return std::nullopt;
        }
    }


    Led::Led(QWidget* const parent)
        : Led(LedState{}, parent)
        {}

    Led::Led(LedState const& state, QWidget* const parent)
        : QWidget(parent)
        , hue_(state.hue())
        , check_(state.isChecked())
    {
        setEnabled(state.isEnabled());
        setAutoFillBackground(true);
    }

    LedState Led::state() const {
        return { hue_, check_, isEnabled()};
    }

    void Led::setState(LedState const& state) {
        hue_ = state.hue();
        check_ = state.isChecked();
        setEnabled(state.isEnabled());
        stateChanged();
    }

    QSize Led::sizeHint() const {
        auto const s = QFontMetrics{ {} }.height();
        return { s, s };
    }

    void Led::paintEvent(QPaintEvent*) {
        auto const [w, h] = QSizeF(size());
        auto const s = std::min(w, h);
        auto const border_square = QRectF(w / 2 - s / 2, h / 2 - s / 2, s, s);
        
        auto const basic_color = [this] {
            if (hue_) {
                auto const lightness = check_ ? .4f : .2f;
                auto const saturation = isEnabled() ? 1.f : .2f;
                return QColor::fromHslF(*hue_ / 360.f, saturation, lightness);
            } else {
                auto const lightness = isEnabled() ? (check_ ? .4f : .2f) : (check_ ? .8f : .6f);
                return QColor::fromHslF(0.f, 0.f, lightness);
            }
        }();
        auto const border_color = basic_color.darker(150);

        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(Qt::NoPen);
        painter.setBrush(border_color);
        painter.drawEllipse(border_square);

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

    void Led::changeEvent(QEvent* event) {
        switch (event->type()) {
        case QEvent::EnabledChange:
            update();
            break;
        }
    }

}
