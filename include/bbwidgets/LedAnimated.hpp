#pragma once

#include "Led.hpp"

#include <QPointer>
#include <QVariantAnimation>

#include <chrono>

namespace bbwidgets {


    class DLLEXPORT LedAnimated: public Led {
        Q_OBJECT

    public:
        LedAnimated(LedStyle const& style, QWidget* parent) noexcept;

        void setAnimationDuration(std::chrono::milliseconds duration) noexcept;
        void setAnimationEasingCurve(QEasingCurve easing_curve) noexcept;

        std::chrono::milliseconds animationDuration() const noexcept;
        QEasingCurve animationEasingCurve() const noexcept;

    protected:
        void startAnimation(LedStyle const& target) noexcept;

    private:
        QPointer<QVariantAnimation> animation_ = nullptr;
        std::chrono::milliseconds duration_ = {};
        QEasingCurve easing_curve_ = {};
    };


}
