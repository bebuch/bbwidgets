#pragma once

#include "LedStyle.hpp"

#include <QFrame>

namespace bbwidgets {


    class DLLEXPORT Led: public QFrame {
        Q_OBJECT
        Q_PROPERTY(Qt::Alignment alignment READ alignment WRITE setAlignment)

    public:
        Led(QWidget* parent = nullptr) noexcept;
        Led(LedStyle const& style, QWidget* parent = nullptr) noexcept;
        Led(Led const&) = delete;

        Led& operator=(Led const&) = delete;

        [[nodiscard]] LedStyle style() const noexcept;
        void setStyle(LedStyle const& style) noexcept;

        [[nodiscard]] Qt::Alignment alignment() const noexcept;
        void setAlignment(Qt::Alignment flag) noexcept;

    protected:
        [[nodiscard]] QSize sizeHint() const override;
        void paintEvent(QPaintEvent* event) override;
        void changeEvent(QEvent* event) override;

    private:
        LedStyle style_;
        Qt::Alignment alignment_;
    };


}
