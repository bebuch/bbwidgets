#include <QApplication>
#include <QVBoxLayout>

#include <bbwidgets/led.hpp>

#include <ranges>


int main(int argc, char** argv){
    QApplication app(argc, argv);

    QWidget window;

    auto const main_layout = new QVBoxLayout{};
    window.setLayout(main_layout);

    auto const count = 12;
    auto const hls_hues = std::views::iota(0, count)
        | std::views::transform([count](int const i) { return static_cast<int>(std::round(360.f / count * i)); });
    auto const checks = { true, false };
    auto const enables = { true, false };

    for (auto check : checks) {
        for (auto enable : enables) {
            auto const layout = new QHBoxLayout{};
            main_layout->addLayout(layout);

            layout->addWidget(new bbwidgets::Led{ {std::nullopt, check, enable} });
            for (auto const hue : hls_hues) {
                layout->addWidget(new bbwidgets::Led{ {hue, check, enable} });
            }
        }
    }

    window.show();

    return app.exec();
}
