#pragma once

#include "LedAnimated.hpp"

namespace bbwidgets {


    class DLLEXPORT LedDualState: public LedAnimated {
        Q_OBJECT
        Q_PROPERTY(bool checked READ isChecked WRITE setChecked)

    public:
        LedDualState(QWidget* parent = nullptr) noexcept;
        LedDualState(std::optional<int> on_hue, QWidget* parent = nullptr) noexcept;
        LedDualState(std::optional<int> on_hue, std::optional<int> off_hue, QWidget* parent = nullptr) noexcept;
        LedDualState(LedDualState const&) = delete;

        LedDualState& operator=(LedDualState const&) = delete;

        [[nodiscard]] bool isChecked() const noexcept;

        void setChecked(bool checked) noexcept;
        void setOnHue(std::optional<int> hue) noexcept;
        void setOffHue(std::optional<int> hue) noexcept;

    signals:
        void stateChanged(bool checked);

    private:
        bool checked_;
        LedStyle on_style_;
        LedStyle off_style_;
    };


}
