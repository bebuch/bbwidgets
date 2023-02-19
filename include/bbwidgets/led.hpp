#pragma once

#include "globals.hpp"

#include <QWidget>

#include <optional>


namespace bbwidgets {


    DLLEXPORT void drawLed(
        QPainter& painter,
        QRectF const& pos,
        std::optional<int> hue,
        float activation,
        bool enabled) noexcept;

    class DLLEXPORT LedState {
    public:
        LedState(std::optional<int> hue = std::nullopt, bool checked = false, bool enabled = true) noexcept;
        LedState(int hue, bool checked = false, bool enabled = true) noexcept;
        LedState(QColor const& color, bool checked = false, bool enabled = true) noexcept;
        LedState(Qt::GlobalColor color, bool checked = false, bool enabled = true) noexcept;

        void unsetHue() noexcept;
        void setHue(std::optional<int> hue) noexcept;
        void setHueBy(QColor const& color) noexcept;

        void setChecked(bool checked) noexcept;
        void setEnabled(bool enabled) noexcept;

        [[nodiscard]] std::optional<int> hue() const noexcept;
        [[nodiscard]] bool isChecked() const noexcept;
        [[nodiscard]] bool isEnabled() const noexcept;

        static [[nodiscard]] std::optional<int> toHue(QColor const& color) noexcept;
        static [[nodiscard]] std::optional<int> normalized(std::optional<int> const hue) noexcept;

    private:
        std::optional<int> hue_;
        bool checked_;
        bool enabled_;
    };

    class DLLEXPORT Led : public QWidget {
        Q_OBJECT

    public:
        Led(QWidget* parent = nullptr) noexcept;
        Led(LedState const& state, QWidget* parent = nullptr) noexcept;

        [[nodiscard]] LedState state() const noexcept;
        void setState(LedState const& state) noexcept;

    signals:
        void stateChanged();

    protected:
        [[nodiscard]] QSize sizeHint() const override;
        void paintEvent(QPaintEvent* event) override;
        void changeEvent(QEvent* event) override;

    private:
        std::optional<int> hue_;
        bool checked_;
    };


}
