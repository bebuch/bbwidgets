#include "../include/bbwidgets/LedDualState.hpp"

namespace bbwidgets {


    using namespace std::literals;

    LedAnimated::LedAnimated(LedStyle const& style, QWidget* const parent) noexcept
        : Led(style, parent) {}

    void LedAnimated::setAnimationDuration(std::chrono::milliseconds const duration) noexcept {
        duration_ = duration;
        if(animation_) {
            animation_->setDuration(duration_.count());
        }
    }

    void LedAnimated::setAnimationEasingCurve(QEasingCurve const easing_curve) noexcept {
        easing_curve_ = easing_curve;
        if(animation_) {
            animation_->setEasingCurve(easing_curve_);
        }
    }

    std::chrono::milliseconds LedAnimated::animationDuration() const noexcept {
        return duration_;
    }

    QEasingCurve LedAnimated::animationEasingCurve() const noexcept {
        return easing_curve_;
    }

    void LedAnimated::startAnimation(LedStyle const& target) noexcept {
        if(duration_ == 0ms) {
            setStyle(target);
            return;
        }

        if(animation_) {
            animation_->stop();
        }

        animation_ = new QVariantAnimation(this);
        animation_->setDuration(duration_.count());
        animation_->setEasingCurve(easing_curve_);
        animation_->setStartValue(QVariant::fromValue(style()));
        animation_->setEndValue(QVariant::fromValue(target));
        animation_->start(QAbstractAnimation::DeleteWhenStopped);

        connect(animation_, &QVariantAnimation::valueChanged, [this](QVariant const& style) {
            setStyle(qvariant_cast<LedStyle>(style));
        });
    }


}
