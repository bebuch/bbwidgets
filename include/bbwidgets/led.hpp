#pragma once

#include <QWidget>

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

        float hslHueF() const;
        void setHslHueF(float hue);

        QColor color() const;
        void setColor(QColor const& color);

    signals:
        void hslHueFChanged(float hue);

    protected:
        QSize sizeHint() const override;
        void paintEvent(QPaintEvent* event) override;

    private:
        float hsl_hue_;
    };


}
