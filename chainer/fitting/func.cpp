#include <stdint.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <random>

float function (float x)
{
  return -1.0f * sinf (x) + 0.2f;
}


int main()
{
  std::ofstream      fp;
  std::random_device rnd;
  fp.open ("input_data.txt");

  for (int i = 0; i < 100000; i++) {
    float float_rand = ((static_cast<float>(rnd()) / UINT32_MAX) - 0.5f) * 4.0f;
    fp << float_rand << ", " << function(float_rand) << '\n';
  }
  fp.close();
  
  return 0;
}

