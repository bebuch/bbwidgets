#pragma once

#include <QWidget>

#include <optional>

#ifdef SHARED_LIBRARY
#define DLLEXPORT Q_DECL_EXPORT
#else
#define DLLEXPORT Q_DECL_IMPORT
#endif


namespace bbwidgets {


    class DLLEXPORT Led : public QWidget {
        Q_OBJECT

    public:
        Led(QWidget* parent = nullptr);
        Led(float hsl_hue, QWidget* parent = nullptr);
        Led(Qt::GlobalColor color, QWidget* parent = nullptr);
        Led(QColor const& color, QWidget* parent = nullptr);

        std::optional<float> hslHueF() const;
        void unsetHslHueF();
        void setHslHueF(float hue);

        QColor color() const;
        void unsetColor();
        void setColor(QColor const& color);

    signals:
        void hslHueFChanged(std::optional<float> hue);

    protected:
        QSize sizeHint() const override;
        void paintEvent(QPaintEvent* event) override;

    private:
        std::optional<float> hsl_hue_;
    };


}
