#ifndef VIPEROS_ECHIS_STD_CONTAINER_VECTOR_H
#define VIPEROS_ECHIS_STD_CONTAINER_VECTOR_H 1

#include <std/utility.h>

#include <stddef.h>
#include <string.h>

namespace vpr
{
    template <class T>
    class vector
    {
    public:
        using reference = T&;
        using const_reference = const T&;

        vector()
            : mData(nullptr)
            , mSize(0)
            , mCapacity(0)
        {
        }

        vector(const vector& other)
            : mSize(other.mSize)
            , mCapacity(other.mCapacity)
        {
            mData = new T[mCapacity];
            memcpy(mData, other.mData, mSize * sizeof(T));
        }

        vector(vector&& other)
            : mData(vpr::move(other.mData))
            , mSize(vpr::move(other.mSize))
            , mCapacity(vpr::move(other.mCapacity))
        {
            other.mData     = nullptr;
            other.mSize     = 0;
            other.mCapacity = 0;
        }

        ~vector()
        {
            if (mData)
            {
                delete mData;
            }
        }


        reference operator[](size_t pos)
        {
            return mData[pos];
        }

        const_reference operator[](size_t pos) const
        {
            return mData[pos];
        }


        reference front()
        {
            return mData[0];
        }

        const_reference front() const
        {
            return mData[0];
        }

        reference back()
        {
            return mData[mSize - 1];
        }

        const_reference back() const
        {
            return mData[mSize - 1];
        }


        T* data()
        {
            return mData;
        }

        const T* data() const
        {
            return mData;
        }


        class iterator
        {
            friend class vector;

            iterator(T* data)
                : mData(data)
            {
            }

        public:
            iterator& operator++()
            {
                ++mData;
                return *this;
            }
            iterator operator++(int)
            {
                iterator temp = *this;
                ++mData;
                return temp;
            }

            iterator& operator--()
            {
                --mData;
                return *this;
            }
            iterator operator--(int)
            {
                iterator temp = *this;
                --mData;
                return temp;
            }

            bool operator==(const iterator& other) const
            {
                return mData == other.mData;
            }

            T& operator*()
            {
                return *mData;
            }

        private:
            T* mData;
        };

        iterator begin()
        {
            return iterator(mData);
        }

        iterator end()
        {
            return iterator(&mData[mSize]);
        }


        bool empty() const
        {
            return mSize == 0;
        }

        size_t size() const
        {
            return mSize;
        }

        size_t capacity() const
        {
            return mCapacity;
        }


        void clear()
        {
            delete[] mData;
            mData = nullptr;
            mSize = 0;
            mCapacity = 0;
        }

        void push_back(const T& value)
        {
            if (mCapacity > mSize)
            {
                mData[mSize++] = value;
            }
            else if (!mData)
            {
                mData = new T[1];
                mData[0] = value;
                mSize = 1;
                mCapacity = 1;
            }
            else
            {
                T* newData = new T[mCapacity * 2];
                for (size_t i = 0; i < mSize; ++i)
                {
                    newData[i] = vpr::move(mData[i]);
                }
                delete[] mData;
                mData = newData;
                mData[mSize++] = value;
                mCapacity *= 2;
            }
        }

    private:
        T* mData;
        size_t mSize;
        size_t mCapacity;
    };
}

#endif // VIPEROS_ECHIS_STD_CONTAINER_VECTOR_H