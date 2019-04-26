int gcd (int a, int b)
{
  int tmp, r;
  if (a<b){
    tmp = a;
    a = b;
    b = tmp;
  }

  r = a % b;
  while(r!=0){
    a = b;
    b = r;
    r = a % b;
  }

  return b;
}


long long gcd(long long a,long long b)
{
  if(b==0)
    return a;
  return gcd(b,a%b);
}

long long lcm(long long a,long long b){
  long long g=gcd(a,b);
  return a/g*b;
}
