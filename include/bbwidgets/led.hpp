#pragma once

#include "globals.hpp"

#include <QWidget>

#include <optional>


namespace bbwidgets {


    class DLLEXPORT LedState {
    public:
        LedState(std::optional<int> hue = std::nullopt, bool checked = false, bool enabled = true);
        LedState(int hue, bool checked = false, bool enabled = true);
        LedState(QColor const& color, bool checked = false, bool enabled = true);
        LedState(Qt::GlobalColor color, bool checked = false, bool enabled = true);

        void unsetHue();
        void setHue(std::optional<int> hue);
        void setHueBy(QColor const& color);

        void setChecked(bool checked);
        void setEnabled(bool enabled);

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
        bool checked_;
    };


}
