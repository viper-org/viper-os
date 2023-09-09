#ifndef VIPER_OS_CONTAINER_LIST_H
#define VIPER_OS_CONTAINER_LIST_H 1

#include <memory.h>

namespace vpr
{
    template <class T>
    class list
    {
    struct node
    {
        node(T data, node* next)
            : data(data)
            , next(next)
        {
        }

        T data;
        node* next { nullptr };
    };
    public:
        list()
            : mRoot(nullptr)
        {
        }

        list(list&& other)
            : mRoot(other.mRoot)
        {
            other.mRoot = nullptr;
        }

        ~list()
        {
            node* currentNode = mRoot;
            while(currentNode && currentNode->next)
            {
                node* next = currentNode->next;
                delete currentNode;
                currentNode = next;
            }
        }


        class iterator
        {
        friend class list;
        public:
            T operator*()
            {
                return mNode->data;
            }

            T operator->()
            {
                return mNode->data;
            }

            iterator& operator++()
            {
                mNode = mNode->next;
                return *this;
            }

            bool operator==(const iterator& other) const
            {
                return (mNode == other.mNode);
            }

            bool operator!=(const iterator& other) const
            {
                return (mNode != other.mNode);
            }
        private:
            iterator(list::node* node)
                : mNode(node)
            {
            }

            list::node* mNode;
        };
        

        iterator begin()
        {
            return iterator(mRoot);
        }

        iterator end()
        {
            return iterator(nullptr);
        }

        void push_back(T data)
        {
            node* newNode = new node(data, mRoot);
            mRoot = newNode;
        }
        
    private:
        node* mRoot;
    };
}

#endif // VIPER_OS_CONTAINER_LIST_H