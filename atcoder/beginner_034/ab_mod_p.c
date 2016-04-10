#include <iostream>
#include <stdint.h>
using namespace std;

int64_t ab_mod_p (int64_t a, int64_t b, int64_t p);

int main ()
{
  cout << "10 ** 3 % 32 = " << ab_mod_p (100, 100, 31) << std::endl;

  return 0;
}


int64_t ab_mod_p (int64_t a, int64_t b, int64_t p)
{
  // cout << "ab_mod_p(" << a << ", " << b << ", " << p << ")\n";
  if (b == 0) {
    // cout << "reach    = 1\n";
    return 1;
  } else if ((b & 0x01) == 0) {
    int d = ab_mod_p (a, b/2, p);
    int ret = (d * d) % p;
    // cout << "even ret = " << ret << endl;
    return ret;
  } else {
    int ret = (a * ab_mod_p(a, b-1, p)) % p;
    // cout << "odd  ret = " << ret << endl;
    return ret;
  }
}
