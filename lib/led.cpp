#include "../include/bbwidgets/led.hpp"

#include <QEvent>
#include <QFontMetrics>
#include <QPainter>
#include <QVariantAnimation>

#include <algorithm>
#include <cmath>
#include <numeric>

namespace bbwidgets {


    static int normalizedHue(int const hue) noexcept {
        return cyclic_adapt(hue, 0, 360);
    }

    static QVariant ledStateInterpolator(LedState const& start, LedState const& end, qreal const progress) {
        return QVariant::fromValue(start.lerp(end, static_cast<float>(progress)));
    }

    void LedState::draw(QPainter& painter, QRectF const& pos, bool const enabled) const noexcept {
        auto const [w, h] = pos.size();
        auto const s = std::min(w, h);
        auto const border_rect = QRectF(w / 2 - s / 2, h / 2 - s / 2, s, s);

        auto const basic_color = [this, enabled] {
            auto const lightness
                = std::lerp(std::lerp(.6f, .2f, saturation_), std::lerp(.8f, .4f, saturation_), activation_);
            auto const saturation = enabled ? saturation_ : std::lerp(.0f, .15f, saturation_);
            return QColor::fromHslF(hue_ / 360.f, saturation, lightness);
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
        basic_gradient.setColorAt(0.0, basic_color.lighter(std::lerp(base_color_light, 100, activation_)));
        basic_gradient.setColorAt(1.0, basic_color.lighter(std::lerp(base_color_light, 220, activation_)));
        painter.setBrush(basic_gradient);
        painter.drawEllipse(basic_rect);

        if(activation_ > 0.f) {
            auto const fgr = basic_rect.adjusted(s * 0.1, s * 0.1, s * -0.1, s * -0.3);
            auto const glare_rect = fgr.adjusted(std::lerp(.3f, 0.f, activation_) * fgr.width(),
                std::lerp(1.f / 3, 0.f, activation_) * fgr.height(), std::lerp(.3f, 0.f, activation_) * -fgr.width(),
                std::lerp(2.f / 3, 0.f, activation_) * -fgr.height());

            auto glare_gradient = QLinearGradient(basic_from, basic_to);
            glare_gradient.setColorAt(0.0, basic_color.lighter(std::lerp(base_color_light, 280, activation_)));
            glare_gradient.setColorAt(1.0, basic_color.lighter(std::lerp(base_color_light, 180, activation_)));
            painter.setBrush(glare_gradient);
            painter.drawEllipse(glare_rect);
        }
    }

    LedState::LedState(int const hue, float const saturation, float const activation, bool const checked) noexcept
        : hue_(normalizedHue(hue))
        , saturation_(saturation)
        , activation_(activation)
        , checked_(checked) {
        // we need to call qRegisterAnimationInterpolator on first element construction
        static [[maybe_unused]] auto const animation_registered = [] {
            qRegisterAnimationInterpolator<LedState>(ledStateInterpolator);
            return true;
        }();
    }

    LedState LedState::lerp(LedState const& b, float const t) const noexcept {
        auto const hue = [&] {
            auto const a_colorfull = saturation_ > 0;
            auto const b_colorfull = b.saturation_ > 0;

            if(a_colorfull && b_colorfull) {
                if(std::abs(hue_ - b.hue_) < 180) {
                    auto s_hue = static_cast<float>(hue_);
                    auto e_hue = static_cast<float>(b.hue_);
                    return static_cast<int>(std::round(std::lerp(s_hue, e_hue, t)));
                } else {
                    auto s_hue = static_cast<float>(hue_ + (hue_ < b.hue_ ? 360 : 0));
                    auto e_hue = static_cast<float>(b.hue_ + (hue_ < b.hue_ ? 0 : 360));
                    return normalizedHue(static_cast<int>(std::round(std::lerp(s_hue, e_hue, t))));
                }
            }

            if(a_colorfull) {
                return hue_;
            }

            if(b_colorfull) {
                return b.hue_;
            }

            return 0;
        }();

        auto const saturation = std::lerp(saturation_, b.saturation_, t);
        auto const activation = std::lerp(activation_, b.activation_, t);

        return LedState(hue, saturation, activation, b.checked_);
    }

    LedState::LedState(std::optional<int> const hue, bool const checked) noexcept
        : LedState(hue ? *hue : 0, hue ? 1.f : 0.f, checked ? 1.f : 0.f, checked) {}

    LedState::LedState(LedState const&) noexcept = default;

    LedState::~LedState() = default;

    LedState& LedState::operator=(LedState const&) noexcept = default;

    void LedState::setHue(std::optional<int> const hue) noexcept {
        hue_ = hue ? normalizedHue(*hue) : 0;
        saturation_ = hue ? 1.f : 0.f;
    }

    void LedState::setChecked(bool const checked) noexcept {
        activation_ = checked ? 1.f : 0.f;
        checked_ = checked;
    }

    bool LedState::isChecked() const noexcept {
        return checked_;
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
        state_.draw(painter, rect(), isEnabled());
    }

    void Led::changeEvent(QEvent* event) {
        switch(event->type()) {
            case QEvent::EnabledChange:
                update();
                break;
        }
    }


}
