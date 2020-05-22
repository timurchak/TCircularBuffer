#ifndef TRING_BUFFER_H
#define TRING_BUFFER_H

#include <vector>

template <class T> class TRingBuffer {
public:
    TRingBuffer();
    explicit TRingBuffer(int _capasity);
    TRingBuffer(const TRingBuffer<T>& other);
    TRingBuffer<T>& operator=(const TRingBuffer<T>& other);
    ~TRingBuffer();
    TRingBuffer(TRingBuffer<T>&& other) noexcept;
    TRingBuffer<T>& operator=(TRingBuffer<T>&& other) noexcept;
    T& operator[](int i) { return data[calcIndex(i)]; }

    template <class E> void addRange(std::vector<E> arr);
    template <class E> void addRange(E* arr, int N);
    template <class E> void addItem(E val);

    std::vector<T> toStdVector();
    void clear();
    void clear(int newCapasity);
    T lastValue(bool* ok = nullptr);
    T lastValue(int i, bool* ok = nullptr);
    T lastValueIndex(int i);
    std::vector<T> lastValueStdVector(int i, bool* ok = nullptr);
    T& at(int i);
    T& at(int i) const;

    int getCapacity() const;
    void setCapacity(int value);
    int getCurrentIndex() const;
    int getFirstIndex() const;
    int getLastIndex() const;
    bool getCircular() const;
    int getCount() const;
    T* getData() const;

protected:
    void changeIndex();
    int calcIndex(int i);
    void resize(int value);

protected:
private:
    void init(int value);

private:    
    int capacity;
    T* data;
    int currentIndex;
    int firstIndex;
    int lastIndex;
    bool circular;
    int count;
};

template <class T>
TRingBuffer<T>::TRingBuffer()
    : capacity(5000)
    , data(new T[capacity])
    , currentIndex(0)
    , firstIndex(0)
    , lastIndex(0)
    , circular(false)
    , count(0)    
{
    init(capacity);
}

template <class T>
TRingBuffer<T>::TRingBuffer(int _capasity)
    : capacity(_capasity)
    , data(new T[_capasity])
    , currentIndex(0)
    , firstIndex(0)
    , lastIndex(0)
    , circular(false)
    , count(0)
{
    init(capacity);
}

template <class T> TRingBuffer<T>::TRingBuffer(const TRingBuffer<T>& other)
    : capacity(other.capacity)
    , data(new T[other.capacity])
    , currentIndex(other.currentIndex)
    , firstIndex(other.firstIndex)
    , lastIndex(other.lastIndex)
    , circular(other.circular)
    , count(other.count)
{
    std::copy(other.data, other.data + capacity, data);
}

template <class T> TRingBuffer<T>& TRingBuffer<T>::operator=(const TRingBuffer& other)
{
    if(this == &other) {
        return *this;
    }
    capacity = other.capacity;
    data = new T[other.capacity];
    std::copy(other.data, other.data + capacity, data);
    currentIndex = other.currentIndex;
    firstIndex = other.firstIndex;
    lastIndex = other.lastIndex;
    circular = other.circular;
    count = other.count;
    return *this;
}

template<class T> TRingBuffer<T>& TRingBuffer<T>::operator=(TRingBuffer<T> &&other) noexcept
{
    if(this == &other) {
        return *this;
    }
    capacity = other.capacity;
    delete [] data;
    data = other.data;
    other.data = nullptr;
    currentIndex = other.currentIndex;
    firstIndex = other.firstIndex;
    lastIndex = other.lastIndex;
    circular = other.circular;
    count = other.count;
    return *this;
}

template<class T> TRingBuffer<T>::TRingBuffer(TRingBuffer<T>&& other) noexcept
    : data(nullptr)
    , capacity(other.capacity)
    , circular(other.circular)
    , count(other.count)
    , currentIndex(other.currentIndex)
    , firstIndex(other.firstIndex)
    , lastIndex(other.lastIndex)
{
    data = other.data;
    other.data = nullptr;
    other.capacity = 0;
}

template <class T> TRingBuffer<T>::~TRingBuffer()
{
    if (!(data == nullptr)) {
        delete[] data;
    }
}

template <class T> void TRingBuffer<T>::init(int value)
{
    for (int i = 0; i < value; i++) {
        data[i] = 0;
    }
}

template <class T> template <class E> void TRingBuffer<T>::addRange(std::vector<E> arr)
{
    for (size_t i = 0; i < arr.size(); i++) {
        addItem(arr[i]);
    }
}

