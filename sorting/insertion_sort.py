from typing import List, Tuple
import random
import time


def insertion_sort(array: List[int], size: int) -> List[int]:  
    print("\nStart insertion sorting...\n")  # Fixed: was "selection"
    counter = 0

    for i in range(1, len(array)):
        counter += 1
        print(f"\n\tPass {counter} processing element at index {i}")
        
        temp = array[i]
        print(f"\tCurrent element to insert {temp}")

        j = i - 1
        shifted = False
        while (j >= 0 and temp < array[j]):
            print(f"\tShifting {array[j]} from index {j} to index {j+1}")        
    
            array[j+1] = array[j]
            j = j - 1

            shifted = True
        
        array[j+1] = temp
    
        # Fixed: Proper indentation - these should be inside the for loop
        # but outside the while loop
        if (shifted):
            print(f"\tInserted {temp} at index {j+1}")
        else:
            print(f"\tElement {temp} already in correct position")

        print("\tArray now: ", end="")
        for k in array:
            print(k, end=" ") 
        print("")

    return array


if __name__ == "__main__":
    array = list(map(int, input("Unsorted: ").split()))
    if not array:
        array = [int(random.random() * 1000) for _ in range(int(input("Size: ")))]
        print(f"Unsorted: {array}")

    begin = time.perf_counter_ns() 
    sorted_array = insertion_sort(array, len(array))  # Fixed: 'sorted' is a built-in function
    end = time.perf_counter_ns()

    print(f"\nSorted array: {sorted_array}")
    print(f"Duration time: {(end - begin):.3f} ns")
