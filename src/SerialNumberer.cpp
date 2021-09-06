//
// Created by Kyle Gendreau on 8/9/21.
//

#ifndef SERIALNUMBERER_SERIALNUMBERER_CPP
#define SERIALNUMBERER_SERIALNUMBERER_CPP

#include <ios>
#include <sstream>
#include "../include/SerialNumberer.h"

template<typename T>
SerialNumberer<T>::SerialNumberer() : serialToItem_(), itemToSerial_(), count_(0) {
}

template<typename T>
SerialNumberer<T>::SerialNumberer(const SerialNumberer &rhs) : serialToItem_(rhs.serialToItem_),
                                                               itemToSerial_(rhs.itemToSerial_), count_(0) {
}

template<typename T>
SerialNumberer<T> &SerialNumberer<T>::operator=(const SerialNumberer &rhs) {
    if (this != &rhs) {
        SerialNumberer<T> ph(rhs);
        swap(ph);
    }
    return *this;
}

template<typename T>
bool SerialNumberer<T>::operator==(const SerialNumberer &rhs) {
    return this == &rhs || serialToItem_ == rhs.serialToItem_;
}

template<typename T>
bool SerialNumberer<T>::operator!=(const SerialNumberer &rhs) {
    return !(*this == rhs);
}

template<typename T>
bool SerialNumberer<T>::empty() const noexcept {
    return size() == 0;
}

template<typename T>
uint32_t SerialNumberer<T>::size() const noexcept {
    return itemToSerial_.size();
}

template<typename T>
uint32_t SerialNumberer<T>::hashCount() const noexcept {
    return count_;
}

template<typename T>
void SerialNumberer<T>::clear() noexcept {
    if (size() != 0) {
        SerialNumberer<T> ph;
        *this = ph;
    }
}

template<typename T>
typename SerialNumberer<T>::serial SerialNumberer<T>::insert(T &item) {
    serial s;
    if (!contains(item)) {
        s = generateSerial(item);
        itemToSerial_.insert(std::pair<T *, serial>(&item, s));
        serialToItem_.insert(std::pair<serial, T *>(s, &item));
    } else {
        s = getSerial(item).first;
    }
    return s;
}

template<typename T>
typename SerialNumberer<T>::serial SerialNumberer<T>::insert(T &&item) {
    return insert(item);
}

template<typename T>
void SerialNumberer<T>::erase(T &item) {
    serial s(itemToSerial_.at(item));
    itemToSerial_.erase(&item);
    serialToItem_.erase(s);
}

template<typename T>
void SerialNumberer<T>::erase(SerialNumberer::serial s) {
    itemToSerial_.erase(serialToItem_.at(s));
    serialToItem_.erase(s);
}

template<typename T>
bool SerialNumberer<T>::contains(const T &item) const {
    return itemToSerial_.contains(&(const_cast<T &>(item))); // i legit have no clue
}

template<typename T>
bool SerialNumberer<T>::contains(const SerialNumberer::serial s) const {
    return serialToItem_.contains(s);
}

template<typename T>
std::pair<typename SerialNumberer<T>::serial, bool> SerialNumberer<T>::getSerial(T &item) const {
    return (contains(item)) ?
           std::pair<typename SerialNumberer<T>::serial, bool>(itemToSerial_.at(&item), true) :
           std::pair<typename SerialNumberer<T>::serial, bool>(typename SerialNumberer<T>::serial(), false);
}

template<typename T>
std::pair<typename SerialNumberer<T>::serial, bool> SerialNumberer<T>::operator[](const T &item) const {
    return getSerial(item);
}

template<typename T>
std::pair<const T *, bool> SerialNumberer<T>::getItem(const SerialNumberer::serial s) {
    return (serialToItem_.contains(s)) ?
           std::pair<const T *, bool>(serialToItem_.at(s), true) :
           std::pair<const T *, bool>(nullptr, false);
}

template<typename T>
std::pair<const T *, bool> SerialNumberer<T>::operator[](const SerialNumberer::serial s) {
    return getItem(s);
}

template<typename T>
typename SerialNumberer<T>::serial SerialNumberer<T>::generateSerial(const T &item) {
    ++count_;
    if (count_ == 0)
        throw std::logic_error("too many inserts, reverted count_ to zero");
    std::stringstream stream;
    stream << std::hex << count_;
    return stream.str();
}

#endif // SERIALNUMBERER_SERIALNUMBERER_CPP