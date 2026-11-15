// Resources
// 1. Sedgewick, Algorithms in C++, Parts 1-4 (3rd ed., 1998), Ch. 3.2 Arrays
// 2. std::vector reference
//    https://en.cppreference.com/w/cpp/container/vector
// 3. C++ standard, [vector]
//    https://eel.is/c++draft/vector
// 4. MSVC Implementation: How Microsoft builds it.

// Dynamic Array - Work log
// 12 Nov 2026 : Implement Static Array
// 13 Nov 2026 : Start on DynamicArray class header
// 15 Nov 2026 : Finished DynamicArray header, implemented array destructor [] 

#include <iostream>
#include <cstddef>

class DynamicArray{
public:
    using SizeType = std::size_t;

    DynamicArray() = default;
    ~DynamicArray();

    DynamicArray(const DynamicArray&) = delete;
    DynamicArray& operator=(const DynamicArray&) = delete; 

    void Append(int Value); 
        // Method [1]: Append(int Value);
        // Outcome: Append a desired element to the `end()` of the array.
        // Parameters: int Value . Value type is the desired int to add.
        // Return Type: void
        // Return: None
        //Per [sequence.reqmts] back() is defined as the element just before   end(). I will use the word append as it carries a clear meaning. "back" = The element at index size()-1.

    int& operator[](SizeType Index);
        // Method [2]: operator[](SizeType Index);
        // Precondition: Index < Size.
        // Outcome: The caller may 
                // 1. Read an int at a desired index of the array.
                // 2. Replace an int at the desired index, with a new int
                    //without changing the Size() of the array or moving
                    //any other element.
        // Parameters: An `index` of SizeType
        // Return Type: int&
        // Return: int& . A reference to the value at the index.
        // What I Learnt: Check Index < Size, not Index < Capacity. Only 0 to Size - 1 are elements. Slots past Size hold garbage, not "none": an int has no null, and by Stroustrup's zero-overhead principle ("you don't pay for what you don't use") new memory isn't cleared, because Grow and Append always write a slot before anything reads it.

    SizeType GetCapacity() const;
        // Method [3]: GetCapacity() . GETTER
        // Outcome: Returns how many slots exist before the array must grow. 
        // Parameters: None
        // Return Type: SizeType
        // Return: Capacity 
        // const after (): Do not change the array object.

    SizeType GetCopyCount() const; 
        // Method [4]: GetCopyCount() . GETTER
        // Outcome: Returns the total number of elements copied by Grow(); 
        // Parameters: None
        // Return Type: SizeType 
        // Return: CopyCount
        // const after (): Do not change the array object.

    SizeType GetSize() const;
        // Method [5]: GetSize() . GETTER
        // Outcome: Returns how many elements are stored.
        // Parameters: None 
        // Return Type: SizeType
        // Return: Size, which is the number of stored elements. 
        // const after (): Do not change the array object.

private:
    // Member Functions 
    void Grow();
    // Method_Private [1] : Grow() 
    // Outcome : Increases array capacity when full ( Size == Capacity ) 
        // 1. Allocates a new "block" of double the capacity or 1 if cap == 0. 
        // 2. Copies every stored element across (in original order)
            // 2.1 Each element copied adds 1 to CopyCount. 
        // 3. Frees old  "block", and points Data at the new one
    // Growth Factor: 2x
    // Parameters: None
    // Return Type: void
    // Return: None

    // Member Variables
    int*     Data      = nullptr; // Address of start of array block
    SizeType Capacity  = 0; // No# slots before the array must grow.
    SizeType CopyCount = 0; // Number of elements copied during Grow();
    SizeType Size      = 0; // Amount of elements currently stored in array.
    
};

// ===== DYNAMIC ARRAY IMPLEMENTATION =====
// Destructor
DynamicArray::~DynamicArray(){
    
    delete[] Data;
}

// TODO: Grow(); 

// =====          END                 ===== 

void StaticArray(){
    int Values[5] = {1,2,3,4,5};

    for (std::size_t i = 0; i < 5; ++i){
        std::cout<<i<<": "<<&Values[i]
                 <<" = "
                 <<Values[i]<<'\n';
    }
    std::cout<<"sizeof(int) = " << sizeof(int) << '\n';
    std::cout<<*(Values+2) << '\n';
}

int main(int argc, const char* argv[]){
    // StaticArray();
}

//Definitions
// Stack:
//   - Lifetime: One function call its scope {}
//   - Size: Small, fixed per thread.
//   - Access Speed: Fast
//   - Allocation Speed: Instant
// Heap:
//   - Lifetime: Outlives its call scope {}
//   - Size: User's RAM and Virtual Memory Size
//   - Access Speed: Can be as fast as Stack access if data is in cpu-cache
//      Or slightly slower if data is in RAM. 1ns(cpu-cache) vs 100ns(RAM)
//   - Allocation Speed: Slower as the allocator needs to search for a free block of memory.

// Static-Arrays: 
//    Contiguous in memory, due to same sized elements.
//    O(1) access via base + index * size (of element). 
//    Resulting in speedy cpu-cache iteration
//    Cost is slower middle array insertion of O(n)
//    Cost is reallocation when array size needs to increase
//        Reallocation can invalidate pointers in array.

// Goal: Prove the n^2 vs n claim for reallocation cost.
// Claim 1: Growing by +1 copies 1 + 2 + ... + (n-1) elements over n pushes,
//          about n^2 / 2 copies total: O(n^2), so O(n) per push.
// Claim 2: Growing by doubling copies 1 + 2 + 4 + ... elements,
//          less than 2n copies total: O(n), so amortised O(1) per push.
// Test:    Push n = 1000 ints, print CopyCount for each growth rule.
// Expect:  +1 rule ~ 499,500 copies. Doubling rule ~ 1,023 copies.

