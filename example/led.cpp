#include <QApplication>
#include <QVBoxLayout>

#include <bbwidgets/led.hpp>

#include <ranges>


int main(int argc, char** argv){
    QApplication app(argc, argv);

    QWidget window;

    auto const layout = new QVBoxLayout{};
    window.setLayout(layout);

    auto const count = 12;
    for (auto const i : std::views::iota(0, count - 1)) {
        layout->addWidget(new bbwidgets::Led{1.f / count * i});
    }

    window.show();

    return app.exec();
}
