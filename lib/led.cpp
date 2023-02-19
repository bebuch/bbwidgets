#include "../include/bbwidgets/led.hpp"

#include <QFontMetrics> 
#include <QPainter>
#include <QEvent>

#include <algorithm>
#include <cmath>


namespace bbwidgets {


    LedState::LedState(std::optional<int> const hue, bool const checked, bool const enabled)
        : hue_(normalized(hue))
        , checked_(checked)
        , enabled_(enabled)
        {}

    LedState::LedState(int const hue, bool const checked, bool const enabled)
        : LedState(std::optional(hue), checked, enabled)
        {}

    LedState::LedState(QColor const& color, bool const checked, bool const enabled)
        : LedState(toHue(color), checked, enabled)
        {}

    LedState::LedState(Qt::GlobalColor const color, bool const checked, bool const enabled)
        : LedState(QColor(color), checked, enabled)
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

    void LedState::setChecked(bool const checked) {
        checked_ = checked;
    }

    void LedState::setEnabled(bool const enabled) {
        enabled_ = enabled;
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
        , checked_(state.isChecked())
    {
        setEnabled(state.isEnabled());
        setAutoFillBackground(true);
    }

    LedState Led::state() const {
        return { hue_, checked_, isEnabled()};
    }

    void Led::setState(LedState const& state) {
        hue_ = state.hue();
        checked_ = state.isChecked();
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
                auto const lightness = checked_ ? .4f : .2f;
                auto const saturation = isEnabled() ? 1.f : .15f;
                return QColor::fromHslF(*hue_ / 360.f, saturation, lightness);
            } else {
                auto const lightness = isEnabled() ? (checked_ ? .4f : .2f) : (checked_ ? .8f : .6f);
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

        if (checked_) {
            auto basic_gradient = QLinearGradient(basic_from, basic_to);
            basic_gradient.setColorAt(0.0, basic_color);
            basic_gradient.setColorAt(1.0, basic_color.lighter(220));
            painter.setBrush(basic_gradient);
        } else {
            if (isEnabled()) {
                painter.setBrush(basic_color.lighter(150));
            } else {
                painter.setBrush(basic_color.lighter(250));
            }
        }
        painter.drawEllipse(basic_square);

        if (checked_) {
            auto const glare_rect = basic_square.adjusted(s * 0.1, s * 0.1, s * -0.1, s * -0.3);

            auto glare_gradient = QLinearGradient(basic_from, basic_to);
            glare_gradient.setColorAt(0.0, basic_color.lighter(280));
            glare_gradient.setColorAt(1.0, basic_color.lighter(180));
            painter.setBrush(glare_gradient);
            painter.drawEllipse(glare_rect);
        }
    }

    void Led::changeEvent(QEvent* event) {
        switch (event->type()) {
        case QEvent::EnabledChange:
            update();
            break;
        }
    }

}
