#pragma once

#include <cstdint>
#include <assert.h>
#include <cstdio>
#include <cstring>

//! Simple open addressing hash map. It is not a set. Same keys will be added multiple times, but you will only be able access the first occurrence.
//! note: Constructor is not called for the stored type!
template <typename Type>
class HashMap
{
public:
  //! holds the actual value and full flag
  struct Slot
  {
    //! initializes the full flag to false, this is actually not called as we don't call the constructor
    Slot() : _full(false) {}
    Type _value;
    bool _full;
  };

  //! Initializes the hashmap to initial capacity and zeroes out data
  HashMap()
  {
    _size = 0;
    _capacity = 32;
    _data = reinterpret_cast<Slot*>(malloc(sizeof(*_data) * _capacity));
    memset(_data, 0, sizeof(*_data) * _capacity);
  }

  ~HashMap()
  {
    free(_data);
  }

  //! Rebuilds the whole hashmap to a new one of a new size
  void Rebuild(size_t newSize)
  {
    assert(newSize > _size);
    Slot* oldData = _data;
    size_t oldSize = _capacity;
    _capacity = newSize;
    _size = 0;
    _data = reinterpret_cast<Slot*>(malloc(sizeof(*_data) * _capacity));
    memset(_data, 0, sizeof(*_data) * _capacity);

    // rehash all values
    for (size_t i = 0; i < oldSize; i++)
    {
      if (oldData[i]._full)
        AddWithoutCheck(oldData[i]._value);
    }
    free(oldData);
  }

  //! Adds a new element by copy
  void Add(const Type& val)
  {
    CheckSize();
    AddWithoutCheck(val);
  }

  //! Retrieves a value by reference for the given key (encapsulated by the Type itself), null reference returned if key not found
  Type& Get(const Type& val)
  {
    // size must be 2^k
    size_t pos = static_cast<size_t>(val.GetHashCode() * 59) & (_capacity - 1);

    assert(_size < _capacity);
    while (_data[pos]._full)
    {
      if (_data[pos]._value.Compare(val))
      {
        return _data[pos]._value;
      }
      pos++;
      if (pos == _capacity)
      {
        pos = 0;
      }
    }
    return _null._value;
  }

  //! Returnes whether the given value reference is the null value, indicating that the value is invalid (not found)
  bool NotNull(const Type& val)
  {
    return &val != &_null._value;
  }

  bool Compact()
  {
    if (_size * 5 < _capacity && _capacity > 64)
    {
      Rebuild(_capacity / 4);
      return true;
    }
    return false;
  }

  void Remove(const Type& val)
  {
    // size must be 2^k
    size_t pos = static_cast<size_t>(val.GetHashCode() * 59) & (_capacity - 1);

    assert(_size < _capacity);
    while (_data[pos]._full)
    {
      if (_data[pos]._value.Compare(val))
      {
        _data[pos]._full = false;
        _size--;
        if (!Compact())
        {
          RehashNextFromPos(pos);
        }
        return;
      }
      pos++;
      if (pos == _capacity)
      {
        pos = 0;
      }
    }
  }

  size_t GetSize() const { return _size; }
private:
  void CheckSize()
  {
    if (_size * 3 > _capacity * 2)
    {
      Rebuild(_capacity * 4);
    }
  }


  void RehashNextFromPos(size_t pos)
  {
    pos++;
    if (pos == _capacity)
    {
      pos = 0;
    }
    assert(_size < _capacity);
    while (_data[pos]._full)
    {
      _data[pos]._full = false;
      _size--;
      AddWithoutCheck(_data[pos]._value);
      pos++;
      if (pos == _capacity)
      {
        pos = 0;
      }
    }
  }

  void AddWithoutCheck(const Type& val)
  {
    // size must be 2^k
    size_t pos = static_cast<size_t>(val.GetHashCode() * 59) & (_capacity - 1);

    assert(_size < _capacity);
    while (_data[pos]._full)
    {
      pos++;
      if (pos == _capacity)
      {
        pos = 0;
      }
    }
    _data[pos]._value = val;
    _data[pos]._full = true;
    _size++;
  }

  Slot _null;
  Slot* _data;
  size_t _capacity;
  size_t _size;
};
