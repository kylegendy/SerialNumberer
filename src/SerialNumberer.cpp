//
// Created by Kyle Gendreau on 8/9/21.
//

#ifndef SERIALNUMBERER_SERIALNUMBERER_CPP
#define SERIALNUMBERER_SERIALNUMBERER_CPP

#include <ios>
#include <sstream>
#include "../include/SerialNumberer.h"

template<typename T>
SerialNumberer<T>::SerialNumberer() : serialToItem_(), itemToSerial_(), itemCount_(0), overCount_(false) {
}

template<typename T>
SerialNumberer<T>::SerialNumberer(const SerialNumberer &rhs) : serialToItem_(rhs.serialToItem_), itemToSerial_(rhs.itemToSerial_), itemCount_(0), overCount_(rhs.overCount) {
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
    return this == &rhs || (serialToItem_ == rhs.serialToItem_ && itemToSerial_ == rhs.itemToSerial_);
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
bool SerialNumberer<T>::overSize() const noexcept {
    return overCount_;
}

template<typename T>
void SerialNumberer<T>::clear() noexcept {
    if (size() != 0) {
        uint32_t cnt = itemCount_;
        *this = SerialNumberer<T>();
        itemCount_ = cnt;
    }
}

template<typename T>
typename SerialNumberer<T>::serial SerialNumberer<T>::insert(T &item) {
    serial s;
    if (!contains(item)) {
        s = generateSerial(item);
        serialToItem_.insert(std::pair<serial,T>(s,item));
        itemToSerial_.insert(std::pair<T,serial>(item,s));
    }
    else {
        s = itemToSerial_.at(item);
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
    itemToSerial_.erase(item);
    serialToItem_.erase(s);
}

template<typename T>
void SerialNumberer<T>::erase(T &&item) {
    erase(item);
}

template<typename T>
void SerialNumberer<T>::erase(SerialNumberer::serial s) {
    itemToSerial_.erase(serialToItem_.at(s));
    serialToItem_.erase(s);
}

template<typename T>
bool SerialNumberer<T>::contains(const T &item) const {
    return itemToSerial_.contains(item);
}

template<typename T>
bool SerialNumberer<T>::contains(const SerialNumberer::serial s) const {
    return contains(serialToItem_.at(s));
}

template<typename T>
std::pair<typename SerialNumberer<T>::serial,bool> SerialNumberer<T>::getSerial(const T &item) const {
    return (itemToSerial_.contains(item)) ?
        std::pair<typename SerialNumberer<T>::serial,bool>(itemToSerial_.at(item),true) :
        std::pair<typename SerialNumberer<T>::serial,bool>(typename SerialNumberer<T>::serial(),false);
}

template<typename T>
std::pair<T&,bool> SerialNumberer<T>::getItem(const SerialNumberer::serial s) {
    return (serialToItem_.contains(s)) ?
        std::pair<T&,bool>(serialToItem_.at(s),true) :
        std::pair<T&,bool>(T(),false);
}

template<typename T>
typename SerialNumberer<T>::serial SerialNumberer<T>::generateSerial(const T& item) {
    std::ostringstream s;
    size_t hsh(std::hash<T>{}(item));
    s << std::hex << itemCount_ << "-";
    s << std::hex << hsh;

    if (itemCount_ == 4294967296)
        overCount_ = true;

    ++itemCount_;
    return s.str();
}

#endif // SERIALNUMBERER_SERIALNUMBERER_CPP