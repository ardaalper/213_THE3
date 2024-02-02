#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

//=======================//
// Implemented Functions //
//=======================//
template<int MAX_SIZE>
int HashTable<MAX_SIZE>::PRIMES[3] = {102523, 100907, 104659};

template<int MAX_SIZE>
void HashTable<MAX_SIZE>::PrintLine(int tableIndex) const
{
    const HashData& data = table[tableIndex];

    // Using printf here it is easier to format
    if(data.sentinel == SENTINEL_MARK)
    {
        printf("[%03d]         : SENTINEL\n", tableIndex);
    }
    else if(data.sentinel == EMPTY_MARK)
    {
        printf("[%03d]         : EMPTY\n", tableIndex);
    }
    else
    {
        printf("[%03d] - [%03d] : ", tableIndex, data.lruCounter);
        printf("(%-5s) ", data.isCostWeighted ? "True" : "False");
        size_t sz = data.intArray.size();
        for(size_t i = 0; i < sz; i++)
        {
            if(i % 2 == 0)
                printf("[%03d]", data.intArray[i]);
            else
                printf("/%03d/", data.intArray[i]);

            if(i != sz - 1)
                printf("-->");
        }
        printf("\n");
    }
}

template<int MAX_SIZE>
void HashTable<MAX_SIZE>::PrintTable() const
{
    printf("____________________\n");
    printf("Elements %d\n", elementCount);
    printf("[IDX] - [LRU] | DATA\n");
    printf("____________________\n");
    for(int i = 0; i < MAX_SIZE; i++)
    {
        PrintLine(i);
    }
}

//=======================//
//          TODO         //
//=======================//

/*This function should be utilized for hashing, in which two integers and a single boolean value
 are used to calculate the hash. The hash function multiplies the input with primes in the array
 “PRIMES”. Given prime array P = p0,p1,p2 and the input set S = starti,endi,cost; the resulting
 hash h is given in Equation 2. In this calculation “isCostWeighted” is considered 1 if it is true 
 and zero if it is false.*/
template<int MAX_SIZE>
int HashTable<MAX_SIZE>::Hash(int startInt, int endInt, bool isCostWeighted)
{
    /* TODO */
    if (isCostWeighted){
        return (startInt*PRIMES[0]+endInt*PRIMES[1]+PRIMES[2])%MAX_SIZE;
    }
    else{
        return (startInt*PRIMES[0]+endInt*PRIMES[1])%MAX_SIZE;
    }
}
/* The constructor initializes the hash table. For each entry, it sets the lruCounter to zero and marks
 every spot on the table as empty*/
template<int MAX_SIZE>
HashTable<MAX_SIZE>::HashTable()
{
    /* TODO */

    for (int i = 0; i < MAX_SIZE; ++i)
    {

        table[i].lruCounter = 0;

        table[i].sentinel = EMPTY_MARK;

        table[i].isCostWeighted = false;
        table[i].startInt = 0;
        table[i].endInt = 0;
    }

    elementCount = 0;
}

template<int MAX_SIZE>
int HashTable<MAX_SIZE>::Insert(const std::vector<int>& intArray, bool isCostWeighted)
{
    /* TODO */
    if(intArray.empty())
        throw InvalidTableArgException();

    
    int key=Hash(intArray[0],intArray[intArray.size()-1],isCostWeighted);
    int prob=0;
    int newkey = key;
    
    while(1){
        if (table[newkey].startInt==intArray[0] && table[newkey].endInt==intArray[intArray.size()-1] &&
        table[newkey].isCostWeighted==isCostWeighted){
            table[newkey].lruCounter++;
            return table[newkey].lruCounter-1;
        }
        else if(table[newkey].sentinel!=EMPTY_MARK){
            prob++;
            newkey = key + prob*prob;
            newkey %= MAX_SIZE;
        }
        else{
            if (elementCount == (MAX_SIZE+1) / CAPACITY_THRESHOLD)
                throw TableCapFullException(elementCount);

            table[newkey].sentinel = OCCUPIED_MARK;
            table[newkey].intArray = intArray;
            table[newkey].isCostWeighted = isCostWeighted;
            table[newkey].startInt = intArray.front();
            table[newkey].endInt = intArray.back();
            table[newkey].lruCounter = 1;
            elementCount++;
            return 0;
        }
    }

}
/* The find function tries to find the entry uniquely 
defined by the “startInt”, “endInt” and “isCost
Weighted” variables. It returns true if the entry 
exists, or returns false when it is unavailable.
Function increments the lruCounter when the “incLRU” 
variable is true.*/
template<int MAX_SIZE>
bool HashTable<MAX_SIZE>::Find(std::vector<int>& intArray,int startInt, 
                               int endInt, bool isCostWeighted,bool incLRU)
{
    /* TODO */
    int key=Hash(startInt,endInt,isCostWeighted);
    int i=0;
    int current;
    current=key;
    while(1){
        if(table[current].sentinel==OCCUPIED_MARK){
            if (table[current].startInt==startInt && table[current].endInt==endInt &&table[current].isCostWeighted==isCostWeighted){
                if(incLRU){
                    intArray = table[current].intArray;
                    table[current].lruCounter++;
                }
                return true;       
            }
            else{
                i++;
                current=key+i*i;
            }
        }
        else if(table[i].sentinel==SENTINEL_MARK){
            i++;
            current=key+i*i;
        }
        else{
            break; 
        }
        
    }
    return false;
}
/*Similar to the constructor, this function removes every entry from the table. Mark the spots as
 empty and delete the data of the present entries.*/
