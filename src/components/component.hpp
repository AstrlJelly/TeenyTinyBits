#pragma once


class Component{};
// requires
// {
//     requires std::is_base_of_v<Component, T>;
//     requires !std::is_arithmetic<T>::value;
//     requires !std::is_pointer_v<T>;
//     { T() };
// }