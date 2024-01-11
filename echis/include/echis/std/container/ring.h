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
                mPointer = obj;
            }
            else
            {
                mPointer = obj;
            }
        }

        void remove()
        {
            if (mPointer->next == mPointer)
            {
                mPointer = nullptr;
            }
            else
            {
                mPointer = mPointer->next;
            }
        }

        T* current()
        {
            return mPointer;
        }

        T* next()
        {
            return mPointer->next;
        }

    private:
        T* mPointer;
    };
}

#endif // VIPEROS_ECHIS_STD_CONTAINER_RING_H