template<int MAX_SIZE>
void HashTable<MAX_SIZE>::InvalidateTable()
{
    /* TODO */
    for(int i=0;i<MAX_SIZE;i++){
            table[i].sentinel = EMPTY_MARK;
            table[i].intArray.clear();
            table[i].isCostWeighted = false;
            table[i].startInt = 0;
            table[i].endInt = 0;
            table[i].lruCounter = 0;
    }
    elementCount=0;
}
/* This function returns the data of the most inserted (or found) entry. In essence, it should return
 the highest lruCounter valued item in the table*/
template<int MAX_SIZE>
void HashTable<MAX_SIZE>::GetMostInserted(std::vector<int>& intArray) const
{
    /* TODO */
    int max=0;
    for(int i=0;i<MAX_SIZE;i++){
        if(table[i].sentinel==OCCUPIED_MARK){
            if(table[i].lruCounter>max){
                max=table[i].lruCounter;
                intArray = table[i].intArray;
            }
        }
    }
}
/*  Similar to the function above, the remove function finds the entry removes it, and returns entries
 data. If this uniquely-defined entry does not exist it silently returns. In this case, the resulting
 “intArray” should not be modified*/
template<int MAX_SIZE>
void HashTable<MAX_SIZE>::Remove(std::vector<int>& intArray,
                                 int startInt, int endInt, bool isCostWeighted)
{
    /* TODO */
    
    int key=Hash(startInt,endInt,isCostWeighted);
    int i=0;
    int current;
    current=key;
    while(1){
        if(table[current].sentinel==OCCUPIED_MARK){
            if (table[current].startInt==startInt && table[current].endInt==endInt &&table[current].isCostWeighted==isCostWeighted){
                intArray = table[current].intArray;
                
                table[current].sentinel = SENTINEL_MARK;
                table[current].intArray.clear();
                table[current].isCostWeighted = false;
                table[current].startInt = 0;
                table[current].endInt = 0;
                table[current].lruCounter = 0;
                
                elementCount--;
                break;
            }
            else{
                i++;
                current=key+i*i;
            }
        }
        else if(table[current].sentinel==SENTINEL_MARK){
            i++;
            current=key+i*i;
        }
        else{
            break; 
        }
        
    }

}
/*This function removes the top “lruElementCount” items from the table. Lowest lruCounter valued
 items should be removed from the table.*/
template<int MAX_SIZE>
void HashTable<MAX_SIZE>::RemoveLRU(int lruElementCount)
{
    /* TODO */
    int bizim;
    for(int j=0; j<lruElementCount; j++){
        int min = 9999999;
        for(int i=0; i<MAX_SIZE; i++){
            if(table[i].sentinel == OCCUPIED_MARK){
                if(table[i].lruCounter < min){
                    bizim=i;
                    min=table[i].lruCounter;
                    
                }
            }
        }
        table[bizim].sentinel=SENTINEL_MARK;
        table[bizim].intArray.clear();
        table[bizim].isCostWeighted=false;
        table[bizim].startInt=0;
        table[bizim].endInt=0;
        table[bizim].lruCounter=0;
    }
    elementCount-=lruElementCount;
}
/* This function prints the entries with non-zero lruCounter in a high-to-low fashion.*/
template<int MAX_SIZE>
void HashTable<MAX_SIZE>::PrintSortedLRUEntries() const
{
    std::vector<const HashData*> entries;

    for (int i = 0; i < MAX_SIZE; i++) {
        if (table[i].sentinel == OCCUPIED_MARK && table[i].lruCounter > 0) {
            entries.push_back(&table[i]);
        }
    }
    
    const HashData* temp;
    for (int i = 0; i < entries.size() - 1; i++) {
        int max = i;
        for (int j = i + 1; j < entries.size(); j++) {
            if (entries[j]->lruCounter > entries[max]->lruCounter) {
                max = j;
            }
        }
        temp = entries[i];
        entries[i] = entries[max];
        entries[max] = temp;
    }
    
    for(int i=0;i<entries.size();i++){
        PrintLine(entries[i] - table);
    }

}







#endif // HASH_TABLE_HPP