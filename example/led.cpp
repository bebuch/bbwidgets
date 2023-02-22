#include <bbwidgets/LedDualState.hpp>
#include <bbwidgets/LedMultiState.hpp>

#include <QApplication>
#include <QMap>
#include <QPushButton>
#include <QVBoxLayout>

#include <ranges>
#include <utility>

using namespace std::literals;

enum class State { Off, Red, Yellow, Green };

[[nodiscard]] State next(State const state) noexcept {
    if(state == State::Green) {
        return State{0};
    } else {
        auto const value = std::underlying_type_t<State>(state);
        return State{value + 1};
    }
}

[[nodiscard]] int next(int const state) noexcept {
    return bbwidgets::cyclic_adapt(state + 1, 0, 4);
}

template <typename State>
QWidget* makeLedMultiState(
    QPushButton* const change_enable, QPushButton* const change_state, std::map<State, bbwidgets::LedStyle> state_map) {
    auto const led = new bbwidgets::LedMultiState{std::move(state_map)};
    led->setEnabled(true);
    led->setAnimationDuration(1000ms);
    led->setAnimationEasingCurve(QEasingCurve::InOutQuart);

    change_enable->connect(change_enable, &QPushButton::clicked, [led, enable = led->isEnabled()]() mutable {
        enable = !enable;
        led->setEnabled(enable);
    });

    change_state->connect(change_state, &QPushButton::clicked, [led, state = led->state()]() mutable {
        state = next(state);
        led->setState(state);
    });

    return led;
}

int main(int argc, char** argv) {
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

    auto const layout = new QHBoxLayout{};
    main_layout->addLayout(layout);

    for(auto const hue: hues) {
        layout->addWidget([change_enable, change_state, hue] {
            auto const led = new bbwidgets::LedDualState{hue};
            led->setEnabled(true);
            led->setChecked(false);
            led->setAnimationDuration(1000ms);
            led->setAnimationEasingCurve(QEasingCurve::InOutQuart);

            change_enable->connect(change_enable, &QPushButton::clicked, [led, enable = led->isEnabled()]() mutable {
                enable = !enable;
                led->setEnabled(enable);
            });

            change_state->connect(change_state, &QPushButton::clicked, [led, check = led->isChecked()]() mutable {
                check = !check;
                led->setChecked(check);
            });

            return led;
        }());
    };

    layout->addWidget(makeLedMultiState(change_enable, change_state,
        std::map<State, bbwidgets::LedStyle>{
            {   State::Off,          {}},
            {   State::Red,   {0, true}},
            {State::Yellow,  {60, true}},
            { State::Green, {120, true}}
    }));

    layout->addWidget(makeLedMultiState(change_enable, change_state,
        std::map<int, bbwidgets::LedStyle>{
            {0,          {}},
            {1,   {0, true}},
            {2,  {60, true}},
            {3, {120, true}}
    }));

    window.show();

    return app.exec();
}
