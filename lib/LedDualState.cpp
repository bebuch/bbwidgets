#include "../include/bbwidgets/LedDualState.hpp"

namespace bbwidgets {


    LedDualState::LedDualState(QWidget* const parent) noexcept
        : LedDualState(std::nullopt, parent) {}

    LedDualState::LedDualState(std::optional<int> const on_hue, QWidget* const parent) noexcept
        : LedDualState(on_hue, on_hue, parent) {}

    LedDualState::LedDualState(
        std::optional<int> const on_hue, std::optional<int> const off_hue, QWidget* const parent) noexcept
        : LedAnimated(off_hue, parent)
        , checked_(false)
        , on_style_({on_hue, true})
        , off_style_({off_hue, false}) {}

    bool LedDualState::isChecked() const noexcept {
        return checked_;
    }

    void LedDualState::setChecked(bool const checked) noexcept {
        if(checked_ == checked) {
            return;
        }

        checked_ = checked;
        startAnimation(checked_ ? on_style_ : off_style_);
    }

    void LedDualState::setOnHue(std::optional<int> const hue) noexcept {
        on_style_ = {hue, true};
        if(checked_) {
            startAnimation(on_style_);
        }
    }

    void LedDualState::setOffHue(std::optional<int> const hue) noexcept {
        off_style_ = {hue, false};
        if(!checked_) {
            startAnimation(off_style_);
        }
    }


}
