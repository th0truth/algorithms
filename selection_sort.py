from typing import List, Tuple
import random
import time

def swap(x: int, y: int) -> Tuple[int, int]:
  return y, x


def selection_sort(array: List[int], size: int) -> List[str]:  
  print("\nStart selection sorting...\n")

  counter = 0
  for j in range(size-1):
    min_idx = j

    for i in range(j+1, size):
      if (array[i] < array[min_idx]):
        min_idx = i

    print(f"\tMax found: {array[min_idx]} at index {min_idx}")

    if (min_idx != j):
      print(f"\tSwap {array[j]} at {j} with {array[min_idx]}\n\n")
      counter += 1
    else:
      print("\tNo swap needed\n\n")

    array[j], array[min_idx] = swap(array[j], array[min_idx])

    print("\tArray now: ", end="")
    for k in array:
      print(k, end=" ") 
    print("")

  print(f"\nTotal swaps: {counter}")

  return array


if __name__ ==  "__main__":
  array = list(map(int, input("Unsorted: ").split()))
  if not array:
    array = [int(random.random() * 1000) for _ in range(int(input("Size: ")))]
    print(f"Unsorted: {array}")

  begin = time.perf_counter_ns() 
  sorted = selection_sort(array, len(array))
  end = time.perf_counter_ns()

  print(f"Sorted array: {sorted}")
  print(f"Duration time: {(end - begin):.3f} ns")
