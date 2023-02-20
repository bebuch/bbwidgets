#pragma once

#include "globals.hpp"

#include <QWidget>

#include <optional>

namespace bbwidgets {


    DLLEXPORT void drawLed(
        QPainter& painter, QRectF const& pos, std::optional<int> hue, float activation, bool enabled) noexcept;

    class DLLEXPORT LedState {
    public:
        LedState(std::optional<int> hue = std::nullopt, bool checked = false) noexcept;
        LedState(int hue, float saturation, float activation, bool checked) noexcept;
        LedState(LedState const&) noexcept;

        ~LedState();

        LedState& operator=(LedState const&) noexcept;

        void setHue(std::optional<int> hue) noexcept;
        void setChecked(bool checked) noexcept;

        [[nodiscard]] bool isChecked() const noexcept;

        void draw(QPainter& painter, QRectF const& pos, bool enabled) const noexcept;

        [[nodiscard]] LedState lerp(LedState const& b, float t) const noexcept;

    private:
        int hue_;
        float saturation_;
        float activation_;
        bool checked_;
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
