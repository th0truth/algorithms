#include <iostream>
#include <cstdint>
#include <ostream>
#include <vector>
#include <string>
#include <chrono>
#include <ctime>
using namespace std;

void selection_sort(int32_t * array, int size); 
void init_array(vector<int32_t>& array, int n, const char ** argv);

int main(int argc, const char ** argv)
{
  vector<int32_t> array;
  
  // Initialize array
  init_array(array, argc, argv);

  // Measure duration time of selection sort execution
  chrono::steady_clock::time_point begin = chrono::steady_clock::now();
  selection_sort(array.data(), array.size());
  chrono::steady_clock::time_point end = chrono::steady_clock::now();

  cout << "\nSorted: ";
  for (int32_t val : array)
  {
    cout << val << " ";
  }

  cout << "\nDuration time: " << chrono::duration_cast<chrono::nanoseconds>(end - begin).count() << " ns" << endl;

  return 0;
}

// Selection Sort
void selection_sort(int32_t * array, int size)
{
  cout << "Start selection sorting ...\n" << endl;
  int counter = 0;

  for (int j = size - 1; j >= 0; j--)
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

    cout << "\tMax found: " << _max << " at index " << idx << endl;
    

    if (idx != j)
    {
      cout << "\tSwap " << array[j] << " at " << j << " with " << array[idx] << "\n\n";
      counter++;
    } else {
      cout << "\tNo swap needed\n\n";
    }

    // swap using build in function std::swap
    swap(array[j], array[idx]);
    
    cout << "\tArray now: ";
    for (int k = 0; k < size; k++)
    {
      cout << array[k] << " ";
    }
    cout << endl;
  }
  cout << "\nTotal swaps: " << counter << endl;
}

void init_array(vector<int32_t>& array, int n, const char ** argv)
{
  if (n > 1)
  {
    cout << "Unsorted: ";
    for (int i = 1; i < n; i++)
    {
      array.push_back(stoi(argv[i]));
      cout << array.back() << " ";
    }
    cout << "\n\n";
  } else {
    int size;

    cout << "Size of array: ";
    cin >> size;
    srand(time(nullptr));
    
    cout << "Unsorted: ";
    for (int i = 0; i < size; i++)
    {
      array.push_back(rand());
      cout << array.back() << " ";
    }
    cout << "\n\n";
  }
}
