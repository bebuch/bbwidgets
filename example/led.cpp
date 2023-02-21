#include <bbwidgets/LedDualState.hpp>

#include <QApplication>
#include <QPushButton>
#include <QVBoxLayout>

#include <ranges>

int main(int argc, char** argv) {
    using namespace std::literals;

    QApplication app(argc, argv);

    QWidget window;

    auto const main_layout = new QVBoxLayout{};
    window.setLayout(main_layout);

    auto const change_enable = new QPushButton("enable");
    main_layout->addWidget(change_enable);

    auto const change_state = new QPushButton("state");
    main_layout->addWidget(change_state);

    auto const count = 6;
    std::vector<std::optional<int>> hues{std::nullopt};
    std::ranges::copy(std::views::iota(0, count) | std::views::transform([count](int const i) {
        return static_cast<int>(std::round(360.f / count * i));
    }),
        std::back_inserter(hues));
    auto const checks = {false, true};
    auto const enables = {true, false};

    for(auto check: checks) {
        for(auto enable: enables) {
            auto const layout = new QHBoxLayout{};
            main_layout->addLayout(layout);

            for(auto const hue: hues) {
                layout->addWidget([change_enable, change_state, hue, enable, check] {
                    auto led = new bbwidgets::LedDualState{hue};
                    led->setEnabled(enable);
                    led->setChecked(check);
                    led->setAnimationDuration(1000ms);
                    led->setAnimationEasingCurve(QEasingCurve::InOutQuart);

                    change_enable->connect(change_enable, &QPushButton::clicked, [led, enable]() mutable {
                        enable = !enable;
                        led->setEnabled(enable);
                    });

                    change_state->connect(change_state, &QPushButton::clicked, [led, check]() mutable {
                        check = !check;
                        led->setChecked(check);
                    });

                    return led;
                }());
            }
        }
    }

    window.show();

    return app.exec();
}
