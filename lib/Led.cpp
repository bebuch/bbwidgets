#include "../include/bbwidgets/Led.hpp"

#include <QEvent>
#include <QFontMetrics>
#include <QPainter>

namespace bbwidgets {


    Led::Led(QWidget* const parent) noexcept
        : Led(LedStyle{}, parent) {}

    Led::Led(LedStyle const& state, QWidget* const parent) noexcept
        : QWidget(parent)
        , style_(state) {
        setAutoFillBackground(true);
    }

    LedStyle Led::style() const noexcept {
        return style_;
    }

    void Led::setStyle(LedStyle const& style) noexcept {
        style_ = style;
        update();
    }

    Qt::Alignment Led::alignment() const noexcept {
        return alignment_;
    }

    void Led::setAlignment(Qt::Alignment const flag) noexcept {
        alignment_ = flag;
        update();
    }

    QSize Led::sizeHint() const {
        auto const s = QFontMetrics{{}}.height();
        return {s, s};
    }

    void Led::paintEvent(QPaintEvent*) {
        QPainter painter(this);
        auto const [w, h] = QSizeF(size());
        auto const s = std::min(w, h);
        auto const x = [this, w, s] {
            if(alignment_ & Qt::AlignLeft) {
                return qreal(0);
            } else if(alignment_ & Qt::AlignRight) {
                return w - s;
            } else {
                return w / 2 - s / 2;
            }
        }();
        auto const y = [this, h, s] {
            if(alignment_ & Qt::AlignTop) {
                return qreal(0);
            } else if(alignment_ & Qt::AlignBottom) {
                return h - s;
            } else {
                return h / 2 - s / 2;
            }
        }();
        auto const square = QRectF{x, y, s, s};
        style_.draw(painter, square, isEnabled());
    }

    void Led::changeEvent(QEvent* event) {
        switch(event->type()) {
            case QEvent::EnabledChange:
                update();
                break;
        }
    }


}
