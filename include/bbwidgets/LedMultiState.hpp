#pragma once

#include "LedAnimated.hpp"

#include <map>
#include <type_traits>

namespace bbwidgets {


    template <typename State>
    class LedMultiState: public LedAnimated {
        Q_PROPERTY(State state READ state WRITE setState)

    public:
        using state_type = State;

        LedMultiState(std::map<State, LedStyle> state_map, QWidget* const parent = nullptr)
            : LedAnimated({}, parent)
            , state_map_(std::move(state_map))
            , state_(defaultState()) {
            setStyle(styleOf(state_));
        }

        LedMultiState(LedMultiState const&) = delete;

        LedMultiState& operator=(LedMultiState const&) = delete;

        [[nodiscard]] State state() const {
            return state_;
        }

        void setState(State state) {
            if(state_ == state) {
                return;
            }

            state_ = std::move(state);
            startAnimation(styleOf(state_));
        }

    private:
        [[nodiscard]] State defaultState() const {
            auto const iter = state_map_.begin();
            if(iter == state_map_.end()) {
                qCritical() << "LedMultiState initialized with empty state_map";
                return {};
            }
            return iter->first;
        }

        [[nodiscard]] LedStyle styleOf(State const& state) const {
            auto const iter = state_map_.find(state);
            if(iter == state_map_.end()) {
                qCritical() << "LedMultiState set to unknown state";
                return {};
            }
            return iter->second;
        }

        std::map<State, LedStyle> const state_map_;
        State state_;
    };

    template <typename State>
    LedMultiState(std::map<State, LedStyle> const&) -> LedMultiState<State>;


}
