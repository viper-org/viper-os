#ifndef VIPEROS_ECHIS_STD_CONTAINER_RING_H
#define VIPEROS_ECHIS_STD_CONTAINER_RING_H 1

namespace vpr
{
    template <class T>
    class intrusive_ring
    {
    public:
        intrusive_ring()
            : mPointer(nullptr)
        {
        }

        void push(T* obj)
        {
            if (mPointer)
            {
                obj->next = mPointer;
                mTail->next = obj;
                mTail = obj;
            }
            else
            {
                mPointer = obj;
                mTail = obj;
                obj->next = obj;
            }
        }

        void remove()
        {
            if (mPointer->next == mPointer)
            {
                mPointer->next = nullptr;
                mPointer = nullptr;
                mTail = nullptr;
            }
            else
            {
                T* temp = mPointer->next;
                mPointer->next = nullptr;
                mPointer = temp;
                mTail->next = mPointer;
            }
        }

        T* current()
        {
            return mPointer;
        }

        T* peek()
        {
            return mPointer->next;
        }

        T* next()
        {
            mTail = mPointer;
            mPointer = mPointer->next;
            return mTail;
        }

    private:
        T* mPointer;
        T* mTail;
    };
}

#endif // VIPEROS_ECHIS_STD_CONTAINER_RING_H