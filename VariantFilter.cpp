

#include <variant>
#include <iostream>

using namespace std;

using Pixel = int;
 
class FilterBright{};
class FilterDark{};

using var_t = std::variant<FilterBright, FilterDark>;
using pixel_t = std::variant<Pixel>;

template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
template<class... Ts> overload(Ts...) -> overload<Ts...>;

int main()
{
    var_t filter{FilterBright()};
    pixel_t i = 5;
    
    std::visit (overload{
        [](FilterBright f, Pixel& pixel)  { pixel+=1; cout << "Filter Bright "; },
        [](FilterDark f, Pixel& pixel)  {  pixel-=1; cout << "Filter Bright ";  },
    }, filter,i);
}
