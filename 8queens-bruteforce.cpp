#include <iostream>

using namespace std;

int
check (char *q)
{
  int i, j;
  for (i = 0; i < 8; i++)
    for (j = i + 1; j < 8; j++)
      if (q[i] == q[j] || q[i] + i == q[j] + j || q[i] - i == q[j] - j)
	return 0;
  return 1;
}

void
show (char *q)
{
  cout << endl;
  int i, j;
  for (i = 0; i < 8; i++)
    {
      cout << "\n   +----+----+----+----+----+----+----+----+\n" << 8 -
	i << "  ";
      for (j = 0; j < 8; j++)
	cout << (j == q[i] ? "| Q  " : "|    ");
      cout << "|";
    }
  cout << "\n   +----+----+----+----+----+----+----+----+\n";
  cout << "      a    b    c    d    e    f    g    h\n\n";
}


int
main ()
{
  int i = 0, j, k, l = 0;
  char q[8];
  char ans[100][8];
  while (i++ < (1 << 24))
    {
      j = i;
      for (k = 0; k < 8; k++)
	{
	  q[k] = j % 8;
	  j /= 8;
	}
      if (check (q))
	{
	  for (k = 0; k < 8; k++)
	    ans[l][k] = q[k];
	  show (ans[l++]);
	}

      cout << "\r" << i << "/" << (1 << 24);
    }

  cout << "\ntotal answers found: " << l << endl;
  return 0;
}