template <class T> template <class E> void TRingBuffer<T>::addRange(E* arr, int N)
{
    for (int i = 0; i < N; i++) {
        addItem(arr[i]);
    }
}

template <class T> template <class E> void TRingBuffer<T>::addItem(E val)
{
    data[currentIndex] = static_cast<T>(val);
    if (!circular) {
        count++;
    } else {
        count = capacity;
    }
    changeIndex();
}

template <class T> void TRingBuffer<T>::changeIndex()
{
    if (currentIndex >= capacity - 1) {
        currentIndex = 0;
        circular = true;
        lastIndex = capacity - 1;
    } else {
        currentIndex++;
        lastIndex = currentIndex - 1;
    }
    if (circular) {
        firstIndex = currentIndex;
    }
}

template <class T> int TRingBuffer<T>::calcIndex(int i)
{
    int index;
    if (i >= (capacity - firstIndex)) {
        index = i - (capacity - firstIndex);
    } else {
        index = firstIndex + i;
    }
    return index;
}

template <class T> void TRingBuffer<T>::clear()
{
    delete[] data;
    data = new T[capacity];
    currentIndex = 0;
    firstIndex = 0;
    lastIndex = 0;
    count = 0;
}

template <class T> void TRingBuffer<T>::clear(int newCapasity)
{
    delete[] data;
    capacity = newCapasity;
    data = new T[capacity];
    currentIndex = 0;
    firstIndex = 0;
    lastIndex = 0;
    count = 0;
}

template <class T> void TRingBuffer<T>::resize(int value)
{
    delete[] data;
    data = new T[value];
    init(value);
}
template <class T> T* TRingBuffer<T>::getData() const { return data; }

template <class T> void TRingBuffer<T>::setCapacity(int value)
{
    T* bufferCopy = new T[capacity];
    for (int i = 0; i < capacity; i++) {
        bufferCopy[i] = data[i];
    }
    if (value > count) {
        resize(value);
        if (circular) {
            for (int i = 0; i < count; i++) {
                data[i] = bufferCopy[calcIndex(i)];
            }
        } else {
            for (int i = 0; i < count; i++) {
                data[i] = bufferCopy[i];
            }
        }
        currentIndex = count;
        firstIndex = 0;
        lastIndex = count - 1;
        circular = false;
    } else {
        resize(value);
        if (circular) {
            for (int i = 0; i < value; i++) {
                data[i] = bufferCopy[calcIndex((count - value) + i)];
            }
        } else {
            for (int i = 0; i < value; i++) {
                data[i] = bufferCopy[(count - value) + i];
            }
        }
        currentIndex = 0;
        firstIndex = 0;
        lastIndex = value - 1;
        circular = true;
    }
    capacity = value;
}

template <class T> T TRingBuffer<T>::lastValue(bool* ok)
{
    if (count > 0) {
        if(ok) {
            *ok = true;
        }  
        return data[lastIndex];
    }
    if(ok) {
        *ok = true;
    }  
    return 0;
}

template <class T> T TRingBuffer<T>::lastValue(int i, bool* ok)
{
    int index = (count - i) - 1;
    if (index > 0) {
        if(ok) {
            *ok = true;
        }        
        return data[calcIndex(index)];
    }
    if(ok) {
        *ok = true;
    }  
    return 0;
}

template <class T> T TRingBuffer<T>::lastValueIndex(int i)
{
    int index = (count - i) - 1;
    if (index > 0) {
        return calcIndex(index);
    }
    return 0;
}

template <class T> std::vector<T> TRingBuffer<T>::lastValueStdVector(int i, bool* ok)
{
    std::vector<T> vecResult(i);
    int j = 0;
    for (int index = i - 1; index >= 0; index--) {
        vecResult[j++] = lastValue(index, ok);
    }
    return vecResult;
}

template <class T> T& TRingBuffer<T>::at(int i) { return data[calcIndex(i)]; }

template <class T> T& TRingBuffer<T>::at(int i) const { return data[calcIndex(i)]; }

template <class T> int TRingBuffer<T>::getCapacity() const { return capacity; }

template <class T> int TRingBuffer<T>::getCurrentIndex() const { return currentIndex; }

template <class T> int TRingBuffer<T>::getFirstIndex() const { return firstIndex; }

template <class T> int TRingBuffer<T>::getLastIndex() const { return lastIndex; }

template <class T> bool TRingBuffer<T>::getCircular() const { return circular; }

template <class T> int TRingBuffer<T>::getCount() const { return count; }

#endif // TRING_BUFFER_H
