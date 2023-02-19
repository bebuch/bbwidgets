#include "../include/bbwidgets/led.hpp"

#include <QFontMetrics> 
#include <QPainter>
#include <QEvent>

#include <algorithm>
#include <cmath>


namespace bbwidgets {



    static void drawLedImpl(
        QPainter& painter,
        QRectF const& pos,
        std::optional<int> const hue,
        float const activation,
        bool const enabled
    ) noexcept {
        auto const [w, h] = pos.size();
        auto const s = std::min(w, h);
        auto const border_square = QRectF(w / 2 - s / 2, h / 2 - s / 2, s, s);

        auto const basic_color = [hue, enabled, activation] {
            if (hue) {
                auto const lightness = std::lerp(.2f, .4f, activation);
                auto const saturation = enabled ? 1.f : .15f;
                return QColor::fromHslF(*hue / 360.f, saturation, lightness);
            }
            else {
                auto const lightness = enabled
                    ? std::lerp(.2f, .4f, activation)
                    : std::lerp(.6f, .8f, activation);
                return QColor::fromHslF(0.f, 0.f, lightness);
            }
        }();
        auto const border_color = basic_color.darker(150);

        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(Qt::NoPen);
        painter.setBrush(border_color);
        painter.drawEllipse(border_square);

        auto const basic_square = border_square.adjusted(s * 0.1, s * 0.1, s * -0.1, s * -0.1);

        auto const basic_from = border_square.topLeft() + QPoint(0, s * 0.1);
        auto const basic_to = border_square.bottomLeft();

        auto basic_gradient = QLinearGradient(basic_from, basic_to);
        auto const base_color_light = enabled ? 150 : 250;
        basic_gradient.setColorAt(0.0, basic_color.lighter(std::lerp(base_color_light, 0, activation)));
        basic_gradient.setColorAt(1.0, basic_color.lighter(std::lerp(base_color_light, 220, activation)));
        painter.setBrush(basic_gradient);
        painter.drawEllipse(basic_square);

        if (activation > 0.f) {
            auto const glare_rect = basic_square.adjusted(s * 0.1, s * 0.1, s * -0.1, s * -0.3);

            auto glare_gradient = QLinearGradient(basic_from, basic_to);
            glare_gradient.setColorAt(0.0, basic_color.lighter(280));
            glare_gradient.setColorAt(1.0, basic_color.lighter(180));
            painter.setBrush(glare_gradient);
            painter.drawEllipse(glare_rect);
        }
    }

    void drawLed(
        QPainter& painter,
        QRectF const& pos,
        std::optional<int> const hue,
        float const activation,
        bool const enabled
    ) noexcept {
        drawLedImpl(painter, pos, LedState::normalized(hue), std::clamp(activation, 0.f, 1.f), enabled);
    }

    LedState::LedState(std::optional<int> const hue, bool const checked, bool const enabled) noexcept
        : hue_(normalized(hue))
        , checked_(checked)
        , enabled_(enabled)
        {}

    LedState::LedState(int const hue, bool const checked, bool const enabled) noexcept
        : LedState(std::optional(hue), checked, enabled)
        {}

    LedState::LedState(QColor const& color, bool const checked, bool const enabled) noexcept
        : LedState(toHue(color), checked, enabled)
        {}

    LedState::LedState(Qt::GlobalColor const color, bool const checked, bool const enabled) noexcept
        : LedState(QColor(color), checked, enabled)
        {}

    void LedState::unsetHue() noexcept {
        hue_.reset();
    }

    void LedState::setHue(std::optional<int> const hue) noexcept {
        hue_ = normalized(hue);
    }

    void LedState::setHueBy(QColor const& color) noexcept {
        setHue(toHue(color));
    }

    void LedState::setChecked(bool const checked) noexcept {
        checked_ = checked;
    }

    void LedState::setEnabled(bool const enabled) noexcept {
        enabled_ = enabled;
    }

    std::optional<int> LedState::hue() const noexcept {
        return hue_;
    }

    bool LedState::isChecked() const noexcept {
        return checked_;
    }

    bool LedState::isEnabled() const noexcept {
        return enabled_;
    }

    std::optional<int> LedState::toHue(QColor const& color) noexcept {
        if (color.hslSaturation() == 0) {
            return std::nullopt;
        } else {
            return color.hue();
        }
    }

    std::optional<int> LedState::normalized(std::optional<int> const hue) noexcept {
        if (hue) {
            return cyclic_adapt(*hue, 0, 360);
        } else {
            return std::nullopt;
        }
    }


    Led::Led(QWidget* const parent) noexcept
        : Led(LedState{}, parent)
        {}

    Led::Led(LedState const& state, QWidget* const parent) noexcept
        : QWidget(parent)
        , hue_(state.hue())
        , checked_(state.isChecked())
    {
        setEnabled(state.isEnabled());
        setAutoFillBackground(true);
    }

    LedState Led::state() const noexcept {
        return { hue_, checked_, isEnabled()};
    }

    void Led::setState(LedState const& state) noexcept {
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
        QPainter painter(this);
        drawLed(painter, rect(), hue_, checked_ ? 1.f : 0.f, isEnabled());
    }

    void Led::changeEvent(QEvent* event) {
        switch (event->type()) {
        case QEvent::EnabledChange:
            update();
            break;
        }
    }

}
