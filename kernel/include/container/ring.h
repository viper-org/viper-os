#include "drivers/Terminal.h"
#ifndef VIPER_OS_CONTAINER_RING_H
#define VIPER_OS_CONTAINER_RING_H 1

#include <memory.h>

namespace vpr
{
    template <class T>
    class ring
    {
        struct node
        {
            T data;
            node* next;
            node* previous;
        };
    public:
        ring()
        {
        }

        ~ring()
        {
            node* start = mPointer;

            node* current = mPointer;
            do
            {
                node* next = current->next;
                
                delete current;

                current = next;
            }
            while (current != start);
        }


        T& current()
        {
            return mPointer->data;
        }

        T& next()
        {
            T ret = mPointer->data;
            mPointer = mPointer->next;
            return ret;
        }


        void push_back(T object)
        {
            if (!mPointer) // First object being added
            {
                node* newNode = new node;
                newNode->data = object;
                newNode->next = newNode;
                newNode->previous = newNode;

                mPointer = newNode;

                return;
            }
            node* previous = mPointer->previous;

            node* newNode = new node(object, mPointer, previous);

            previous->next     = newNode;
            mPointer->previous = newNode;
        }

    private:
        node* mPointer;
    };
}

#endif // VIPER_OS_CONTAINER_RING_H