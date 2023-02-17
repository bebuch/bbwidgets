#pragma once

#include <QWidget>

#include <optional>

#include "globals.hpp"


namespace bbwidgets {


    class DLLEXPORT LedState {
    public:
        LedState(std::optional<int> hue = std::nullopt, bool state = false, bool enabled = true);
        LedState(int hue, bool state = false, bool enabled = true);
        LedState(QColor const& color, bool state = false, bool enabled = true);
        LedState(Qt::GlobalColor color, bool state = false, bool enabled = true);

        void unsetHue();
        void setHue(std::optional<int> hue);
        void setHueBy(QColor const& color);

        std::optional<int> hue() const;
        bool isChecked() const;
        bool isEnabled() const;

        static std::optional<int> toHue(QColor const& color);
        static std::optional<int> normalized(std::optional<int> const hue);

    private:
        std::optional<int> hue_;
        bool checked_;
        bool enabled_;
    };

    class DLLEXPORT Led : public QWidget {
        Q_OBJECT

    public:
        Led(QWidget* parent = nullptr);
        Led(LedState const& state, QWidget* parent = nullptr);

        LedState state() const;
        void setState(LedState const& state);

    signals:
        void stateChanged();

    protected:
        QSize sizeHint() const override;
        void paintEvent(QPaintEvent* event) override;
        void changeEvent(QEvent* event) override;

    private:
        std::optional<int> hue_;
        bool check_;
    };


}
