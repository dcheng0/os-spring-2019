// include guards prevent double declarations
#ifndef DOCUMENTS_MAP_IMPL_H
#define DOCUMENTS_MAP_IMPL_H

//include from C++ standard library
#include <string>
#include <memory>
#include <functional>
#include <utility>
#include <vector>
//include from project directory
#include "data_structures/map/maybe.h"

namespace data_structures {
namespace map {

template<typename KeyType, typename ValueType>
class MapImpl {
// public access modifier
public:
// C++ allows us to create new types, based off existing types, and define them
    // the below will be used for Remove
    typedef std::function<bool(const KeyType&, const KeyType&)> KeyComparerFn;
    // the below will be used for Put
    typedef std::function<uint32_t(const KeyType&)> HashCalculator;
// private access modifier to define private types
private:
    // Type for map entries
    // Each entry of the Map is a pair of a key and a value
    typedef std::pair<KeyType, ValueType> MapEntry;
    // The underlying structure for the Map will be a vector of entries
    typedef std::vector<MapEntry> MapList;

    /*
     * In trying to understand the trailing underscores, I found the following:
     * http://geosoft.no/development/cppstyle.html
     * In short, it is a nice naming convention for private class variables
     */
    const KeyComparerFn key_comparer_;
    const HashCalculator hash_calculator_;
    const uint32_t capacity_;
    uint32_t size_;
    /* from https://en.cppreference.com/w/cpp/memory/unique_ptr
     *
     * "unique_ptr is a smart pointer that owns and manages another object through
     * a pointer. "
     */
    std::unique_ptr<MapList[]> storage_;
public:

    MapImpl(const KeyComparerFn key_comparer,
            const HashCalculator hash_calculator, const uint32_t capacity)
        : key_comparer_(key_comparer), hash_calculator_(hash_calculator),
          capacity_(capacity), size_(0), storage_(new MapList[capacity])
    {}

    // a way to check the size of the map
    int Size() const {
        return (int)size_;
    }

    // method for adding key and value to map
    void Put(const KeyType& key, const ValueType& value) {
        auto index = GetIndex(key);
        MapList& list = storage_.get()[index];
        MapEntry new_entry = {key,value};
        // iterates through list
        for(auto it = list.begin();
            it != list.end(); ++it) {
            // the reference of entry is set equal to the key and value
            // of where the iterator is pointing
            MapEntry& entry = *it;
            // if the key exists, it is erased and then appended to the end
            if(key_comparer_(entry.first, key)) {
                list.erase(it);
                list.emplace_back(new_entry);
                /*
                 * size is not updated, unless you put something that was not
                 * there before.
                 * we essentially just moved a value from its original
                 * place to the end
                 */
                return;
            }
        }
        // if the key is not found, we append it to the end of the list
        list.emplace_back(new_entry);
        // we just added a new key, now we need to increment size
        ++size_;
    }

    /*
     * I think of this as a remove method, that has the added benefit of
     * returning True if the key was found and removed, and returning false
     * if the key was not found and thus not removed.
    */
    bool Remove(const KeyType& key) {
        // get location of key in underlying vector
        // auto should typecast index to uint32
        auto index = GetIndex(key);
        MapList& list = storage_.get()[index];
        // iterate through vector
        for(auto it = list.begin();
            it != list.end(); ++it) {
            // declare var entry as what the iterator points to
            MapEntry& entry = *it;
            // if an entry has the key we are looking for, erase it
            if(key_comparer_(entry.first, key)) {
                list.erase(it);
                // decrement size after erasing
                --size_;
                // return true because entry for key was found and erased
                return true;
            }
        }
        /*
         * return false if entry for key was not found. Nothing erased, size
         * remains the same
         */
         return false;
    }
    /*
     * The Maybe type presents a useful boolean that indicates presence, and
     * also a pointer to the value if present, and a null pointer otherwise.
     */
    Maybe<ValueType> Get(const KeyType& key) const {
        // creates and defines index variable, which is the index of the key
        auto index = GetIndex(key);
        // declare a MapList type list,
        MapList& list = storage_.get()[index];
        for(auto it = list.begin();
            it != list.end(); ++it) {
            MapEntry& entry = *it;
            // if key exists, return corresponding value
            if(key_comparer_(entry.first, key)) {
                return Maybe<ValueType>(entry.second);
            }
        }
        // return empty Maybe object if key is not found
        return Maybe<ValueType>();
    }
private:

    /*
     * This is the magic that bridges the map datastructure to the underlying vector
     * Vectors work by indexes, so we need a way to get the index of a value,
     * given its key.
     */

    uint32_t GetIndex(const KeyType& key) const {
        /* The hashing is what reduces the lookup of a value in an array O(n) to
         * the O(1) lookup for HashMaps that we have been taught.
         */
        uint32_t hash = hash_calculator_(key);
        /* the hash function should be written so the hash is an indicator of place
         * in underlying vector. So we now use hash to quickly calculate the index,
         * rather than looking through sequentially.
         */
        uint32_t index = hash % capacity_;
        // return the index so the value can be looked up O(1) from the array/vector
        return index;
    }
};

}  // namespace map
}  // namespace data_structures

#endif //DOCUMENTS_MAP_IMPL_H
