#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <vector>
using namespace std;

struct foo
{
        int     x;
        int     y;
        int     z;
};
//structを宣言して、vector型で用いる
/*
int main (int argc, char **argv)
{
        vector<foo> v;//vectorクラスfoo型のオブジェクトvを宣言
        foo a = { 10, 120, 7}, b = { 3, 2, 3}, c = { -1, -2, -3};

        v.push_back (a);
        v.push_back (b);//配列ごとに代入される
        v.push_back (c);

        for (std::vector<foo>::const_iterator i = v.begin ();
                        i != v.end (); i++)
        {
                (void) printf ("%d\n", i->y);//yの位置をプリント
        }

        exit (EXIT_SUCCESS);
}
*/



std::vector<int> v{1, 2, 3, 4};
int main()
{
  for (std::vector<int>::iterator itr = v.begin(); itr != v.end(); itr++)
  { std::cout << *itr << "\n";      // 最初の要素の値を表示
  }
}
