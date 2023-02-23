#include "../include/bbwidgets/Led.hpp"

#include <QEvent>
#include <QFontMetrics>
#include <QPainter>

namespace bbwidgets {


    Led::Led(QWidget* const parent) noexcept
        : Led(LedStyle{}, parent) {}

    Led::Led(LedStyle const& state, QWidget* const parent) noexcept
        : QFrame(parent)
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

    void Led::paintEvent(QPaintEvent* const event) {
        QFrame::paintEvent(event);

        QPainter painter(this);
        auto const content_rect = QRectF(contentsRect());
        if(!content_rect.isValid()) {
            return;
        }

        auto const [x, y] = content_rect.topLeft();
        auto const [w, h] = content_rect.size();
        auto const s = std::min(w, h);
        auto const sx = [this, w, s] {
            if(alignment_ & Qt::AlignLeft) {
                return qreal(0);
            } else if(alignment_ & Qt::AlignRight) {
                return w - s;
            } else {
                return w / 2 - s / 2;
            }
        }();
        auto const sy = [this, h, s] {
            if(alignment_ & Qt::AlignTop) {
                return qreal(0);
            } else if(alignment_ & Qt::AlignBottom) {
                return h - s;
            } else {
                return h / 2 - s / 2;
            }
        }();
        auto const square = QRectF{sx + x, sy + y, s, s};
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
