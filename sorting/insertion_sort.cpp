#include <iostream>
#include <cstdint>
#include <ostream>
#include <vector>
#include <string>
#include <chrono>
#include <ctime>
using namespace std;

void insertion_sort(int32_t * arr, int size);
void init_array(vector<int32_t>& array, int n, const char ** argv);


int main(int argc, const char ** argv)
{
  vector<int32_t> array;

  // Initialize array
  init_array(array, argc, argv);

  
  // Measure duration time of selection sort execution
  chrono::steady_clock::time_point begin = chrono::steady_clock::now();
  insertion_sort(array.data(), array.size());
  chrono::steady_clock::time_point end = chrono::steady_clock::now();

  cout << "\nSorted: ";
  for (int32_t val : array)
  {
    cout << val << " ";
  }

  cout << "\nDuration time: " << chrono::duration_cast<chrono::nanoseconds>(end - begin).count() << " ns" << endl;

  return 0;
}

// Insertion Sort algorithm
void insertion_sort(int32_t * array, int size)
{
  cout << "Start insertion sorting ..." << endl;
  int counter = 0;

  for (int i = 1; i < size; i++)
  {
    counter++;
    cout << "\n\tPass " << counter << " (processing element at index " << i << "):" << endl;
      
    int32_t temp = array[i];
    cout << "\tCurrent element to insert: " << temp << endl;

    // Initialize j to point to the last element of the sorted portion
    int j = i - 1;
    
    
    bool shifted = false;
    while (j >= 0 && temp < array[i])
    {
      cout << "\tShifting " << array[j] << " from index " << j << " to index " << (j+1) << endl;

      array[j+1] = array[j];
      j = j - 1;
    
      shifted = true;
    }
    array[j+1] = temp;
  
    if (shifted)
    {
      cout << "\tInserted " << temp << " at index " << (j+1) << endl;
    } else {
      cout << "\tElement " << temp << " already in correct position" << endl;
    }


    cout << "\tArray now: " << counter << ": ";
    for (int k = 0; k < size; k++)
    {
      cout << array[k] << " ";
    }
    cout << endl << endl;
  }
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
