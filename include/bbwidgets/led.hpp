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
        // Q_PROPERTY(QColor color MEMBER color_ NOTIFY colorChanged)

    public:
        Led(QWidget* parent = nullptr);

    protected:
        void paintEvent(QPaintEvent* event) override;

    private:

    };


}
