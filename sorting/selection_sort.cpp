#include <iostream>
#include <cstdint>
#include <vector>
#include <random>
#include <ctime>
using namespace std;

void Selection_Sort(int32_t *array, int size); 

int main(int argc, const char **argv)
{
  int size = argc-1;
  vector<int32_t> array(size);
  
  for (int i = 1; i < argc; i++)
  {
    array[i-1] = stoi(argv[i]);
  }

  if (array.data() == 0)
  {
    int size;
    cin >> size;
    
    mt19937 random(time(nullptr));
    
    
  }
  

  Selection_Sort(array.data(), size);

  cout << "Result: ";
  for (int val : array)
  {
    cout << val << " ";
  }
  cout << endl;

  return 0;
}

// Selection Sort (but it works slightly different than a traditional)
void Selection_Sort(int32_t *array, int size)
{
  for (int32_t j = size - 1; j >= 0; j--)
  {
    int32_t _max = array[j];
    int idx = j;

    for (int i = 0; i < j; i++)
    {
      if (array[i] > _max)
      {
        _max = array[i];
        idx = i;
      }
    }
    int32_t tmp = array[j];
    array[j] = array[idx];
    array[idx] = tmp;
  }
}
