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

#include "mp/fsm.h"
#include <iostream>
#include <string_view>

struct event_connect { std::string_view address; };
struct event_connected {};
struct event_disconnect {};
struct event_timeout {};

struct state_idle {
  state_idle() { std::cout << "Idle\n"; }
};

struct state_connecting {
  static constexpr int n_max = 3;
  int n = 0;
  std::string address;
  explicit state_connecting(std::string address_) : address(std::move(address_))
  {
    std::cout << "Connecting to: " << address << '\n';
  }
};

struct state_connected {
  state_connected() { std::cout << "Connected\n"; }
};

using state = std::variant<state_idle, state_connecting, state_connected>;

class connection_fsm : public mp::fsm<connection_fsm, state> {
public:
  template<typename State, typename Event>
  auto on_event(State&, const Event&)
  {
    std::cout << "Invalid transition\n";
    return std::nullopt;
  }

  auto on_event(state_idle&, const event_connect& e) { return state_connecting{std::string(e.address)}; }
  auto on_event(state_connecting&, const event_connected&) { return state_connected{}; }
  auto on_event(state_connecting& s, const event_timeout&)
  {
    return ++s.n < state_connecting::n_max ? std::nullopt : std::optional<state>(state_idle{});
  }
  auto on_event(state_connected&, const event_disconnect&) { return state_idle{}; }
};
