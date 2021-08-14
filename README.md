# Serial Numberer
## Table of Contents

1. [Objective](#objective)
2. [Outline](#outline)
   1. [General](#general)
   2. [SerialNumberer](#serialnumberer)

## Objective
Create a method of making a unique and PERSISTENT hash for an object that can be copiable.  Ie, say you have a
container of objects, and you wanted them all to have a unique identifier.  

An obvious solution is to simply use pointers, or use the std::hash operation provided by the standards committee.  
THE PROBLEM:
But imagine a scenario in which objects outside of this container need to reference items within the container, and now 
you would like to create a copy of the entire environment.  We could first make a copy of the container and its objects, 
and then of the objects outside of the container, but now how do we deal with references to the items of the container?  
If we used pointers, we would have to find the new address of the duplicate and replace the addresses of all copied 
pointers.  If we used std::hash, its template parameter type would have to be that of a pointer (else we would need to 
create a new hash function for every class, and this is obviously not feasible), and therefore we'd run into the same 
problem we would have with pointers.
So, this simple class solves this by providing an interface to using a dictionary of sorts.

## Outline

### General
Allow objects of any type T to be inserted and generate a unique serial number that can be specific to the object. 
Additionally, be able to quickly identify an object by its serial number and vice-versa (ie identify an objects 
serial number) if present within the container.

### SerialNumberer

##### Member Functions:
signature | description
----------|-----------
explicit SerialNumberer(const SerialNumberer& rhs); | copy constructor
SerialNumberer& operator=(const SerialNumberer& rhs); | assignment operator, copies all objects and serial numbers
bool operator==(const SerialNumberer& rhs); | equivalence operator, checks logical equivalence
bool operator!=(const SerialNumberer& rhs); | negated equivalence operator, returns the opposite of operator==()
bool empty() const noexcept; | returns true if container is empty, else false
uint32_t size() const noexcept; | returns the number of unique items (ie the number of serial numbers) within the container
bool overSize() const noexcept; | returns true if internal variables have surpassed a certain point (if total inserted surpasses maximum of type uint32_t)
void clear() noexcept; | empties the container
serial insert(...param...); parameters include: T& OR T&& | inserts a new item if not already present and returns its serial number
void erase(...param...); parameters include: T& OR T&& OR std::string | deletes an item and corresponding serial number (or vice-versa) if present in container
bool contains(...param...) const; parameters include: const T& OR const std::string | returns true if present within container
std::pair<serial,bool> getSerial(const T& item) const; | returns pair<string,bool> where the second signals if present within container or not, and the first being the serial number specific to the item parameter
std::pair<T&,bool> getItem(const serial s); | returns pair<T&,bool> where the second signals if present within container or not, and the first being the item at the given serial number

