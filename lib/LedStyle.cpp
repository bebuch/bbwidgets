#include "../include/bbwidgets/LedStyle.hpp"

#include <QEvent>
#include <QFontMetrics>
#include <QPainter>
#include <QVariantAnimation>

#include <algorithm>
#include <cmath>
#include <numeric>

namespace bbwidgets {


    static [[nodiscard]] int normalizedHue(int const hue) noexcept {
        return cyclic_adapt(hue, 0, 360);
    }

    static [[nodiscard]] QVariant ledStateInterpolator(
        LedStyle const& start, LedStyle const& end, qreal const progress) {
        return QVariant::fromValue(start.lerp(end, static_cast<float>(progress)));
    }

    void LedStyle::draw(QPainter& painter, QRectF const& rect, bool const enabled) const noexcept {
        auto const [w, h] = rect.size();

        auto const basic_color = [this, enabled] {
            auto const lightness = std::lerp(.2f, .4f, activation_);
            auto const saturation = enabled ? saturation_ : std::lerp(.0f, .15f, saturation_);
            return QColor::fromHslF(hue_ / 360.f, saturation, lightness);
        }();
        auto const border_color = basic_color.darker(150);

        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(Qt::NoPen);
        painter.setBrush(border_color);
        painter.drawEllipse(rect);

        auto const basic_rect = rect.adjusted(w * 0.1, h * 0.1, w * -0.1, h * -0.1);

        auto const basic_from = rect.topLeft() + QPoint(0, h * 0.1);
        auto const basic_to = rect.bottomLeft();

        auto basic_gradient = QLinearGradient(basic_from, basic_to);
        auto const lighter = enabled ? 150 : std::lerp(250, 150, saturation_);
        basic_gradient.setColorAt(0.0, basic_color.lighter(std::lerp(lighter, lighter - 50, activation_)));
        basic_gradient.setColorAt(1.0, basic_color.lighter(std::lerp(lighter, lighter + 70, activation_)));
        painter.setBrush(basic_gradient);
        painter.drawEllipse(basic_rect);

        if(activation_ > 0.f) {
            auto const fgr = basic_rect.adjusted(w * 0.1, h * 0.1, w * -0.1, h * -0.3);
            auto const glare_rect = fgr.adjusted(std::lerp(.3f, 0.f, activation_) * fgr.width(),
                std::lerp(1.f / 3, 0.f, activation_) * fgr.height(), std::lerp(.3f, 0.f, activation_) * -fgr.width(),
                std::lerp(2.f / 3, 0.f, activation_) * -fgr.height());

            auto glare_gradient = QLinearGradient(basic_from, basic_to);
            glare_gradient.setColorAt(0.0, basic_color.lighter(std::lerp(lighter, lighter + 130, activation_)));
            glare_gradient.setColorAt(1.0, basic_color.lighter(std::lerp(lighter, lighter + 30, activation_)));
            painter.setBrush(glare_gradient);
            painter.drawEllipse(glare_rect);
        }
    }

    LedStyle::LedStyle(int const hue, float const saturation, float const activation) noexcept
        : hue_(normalizedHue(hue))
        , saturation_(saturation)
        , activation_(activation) {
        // call qRegisterAnimationInterpolator on first element construction
        static [[maybe_unused]] auto const animation_registered = [] {
            qRegisterAnimationInterpolator<LedStyle>(ledStateInterpolator);
            return true;
        }();
    }

    LedStyle LedStyle::lerp(LedStyle const& b, float const t) const noexcept {
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

        return LedStyle(hue, saturation, activation);
    }

    LedStyle::LedStyle(std::optional<int> const hue, bool const checked) noexcept
        : LedStyle(hue ? *hue : 0, hue ? 1.f : 0.f, checked ? 1.f : 0.f) {}

    LedStyle::LedStyle(LedStyle const&) noexcept = default;

    LedStyle::~LedStyle() = default;

    LedStyle& LedStyle::operator=(LedStyle const&) noexcept = default;


}

Q_DECLARE_METATYPE(bbwidgets::LedStyle);
