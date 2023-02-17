#pragma once

#include <QWidget>

#include <optional>

#ifdef SHARED_LIBRARY
#define DLLEXPORT Q_DECL_EXPORT
#else
#define DLLEXPORT Q_DECL_IMPORT
#endif


namespace bbwidgets {


    class DLLEXPORT LedState {
    public:
        LedState(std::optional<float> hls_hue = std::nullopt, bool state = false, bool enabled = true);
        LedState(float hls_hue, bool state = false, bool enabled = true);
        LedState(QColor const& color, bool state = false, bool enabled = true);
        LedState(Qt::GlobalColor color, bool state = false, bool enabled = true);

        std::optional<float> hslHueF() const;
        void unsetHslHueF();
        void setHslHueF(std::optional<float> hue);

        QColor color() const;
        void unsetColor();
        void setColor(QColor const& color);

        static std::optional<float> toHlsHueF(QColor const& color);

    private:
        std::optional<float> hsl_hue_;
        bool state_;
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
        void stateChanged(LedState const& state);

    protected:
        QSize sizeHint() const override;
        void paintEvent(QPaintEvent* event) override;

    private:
        LedState state_;
    };


}
