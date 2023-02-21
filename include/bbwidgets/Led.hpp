#pragma once

#include "LedStyle.hpp"

#include <QWidget>

namespace bbwidgets {


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
