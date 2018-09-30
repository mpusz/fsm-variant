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

#include "tcp_fsm.h"
#include <iostream>

template<typename Fsm, typename... Events>
void dispatch(Fsm& fsm, Events&&... events)
{
  (fsm.dispatch(std::forward<Events>(events)), ...);
}

void active_open_close(tcp_fsm& fsm)
{
  dispatch(fsm, event_active_open(), event_rcv_syn_ack(), event_close(), event_rcv_fin_ack(), event_rcv_fin(),
           event_timeout());
}

void passive_open_close(tcp_fsm& fsm)
{
  dispatch(fsm, event_passive_open(), event_rcv_syn(), event_rcv_syn_ack(), event_close(), event_rcv_fin_ack(),
           event_rcv_fin(), event_timeout());
}

void active_open_fin(tcp_fsm& fsm)
{
  dispatch(fsm, event_active_open(), event_rcv_syn_ack(), event_rcv_fin(), event_close(), event_rcv_fin_ack());
}

void passive_open_fin(tcp_fsm& fsm)
{
  dispatch(fsm, event_passive_open(), event_rcv_syn(), event_rcv_syn_ack(), event_rcv_fin(), event_close(),
           event_rcv_fin_ack());
}

int main()
{
  try {
    tcp_fsm fsm;
    active_open_close(fsm);
    passive_open_close(fsm);
    active_open_fin(fsm);
    passive_open_fin(fsm);
  }
  catch(const std::exception& ex) {
    std::cerr << "Unhandled std exception caught: " << ex.what() << '\n';
  }
  catch(...) {
    std::cerr << "Unhandled unknown exception caught\n";
  }
}