#include <iostream>

int main ()
{

    float wind_max[12] = {0.2f, 1.5f, 3.3f, 5.4f, 7.9f, 10.7f,
                          13.8f, 17.1f, 20.7f, 24.4f, 28.4f, 32.6f};
    std::string deg_str[16] = {"N", "NNE", "NE", "ENE", "E", "ESE", "SE" , "SSE",
                               "S", "SSW", "SW", "WSW", "W", "WNW", "NW", "NNW"};
    int deg, dis;
    std::cin >> deg >> dis;

    int deg_index = (deg + (11.25 * 10)) / 225;
    std::cerr << "deg_index = " << deg_index << '\n';

    float dis_f = static_cast<float>(static_cast<int>((dis / 60.0f) * 10 + 0.5f)) / 10.0f;

    int wind_index;
    if (dis_f > wind_max[11]) {
        wind_index = 12;
    } else {
        for (int i = 0; i < 12 ; i++) {
            if (dis_f <= wind_max[i]) {
                wind_index = i;
                break;
            }
        }
    }

    std::cout << (wind_index == 0 ? "C" : deg_str[(deg_index % 16)]) << " " << wind_index << '\n';


}
