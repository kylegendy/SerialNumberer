//
// Created by Kyle Gendreau on 8/9/21.
//

#ifndef SERIALNUMBERER_SERIALNUMBERER_H
#define SERIALNUMBERER_SERIALNUMBERER_H

#include <unordered_map>
#include <map>
#include <string>

template <typename T>
struct RefHash {
    std::size_t operator()(T* ptr) const {
        if (ptr == nullptr)
            throw std::logic_error("null at address");
        return (std::size_t)(&(*ptr));
    }
};

template <typename T>
class SerialNumberer {

    using serial = std::string;
    using Cont = std::unordered_map<serial,T*>;

public:

    // def ctor
    explicit SerialNumberer();

    // copy ctor
    explicit SerialNumberer(const SerialNumberer& rhs);

    // dtor
    ~SerialNumberer() = default;

    // assignment op
    SerialNumberer& operator=(const SerialNumberer& rhs);

    // logical equivalence op
    bool operator==(const SerialNumberer& rhs);

    // negated logical equivalence op
    bool operator!=(const SerialNumberer& rhs);

//////////////////////////////////////////////////////
//// CAPACITY

    /**
     * @return true if size() returns 0, else false
     */
    bool empty() const noexcept;

    /**
     * @return number of items held
     */
    uint32_t size() const noexcept;

    /**
     * @return the number of times serial numbers were created
     */
    uint32_t hashCount() const noexcept;

//////////////////////////////////////////////////////
//// MODIFIERS

    /**
     * replaces *this with a def ctor instance, maintaining count_ for persistent randomized serialization
     */
    void clear() noexcept;

    /**
     * inserts a new item (if not already present) and returns the paired serial number for it
     * @param item - the item being inserted
     * @return the new serial number for it
     */
    serial insert(T& item);
    serial insert(T&& item);

    /**
     * deletes item and corresponding serial number for it if present
     * @param item - the item being deleted
     */
    void erase(T& item);

    /**
     * deletes serial number and corresponding item if present
     * @param s - the serial number being deleted
     */
    void erase(serial s);

//////////////////////////////////////////////////////
//// LOOKUPS

    /**
     * checks if *this has item
     * @param item - the item being searched for
     * @return true if within *this, else false
     */
    bool contains(const T& item) const;

    /**
     * checks if *this has item at s
     * @param s - the serial number being searched for
     * @return true if within *this, else false
     */
    bool contains(const serial s) const;

    /**
     * finds the serial for an item if present
     * @param item - the item being searched for
     * @return pair of serial and bool, bool true if valid else false
     */
    std::pair<serial,bool> getSerial(T& item) const;
    std::pair<serial,bool> operator[](const T& item) const;

    /**
     * finds the item at the given serial number if present
     * @param s - the serial number being searched for
     * @return pair of item and bool, bool true if valid else false
     */
    std::pair<const T*,bool> getItem(const serial s);
    std::pair<const T*,bool> operator[](const serial s);

private:

    Cont serialToItem_;

    std::unordered_map<T*,serial,RefHash<T>> itemToSerial_;

    uint32_t count_;

    serial generateSerial(const T& item);

};

#include "../src/SerialNumberer.cpp"

#endif //SERIALNUMBERER_SERIALNUMBERER_H
