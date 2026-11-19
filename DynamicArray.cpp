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
// 19 Nov 2026 : Implemented Grow(), Append(); Getters; operator[]; Test
// 19 Nov 2026 : Implemented Growth Test comparing geometric array growth vs 
//               increasing array by 1 on every append.
//               Result: 
//                  Geometric Doubling:
//                      Appends: 1,000
//                      Copies: 1,023 elements
//                      Space Cost: ~n
//                  Linear Growth + 1:
//                      Appends: 1,000
//                      Copies: 499,500
//                      Space Cost: ~ n^2/2, matches prediction;

#include <iostream>
#include <cstddef>
#include <cassert>

class DynamicArray{
public:
    using SizeType = std::size_t;

    enum class GrowthRule{
        Double,
        PlusOne
    };

    DynamicArray() = default;
    explicit DynamicArray(GrowthRule InRule);
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
        //

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
        // Return Success: int& . A reference to the value at the index.
        // Return Fail: Fails when Index >= Size.
        //              assert(Index < Size); 
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
    
    // Member Enums
    GrowthRule Rule = GrowthRule::Double;
};

// ===== DYNAMIC ARRAY IMPLEMENTATION =====

// GrowthRule Constructor
// Using Member initialiser list
DynamicArray::DynamicArray(GrowthRule InRule) : Rule(InRule){}

// Destructor
DynamicArray::~DynamicArray(){
    
    delete[] Data;
}

//Getter (1) - Capacity
DynamicArray::SizeType DynamicArray::GetCapacity() const{
    return this->Capacity;
}
//Getter (2) - CopyCount
DynamicArray::SizeType DynamicArray::GetCopyCount() const{
    return this->CopyCount;
}
//Getter (3) - Size
DynamicArray::SizeType DynamicArray::GetSize() const{
    return this->Size; 
}



void DynamicArray::Append(int Value){
   // Check if Array is Full: Does Size == Capacity?
   if(this->Size == this->Capacity){
       // Make Room
       this->Grow();
   }
   // Write into Empty Slot
   Data[Size] = Value; 
   // Record Size
   ++this->Size;
}

void DynamicArray::Grow(){
    //Resource Setup
    SizeType NewCapacity = 0;
    // 1. Decide and allocate: 1 if empty; otherwise double (Rule: Double) or add one slot (Capacity + 1).
    if(this->Capacity==0){
        NewCapacity=1;
    }
    else if (this->Rule == GrowthRule::Double) {
        NewCapacity=this->Capacity*2;
    }
    else{
        //The growth rule is PlusOne 
        NewCapacity=this->Capacity + 1;
    }

    int* NewData = new int [NewCapacity]; 
    // 2. Copy: Every stored element from index 0 of old array to new array. 
        // On every copy, increment CopyCount
        // Ensure that it loops over size and not capacity.
    for (SizeType i = 0; i < this->Size; ++i)
    {
        NewData[i] = Data[i]; 
        ++this->CopyCount;
    }
    // 3. Transfer ownership:
        // Free old block of memory,
        // Point Data variable to new block
        // Update capacity 
    delete[] Data;
    Data = NewData;
    this->Capacity = NewCapacity;

}

int& DynamicArray::operator[](SizeType Index){
    // [x] Test Fail Condition
    // Precondition: Index must be one of the stored elements.
    assert(Index < this->Size && "Index out of range: must be less than Size");
    // Read Part 
    return Data[Index];
}
// =====  END DYNAMIC ARRAY IMPLEMENTATION       ===== 

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
    DynamicArray TestArray;
    
    //Test: Append 5 numbers to an empty dynamic array
    // State BEFORE each append, and expected result:
    // 1st (10): Capacity 0, Size 0 -> full  -> Grow to 1
    // 2nd (20): Capacity 1, Size 1 -> full  -> Grow to 2
    // 3rd (30): Capacity 2, Size 2 -> full  -> Grow to 4
    // 4th (40): Capacity 4, Size 3 -> room  -> no Grow
    // 5th (50): Capacity 4, Size 4 -> full  -> Grow to 8
    // Expected CopyCount after each: 0, 1, 3, 3, 7
    
    for (int Value = 10; Value <= 50; Value += 10)
    {
        TestArray.Append(Value);
        std::cout<< "Size " << TestArray.GetSize()
                 << "  Capacity " << TestArray.GetCapacity()
                 << "  CopyCount " << TestArray.GetCopyCount() << '\n';
    }
   
    // TestArray[5] = 5

    // operator[] test: overwrite the 5th element (index 4). 
    // Expected: Prints 100, previously it was 50. Size stays 5.
    TestArray[4] = 100;
    std::cout << TestArray[4] << '\n'; 

    DynamicArray DefaultRuleDouble;
    DynamicArray PlusOne(DynamicArray::GrowthRule::PlusOne);

    for (DynamicArray::SizeType i = 0; i < 1000; ++i){
        DefaultRuleDouble.Append(i);
        PlusOne.Append(i);
    }
    std::cout << "Double: CopyCount " << DefaultRuleDouble.GetCopyCount()
              << '\n';
    std::cout << "PlusOne: CopyCount " << PlusOne.GetCopyCount();
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
// Moving Data:
//    Every "move" at the hardware level is a copy that goes through the CPU:
//        Load: The CPU reads a value from the old address into a register, its "little hands".
//        Store: It writes that register into the new address.
//        Register is the holding spot. Old values stay where it was until you
//        free it, which is why it must be freed.
// Goal: Prove the n^2 vs n claim for reallocation cost.
// Claim 1: Growing by +1 copies 1 + 2 + ... + (n-1) elements over n pushes,
//          about n^2 / 2 copies total: O(n^2), so O(n) per push.
// Claim 2: Growing by doubling copies 1 + 2 + 4 + ... elements,
//          less than 2n copies total: O(n), so amortised O(1) per push.
// Test:    Push n = 1000 ints, print CopyCount for each growth rule.
// Expect:  +1 rule ~ 499,500 copies. Doubling rule ~ 1,023 copies.

