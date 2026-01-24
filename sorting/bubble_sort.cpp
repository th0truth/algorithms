#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

void BubbleSort(int *array, int n);

int main()
{

  vector<int> array = {10, 5, 15, 0, 12};
  int size = array.size();

  cout << "Unsorted array: ";
  for (int value : array)
  {
    cout << value << " ";
  }

  BubbleSort(array.data(), size);

  cout << "\nSorted array: ";
  for (int value : array){
    cout << value << " ";
  }
  cout << endl;

  return 0;
}


void BubbleSort(int *array, int n)
{
  int i, j;
  bool flag;

  for (i = 0; i < n; i++)
  {
    flag = false;
    for (j = 0; j < n-i-1; j++)
    {
      if (array[j] > array[j+1])
      {
        swap(array[j], array[j+1]);
        flag = true;
      }
    }

    if (!flag)
    {
      break;
    }
  }
}
