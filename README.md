# mp::fsm

Implementation of Finite State Machine presented by me on CppCon 2018 in a talk
[Effective replacement of dynamic polymorphism with std::variant](https://github.com/CppCon/CppCon2018/blob/master/Presentations/effective_replacement_of_dynamic_polymorphism_with_stdvariant/effective_replacement_of_dynamic_polymorphism_with_stdvariant__mateusz_pusz__cppcon_2018.pdf). 

It uses `std::variant` and `std::optional` under the hood.


## How to?

FSM works with any types defining **events**. **State** is represented by any type too.
All states have to be put into the variant and the initial state should appear the first
on the list. Both states and events can be stateful.

```cpp
#include <mp/fsm.h>

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
  sample_fsm fsm;
  fsm.dispatch(event_connect());
  fsm.dispatch(event_disconnect());
}
```


## Building, testing and installation

For detailed information on project compilation, testing and reuse please refer to
[INSTALL.md](INSTALL.md).


## Acknowledgements

The FSM design was inspired by:
- http://khuttun.github.io/2017/02/04/implementing-state-machines-with-std-variant.html
- http://mooooo.ooo/sumtypes-and-statemachines/

Thank you!