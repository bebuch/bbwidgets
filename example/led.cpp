#include <bbwidgets/led.hpp>

#include <QApplication>
#include <QVBoxLayout>

#include <ranges>

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    QWidget window;

    auto const main_layout = new QVBoxLayout{};
    window.setLayout(main_layout);

    auto const count = 12;
    auto const hls_hues = std::views::iota(0, count) | std::views::transform([count](int const i) {
        return static_cast<int>(std::round(360.f / count * i));
    });
    auto const checks = {true, false};
    auto const enables = {true, false};

    for(auto enable: enables) {
        for(auto check: checks) {
            auto const layout = new QHBoxLayout{};
            main_layout->addLayout(layout);

            layout->addWidget([enable, check] {
                auto led = new bbwidgets::Led{
                    {std::nullopt, check}
                };
                led->setEnabled(enable);
                return led;
            }());
            for(auto const hue: hls_hues) {
                layout->addWidget([hue, enable, check] {
                    auto led = new bbwidgets::Led{
                        {hue, check}
                    };
                    led->setEnabled(enable);
                    return led;
                }());
            }
        }
    }

    window.show();

    return app.exec();
}
