#pragma once

#include "globals.hpp"

#include <QPainter>

#include <optional>

namespace bbwidgets {


    class DLLEXPORT LedStyle {
    public:
        LedStyle(std::optional<int> hue = std::nullopt, bool checked = false) noexcept;
        LedStyle(int hue, float saturation, float activation) noexcept;
        LedStyle(LedStyle const&) noexcept;

        ~LedStyle();

        LedStyle& operator=(LedStyle const&) noexcept;

        void draw(QPainter& painter, QRectF const& pos, bool enabled) const noexcept;

        [[nodiscard]] LedStyle lerp(LedStyle const& b, float t) const noexcept;

    private:
        int hue_;
        float saturation_;
        float activation_;
    };


}
