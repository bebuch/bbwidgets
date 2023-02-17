#include <QApplication>
#include <QVBoxLayout>

#include <bbwidgets/led.hpp>


int main(int argc, char** argv){
    QApplication app(argc, argv);

    QWidget window;

    auto const layout = new QVBoxLayout{};
    window.setLayout(layout);

    layout->addWidget(new bbwidgets::Led{});
    layout->addWidget(new bbwidgets::Led{ QColor(Qt::green) });
    layout->addWidget(new bbwidgets::Led{ QColor(0, 255, 0) });
    layout->addWidget(new bbwidgets::Led{ Qt::green });
    layout->addWidget(new bbwidgets::Led{ Qt::red });
    layout->addWidget(new bbwidgets::Led{ Qt::blue });
    layout->addWidget(new bbwidgets::Led{ Qt::yellow });

    window.show();

    return app.exec();
}
