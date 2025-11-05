#ifndef OVERLOADED_H_
#define OVERLOADED_H_

namespace computer_club::utils {

template <typename... Ts>
struct Overloaded : Ts... {
    using Ts::operator()...;
};

template <typename... Ts>
Overloaded(Ts...) -> Overloaded<Ts...>;

}  // namespace test_computer_club.cpp::utils

#endif  // OVERLOADED_H_