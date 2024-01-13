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
                mPointer = obj;
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
                mPointer = nullptr;
                mTail = nullptr;
            }
            else
            {
                mPointer = mPointer->next;
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
            T* ret = mPointer;
            mPointer = mPointer->next;
            //mTail->next = mPointer;
            return ret;
        }

    private:
        T* mPointer;
        T* mTail;
    };
}

#endif // VIPEROS_ECHIS_STD_CONTAINER_RING_H