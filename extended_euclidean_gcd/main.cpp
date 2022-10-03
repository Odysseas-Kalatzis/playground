#include <iostream>
#include <cassert>
#include <cctype>
#include <cstring>
#include <cstdlib>

int main(int argc, char** argv)
{
    assert(argc == 3);
    for (std::size_t i = 1; i < 3; ++i)
    {
        assert(strlen(argv[i])> 0); 
        for (std::size_t j = 0; j < strlen(argv[i]); ++j)
        {
            if (!(std::isdigit(argv[i][j])))
            {
                std::cout << "Both inputs for gcd must be integers.\n";
                return 1;
            }
        }
    }

    int a = std::atoi(argv[1]);
    int b = std::atoi(argv[2]);

    assert(a > 0 && b > 0);

    int r_0 = std::max(a, b);
    int r_1 = std::min(a, b);

    int s_0 = 1;
    int s_1 = 0;

    int t_0 = 0;
    int t_1 = 1;

    while(r_1 != 0)
    {
        int q_1 = r_0/r_1;

        int r_2 = r_0 - q_1*r_1;
        int s_2 = s_0 - q_1*s_1;
        int t_2 = t_0 - q_1*t_1;

        r_0 = r_1;
        r_1 = r_2;

        s_0 = s_1;
        s_1 = s_2;

        t_0 = t_1;
        t_1 = t_2;
    }

    std::cout << "gcd(" << a << ", " << b << ") = " << r_0 << '\n';
    std::cout << "Bezout coefficients: For " << std::max(a, b) << " we have: " << s_0 
        << ", and for " << std::min(a, b) << " we have: " << t_0 << '\n';
    
    return 0;
}
