// The MIT License (MIT)
//
// Copyright (c) 2016 Mateusz Pusz
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

#include <mp/fsm.h>
#include <iostream>

struct event_connect {};
struct event_disconnect {};

struct state_idle {};
struct state_connected {};

using state = std::variant<state_idle, state_connected>;

class sample_fsm : public mp::fsm<sample_fsm, state> {
public:
  template<typename State, typename Event>
  auto on_event(State&, const Event&) { return std::nullopt; }

  auto on_event(state_idle&, const event_connect&) { return state_connected{}; }
  auto on_event(state_connected&, const event_disconnect&) { return state_idle{}; }
};

int main()
{
  try {
    sample_fsm fsm;
    fsm.dispatch(event_connect());
    fsm.dispatch(event_disconnect());
  }
  catch(const std::exception& ex) {
    std::cerr << "Unhandled std exception caught: " << ex.what() << '\n';
  }
  catch(...) {
    std::cerr << "Unhandled unknown exception caught\n";
  }
}
