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

struct event_active_open { int address; int port; };
struct event_passive_open { int port; };
struct event_close {};
struct event_send {};
struct event_rcv_syn {};
struct event_rcv_syn_ack {};
struct event_rcv_fin {};
struct event_rcv_fin_ack {};
struct event_timeout {};

struct state_closed {};
struct state_listen {};
struct state_syn_sent {};
struct state_syn_received {};
struct state_established {};
struct state_fin_wait_1 {};
struct state_fin_wait_2 {};
struct state_close_wait {};
struct state_last_ack {};
struct state_closing {};
struct state_time_wait {};

using state = std::variant<state_closed, state_listen, state_syn_sent, state_syn_received, state_established, state_fin_wait_1, state_fin_wait_2, state_close_wait, state_last_ack, state_closing, state_time_wait>;


class tcp_fsm : public mp::fsm<tcp_fsm, state> {
public:
  template<typename State, typename Event>
  std::optional<state> on_event(State&, const Event&) { throw std::logic_error("Invalid transition"); }

  auto on_event(state_closed&, const event_active_open&) { return state_syn_sent(); }
  auto on_event(state_closed&, const event_passive_open&) { return state_listen(); }

  auto on_event(state_syn_sent&, const event_rcv_syn_ack&) { return state_established(); }
  auto on_event(state_syn_sent&, const event_close&) { return state_closed(); }
  auto on_event(state_syn_sent&, const event_rcv_syn&) { return state_syn_received(); }

  auto on_event(state_listen&, const event_rcv_syn&) { return state_syn_received(); }
  auto on_event(state_listen&, const event_send&) { return state_syn_sent(); }
  auto on_event(state_listen&, const event_close&) { return state_closed(); }

  auto on_event(state_syn_received&, const event_rcv_syn_ack&) { return state_established(); }
  auto on_event(state_syn_received&, const event_close&) { return state_fin_wait_1(); }

  auto on_event(state_established&, const event_rcv_fin&) { return state_close_wait(); }
  auto on_event(state_established&, const event_close&) { return state_fin_wait_1(); }

  auto on_event(state_close_wait&, const event_close&) { return state_last_ack(); }

  auto on_event(state_last_ack&, const event_rcv_fin_ack&) { return state_closed(); }

  auto on_event(state_fin_wait_1&, const event_rcv_fin&) { return state_closing(); }
  auto on_event(state_fin_wait_1&, const event_rcv_fin_ack&) { return state_fin_wait_2(); }

  auto on_event(state_fin_wait_2&, const event_rcv_fin&) { return state_time_wait(); }

  auto on_event(state_closing&, const event_rcv_fin_ack&) { return state_time_wait(); }

  auto on_event(state_time_wait&, const event_timeout&) { return state_closed(); }
};
