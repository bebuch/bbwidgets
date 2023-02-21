#include <bbwidgets/led.hpp>

#include <QApplication>
#include <QPropertyAnimation>
#include <QVBoxLayout>

#include <ranges>

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    QWidget window;

    auto const main_layout = new QVBoxLayout{};
    window.setLayout(main_layout);

    auto const count = 6;
    std::vector<std::optional<int>> hues{std::nullopt};
    std::ranges::copy(std::views::iota(0, count) | std::views::transform([count](int const i) {
        return static_cast<int>(std::round(360.f / count * i));
    }),
        std::back_inserter(hues));
    auto const checks = {false, true};
    auto const enables = {true, false};

    for(auto enable: enables) {
        for(auto check: checks) {
            auto const layout = new QHBoxLayout{};
            main_layout->addLayout(layout);

            for(auto const hue: hues) {
                layout->addWidget([hue, enable, check] {
                    auto led = new bbwidgets::Led{
                        {hue, check}
                    };
                    led->setEnabled(enable);

                    QPropertyAnimation* anim = new QPropertyAnimation(led, "style", led);
                    anim->setDuration(1000);
                    anim->setEasingCurve(QEasingCurve::InOutQuart);
                    anim->setStartValue(QVariant::fromValue(bbwidgets::LedStyle{hue, check}));
                    anim->setEndValue(QVariant::fromValue(bbwidgets::LedStyle{hue ? *hue + 360 / count : 120, !check}));
                    anim->start();

                    anim->connect(anim, &QPropertyAnimation::finished, [anim] {
                        anim->setDirection(QPropertyAnimation::Direction(!anim->direction()));
                        anim->start();
                    });

                    return led;
                }());
            }
        }
    }

    window.show();

    return app.exec();
}
