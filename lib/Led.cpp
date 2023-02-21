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

    QSize Led::sizeHint() const {
        auto const s = QFontMetrics{{}}.height();
        return {s, s};
    }

    void Led::paintEvent(QPaintEvent*) {
        QPainter painter(this);
        style_.draw(painter, rect(), isEnabled());
    }

    void Led::changeEvent(QEvent* event) {
        switch(event->type()) {
            case QEvent::EnabledChange:
                update();
                break;
        }
    }


}
