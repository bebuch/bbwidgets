#include "../include/bbwidgets/led.hpp"

#include <QEvent>
#include <QFontMetrics>
#include <QPainter>
#include <QVariantAnimation>

#include <algorithm>
#include <cmath>
#include <numeric>

namespace bbwidgets {


    static QVariant ledStateInterpolator(LedState const& start, LedState const& end, qreal const progress) {
        auto const shue = start.hue();
        auto const ehue = end.hue();
        auto const pg = static_cast<float>(progress);

        // BUG: We need to consider the direction of the color circle here!
        auto const hue = shue && ehue ? std::lerp(*shue, *ehue, pg) : ehue;

        auto const activation = std::lerp(start.activation(), end.activation(), pg);

        return QVariant::fromValue(LedState(hue, activation));
    }

    static void drawLedImpl(QPainter& painter, QRectF const& pos, std::optional<int> const hue, float const activation,
        bool const enabled) noexcept {
        auto const [w, h] = pos.size();
        auto const s = std::min(w, h);
        auto const border_rect = QRectF(w / 2 - s / 2, h / 2 - s / 2, s, s);

        auto const basic_color = [hue, enabled, activation] {
            if(hue) {
                auto const lightness = std::lerp(.2f, .4f, activation);
                auto const saturation = enabled ? 1.f : .15f;
                return QColor::fromHslF(*hue / 360.f, saturation, lightness);
            } else {
                auto const lightness = enabled ? std::lerp(.2f, .4f, activation) : std::lerp(.6f, .8f, activation);
                return QColor::fromHslF(0.f, 0.f, lightness);
            }
        }();
        auto const border_color = basic_color.darker(150);

        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(Qt::NoPen);
        painter.setBrush(border_color);
        painter.drawEllipse(border_rect);

        auto const basic_rect = border_rect.adjusted(s * 0.1, s * 0.1, s * -0.1, s * -0.1);

        auto const basic_from = border_rect.topLeft() + QPoint(0, s * 0.1);
        auto const basic_to = border_rect.bottomLeft();

        auto basic_gradient = QLinearGradient(basic_from, basic_to);
        auto const base_color_light = enabled ? 150 : 250;
        basic_gradient.setColorAt(0.0, basic_color.lighter(std::lerp(base_color_light, 100, activation)));
        basic_gradient.setColorAt(1.0, basic_color.lighter(std::lerp(base_color_light, 220, activation)));
        painter.setBrush(basic_gradient);
        painter.drawEllipse(basic_rect);

        if(activation > 0.f) {
            auto const fgr = basic_rect.adjusted(s * 0.1, s * 0.1, s * -0.1, s * -0.3);
            auto const glare_rect = fgr.adjusted(std::lerp(.3f, 0.f, activation) * fgr.width(),
                std::lerp(1.f / 3, 0.f, activation) * fgr.height(), std::lerp(.3f, 0.f, activation) * -fgr.width(),
                std::lerp(2.f / 3, 0.f, activation) * -fgr.height());

            auto glare_gradient = QLinearGradient(basic_from, basic_to);
            glare_gradient.setColorAt(0.0, basic_color.lighter(std::lerp(base_color_light, 280, activation)));
            glare_gradient.setColorAt(1.0, basic_color.lighter(std::lerp(base_color_light, 180, activation)));
            painter.setBrush(glare_gradient);
            painter.drawEllipse(glare_rect);
        }
    }

    void drawLed(QPainter& painter, QRectF const& pos, std::optional<int> const hue, float const activation,
        bool const enabled) noexcept {
        drawLedImpl(painter, pos, LedState::normalized(hue), std::clamp(activation, 0.f, 1.f), enabled);
    }

    LedState::LedState(std::optional<int> const hue, float const activation) noexcept
        : hue_(normalized(hue))
        , activation_(activation) {
        // we need to call qRegisterAnimationInterpolator on first element construction
        static [[maybe_unused]] auto const animation_registered = [] {
            qRegisterAnimationInterpolator<LedState>(ledStateInterpolator);
            return true;
        }();
    }

    LedState::LedState(std::optional<int> const hue, bool const checked) noexcept
        : LedState(hue, checked ? 1.f : 0.f) {}

    LedState::LedState(int const hue, float const activation) noexcept
        : LedState(std::optional(hue), activation) {}

    LedState::LedState(int const hue, bool const checked) noexcept
        : LedState(hue, checked ? 1.f : 0.f) {}

    LedState::LedState(QColor const& color, float const activation) noexcept
        : LedState(toHue(color), activation) {}

    LedState::LedState(QColor const& color, bool const checked) noexcept
        : LedState(color, checked ? 1.f : 0.f) {}

    LedState::LedState(Qt::GlobalColor const color, float const activation) noexcept
        : LedState(QColor(color), activation) {}

    LedState::LedState(Qt::GlobalColor const color, bool const checked) noexcept
        : LedState(color, checked ? 1.f : 0.f) {}

    LedState::LedState(LedState const&) noexcept = default;

    LedState::~LedState() = default;

    LedState& LedState::operator=(LedState const&) noexcept = default;

    void LedState::unsetHue() noexcept {
        hue_.reset();
    }

    void LedState::setHue(std::optional<int> const hue) noexcept {
        hue_ = normalized(hue);
    }

    void LedState::setHueBy(QColor const& color) noexcept {
        setHue(toHue(color));
    }

    void LedState::setActivation(float const activation) noexcept {
        activation_ = activation;
    }

    void LedState::setChecked(bool const checked) noexcept {
        setActivation(checked ? 1.f : 0.f);
    }

    std::optional<int> LedState::hue() const noexcept {
        return hue_;
    }

    float LedState::activation() const noexcept {
        return activation_;
    }

    bool LedState::isChecked() const noexcept {
        return activation() == 1.f;
    }

    std::optional<int> LedState::toHue(QColor const& color) noexcept {
        if(color.hslSaturation() == 0) {
            return std::nullopt;
        } else {
            return color.hue();
        }
    }

    std::optional<int> LedState::normalized(std::optional<int> const hue) noexcept {
        if(hue) {
            return cyclic_adapt(*hue, 0, 360);
        } else {
            return std::nullopt;
        }
    }

    Led::Led(QWidget* const parent) noexcept
        : Led(LedState{}, parent) {}

    Led::Led(LedState const& state, QWidget* const parent) noexcept
        : QWidget(parent)
        , state_(state) {
        setAutoFillBackground(true);
    }

    LedState Led::state() const noexcept {
        return state_;
    }

    void Led::setState(LedState const& state) noexcept {
        state_ = state;
        stateChanged(state_);
        update();
    }

    QSize Led::sizeHint() const {
        auto const s = QFontMetrics{{}}.height();
        return {s, s};
    }

    void Led::paintEvent(QPaintEvent*) {
        QPainter painter(this);
        drawLed(painter, rect(), state_.hue(), state_.activation(), isEnabled());
    }

    void Led::changeEvent(QEvent* event) {
        switch(event->type()) {
            case QEvent::EnabledChange:
                update();
                break;
        }
    }


}
