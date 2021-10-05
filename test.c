#include "header.h"

void test()
{
  struct ints arr;
  arr.arr = malloc(sizeof(int));
  arr.sz = 0;

  add_to_int_arr(&arr, 1);
  add_to_int_arr(&arr, 1);
  add_to_int_arr(&arr, 1);
  add_to_int_arr(&arr, 1);
  add_to_int_arr(&arr, 1);

  for (int i = 0; i < arr.sz; i++)
  {
    printf("%d\n", arr.arr[i]);
  }
}
