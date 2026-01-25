from typing import List, Tuple
import time

def swap(x: int, y: int) -> Tuple[int, int]:
  temp = x
  x = y
  y = temp
  return x, y


def selection_sort(array: List[int], size: int) -> List[str]:  
  for j in range(size-1):
    _max = array[j]
    idx = j

    for i in range(j+1, size):
      if (array[i] < _max):
        _max = array[i]
        idx = i

    array[j], array[idx] = swap(array[j], array[idx])

  return array


if __name__ ==  "__main__":
  array = list(map(int, input("Unsorted: ").split()))

  begin = time.perf_counter_ns() 
  sorted = selection_sort(array, len(array))
  end = time.perf_counter_ns()

  print(f"Sorted array: {sorted}")
  print(f"Duration time: {(end - begin):.3f} ns")
