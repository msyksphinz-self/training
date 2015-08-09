#include <iostream>
#include <vector>
#include <algorithm>

void permutation_int (int n);
void permutation_vector (void);


int main ()
{
    permutation_int (4);
    permutation_vector ();
}


void permutation_int (int n)
{
    int *perm = new int[n];

    for (int i = 0; i < n; i++) {
        perm[i] = i;
    }

    do {
        for (int i = 0; i < n; i++) {
            std::cout << perm[i] << " ";
        }
        std::cout << "\n";
    } while (std::next_permutation (perm, perm + n));

    return;
}


void permutation_vector(void)
{
    std::vector<std::string> perm;

    perm.push_back ("Hello");
    perm.push_back ("World");
    perm.push_back ("My name is");
    perm.push_back ("Msyksphinz");

    do {
        for (int i = 0; i < 4; i++) {
            std::cout << perm[i] << ",";
        }
        std::cout << "\n";
    } while (std::next_permutation (perm.begin(), perm.end()));

}
