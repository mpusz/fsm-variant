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

#include "mp/fsm.h"
#include <chrono>
#include <iostream>
#include <optional>
#include <string_view>
#include <vector>
#include <thread>

using namespace std::chrono;

struct state_disconnected {};
struct state_connecting { std::string address; };
struct state_connected { system_clock::time_point timestamp = system_clock::now(); };
struct state_interrupted { std::string reason; };
using state = std::variant<state_disconnected, state_connecting, state_connected, state_interrupted>;

struct event_connect { std::string_view address; };
struct event_connected {};
struct event_interrupted { std::string_view reason; };
struct event_disconnect {};
using event = std::variant<event_connect, event_connected, event_interrupted, event_disconnect>;

class connection_fsm : public mp::fsm_base<connection_fsm, state, event> {
public:
  using base_type::on_event;
  auto on_event(state_disconnected&, const event_connect& e) { return state_connecting{std::string{e.address}}; }
  auto on_event(state_connecting&, const event_connected&) { return state_connected{}; }
  auto on_event(state_connected&, const event_disconnect&) { return state_disconnected{}; }
  auto on_event(state_connected&, const event_interrupted& e) { return state_interrupted{std::string{e.reason}}; }
  auto on_event(state_interrupted&, const event_connect& e) { return state_connecting{std::string{e.address}}; }
//  template<typename State, typename Event>
//  std::optional<state> on_event(State&, const Event&) { throw std::logic_error("invalid transition"); }

  void on_enter(state_disconnected&)  { std::cout << "Disconnected\n"; }
  void on_enter(state_connecting& s)  { std::cout << "Connecting to: " << s.address << '\n'; }
  void on_enter(state_connected&)     { std::cout << "Connected\n"; }
  void on_enter(state_interrupted& s) { std::cout << "Connection interrupted: " << s.reason << '\n'; }

  std::optional<system_clock::duration> connection_duration() const
  {
    if(const auto* current_state = std::get_if<state_connected>(&get_state()))
      return system_clock::now() - current_state->timestamp;
    else
      return std::nullopt;
  }
};

void dispatch(connection_fsm& fsm, const std::vector<event>& events)
{
  for(auto& e : events) {
    fsm.dispatch(e);
  }
}

void sequence_1(connection_fsm& fsm)
{
  std::vector<event> events = {event_connect{"train-it.eu"},
                               event_connected{},
                               event_interrupted{"timeout"}};

  std::cout << "\nSequence #1\n";
  dispatch(fsm, events);
}

void sequence_2(connection_fsm& fsm)
{
  std::vector<event> events = {event_connect{"train-it.eu"},
                               event_connected{}};

  std::cout << "\nSequence #2\n";
  dispatch(fsm, events);

  std::this_thread::sleep_for(1s);
  const auto dur = fsm.connection_duration();

  std::cout << "Connected for: " << duration_cast<milliseconds>(dur.value()).count() << "ms\n";
  fsm.dispatch(event_disconnect{});
}

int main()
{
  try {
    connection_fsm fsm;
    sequence_1(fsm);
    sequence_2(fsm);
  }
  catch(const std::exception& ex) {
    std::cerr << "Exception: ex.what()\n";
  }
}
