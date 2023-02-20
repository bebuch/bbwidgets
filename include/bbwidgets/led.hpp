#pragma once

#include "globals.hpp"

#include <QWidget>

#include <optional>

namespace bbwidgets {


    DLLEXPORT void drawLed(
        QPainter& painter, QRectF const& pos, std::optional<int> hue, float activation, bool enabled) noexcept;

    class DLLEXPORT LedState {
    public:
        LedState(std::optional<int> hue, float activation) noexcept;
        LedState(std::optional<int> hue = std::nullopt, bool checked = false) noexcept;
        LedState(int hue, float activation) noexcept;
        LedState(int hue, bool checked = false) noexcept;
        LedState(QColor const& color, float activation) noexcept;
        LedState(QColor const& color, bool checked = false) noexcept;
        LedState(Qt::GlobalColor color, float activation) noexcept;
        LedState(Qt::GlobalColor color, bool checked = false) noexcept;
        LedState(LedState const&) noexcept;

        ~LedState();

        LedState& operator=(LedState const&) noexcept;

        void unsetHue() noexcept;
        void setHue(std::optional<int> hue) noexcept;
        void setHueBy(QColor const& color) noexcept;

        void setActivation(float activation) noexcept;
        void setChecked(bool checked) noexcept;

        [[nodiscard]] std::optional<int> hue() const noexcept;
        [[nodiscard]] float activation() const noexcept;
        [[nodiscard]] bool isChecked() const noexcept;

        static [[nodiscard]] std::optional<int> toHue(QColor const& color) noexcept;
        static [[nodiscard]] std::optional<int> normalized(std::optional<int> const hue) noexcept;

    private:
        std::optional<int> hue_;
        float activation_;
    };

    class DLLEXPORT Led: public QWidget {
        Q_OBJECT
        Q_PROPERTY(LedState state READ state WRITE setState NOTIFY stateChanged)

    public:
        Led(QWidget* parent = nullptr) noexcept;
        Led(LedState const& state, QWidget* parent = nullptr) noexcept;

        [[nodiscard]] LedState state() const noexcept;
        void setState(LedState const& state) noexcept;

    signals:
        void stateChanged(LedState const& state);

    protected:
        [[nodiscard]] QSize sizeHint() const override;
        void paintEvent(QPaintEvent* event) override;
        void changeEvent(QEvent* event) override;

    private:
        LedState state_;
    };


}

Q_DECLARE_METATYPE(bbwidgets::LedState);
