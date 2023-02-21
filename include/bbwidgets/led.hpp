#pragma once

#include "globals.hpp"

#include <QWidget>

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

    class DLLEXPORT Led: public QWidget {
        Q_OBJECT
        Q_PROPERTY(LedStyle style READ style WRITE setStyle)

    public:
        Led(QWidget* parent = nullptr) noexcept;
        Led(LedStyle const& style, QWidget* parent = nullptr) noexcept;

        [[nodiscard]] LedStyle style() const noexcept;
        void setStyle(LedStyle const& style) noexcept;

    protected:
        [[nodiscard]] QSize sizeHint() const override;
        void paintEvent(QPaintEvent* event) override;
        void changeEvent(QEvent* event) override;

    private:
        LedStyle style_;
    };


}

Q_DECLARE_METATYPE(bbwidgets::LedStyle);
