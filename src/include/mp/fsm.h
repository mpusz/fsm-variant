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
#include <utility>

namespace mp {

  template<typename Derived, typename StateVariant>
  class fsm {
    StateVariant state_;
  public:
    const StateVariant& get_state() const { return state_; }
    StateVariant& get_state() { return state_; }

    template<typename Event>
    void dispatch(Event&& event)
    {
      Derived& child = static_cast<Derived&>(*this);
      auto new_state = std::visit(
          [&](auto& s) -> std::optional<StateVariant> { return child.on_event(s, std::forward<Event>(event)); },
          state_);
      if(new_state)
        state_ = *std::move(new_state);
    }
  };

}  // namespace mp
