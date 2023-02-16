#include <QApplication>
#include <QVBoxLayout>

#include <bbwidgets/led.hpp>


int main(int argc, char** argv){
    QApplication app(argc, argv);

    QWidget window;

    auto const layout = new QVBoxLayout{};
    window.setLayout(layout);

    layout->addWidget(new bbwidgets::Led{});

    window.show();

    return app.exec();
}
