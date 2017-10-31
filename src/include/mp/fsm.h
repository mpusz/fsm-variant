// The MIT License (MIT)
//
// Copyright (c) 2017 Mateusz Pusz
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include <optional>
#include <variant>

namespace mp {

  template<typename Derived, typename StateVariant, typename EventVariant>
  class fsm_base {
    StateVariant state_;

  public:
    using base_type = fsm_base<Derived, StateVariant, EventVariant>;

    const StateVariant& get_state() const { return state_; }
    StateVariant& get_state() { return state_; }

    void dispatch(const EventVariant& event)
    {
      Derived& fsm = static_cast<Derived&>(*this);
      auto new_state = std::visit(
          [&](auto& state, const auto& event) -> std::optional<StateVariant> { return fsm.on_event(state, event); },
          state_, event);
      if(new_state) {
        const bool state_change = state_.index() != new_state->index();
        if(state_change) std::visit([&](auto& s) { fsm.on_exit(s); }, state_);
        state_ = *std::move(new_state);
        if(state_change) std::visit([&](auto& s) { fsm.on_enter(s); }, state_);
      }
    }

    // default implementation of handlers
    template<typename State, typename Event>
    std::optional<StateVariant> on_event(State&, const Event&)
    {
      return std::nullopt;
    }
    template<typename State>
    void on_enter(State&)
    {
    }
    template<typename State>
    void on_exit(State&)
    {
    }
  };

}  // namespace mp
