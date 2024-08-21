#ifndef VIPEROS_ECHIS_INTRUSIVE_LIST_H
#define VIPEROS_ECHIS_INTRUSIVE_LIST_H 1

template <class T>
class intrusive_list
{
public:
    intrusive_list()
        : mHead(nullptr)
        , mTail(nullptr)
    {
    }

    T* head() const
    {
        return mHead;
    }

    T* tail() const
    {
        return mTail;
    }

    void push_back(T* elem)
    {
        if (!mHead)
        {
            mHead = elem;
            mTail = elem;
        }
        else
        {
            mTail->next = elem;
            elem->prev = mTail;
            mTail = elem;
        }
    }

    void push_front(T* elem)
    {
        if (!mHead)
        {
            mHead = elem;
            mTail = elem;
        }
        else
        {
            mHead->prev = elem;
            elem->next = mHead;
            mHead = elem;
        }
    }

    void pop_front()
    {
        T* head = mHead;
        mHead = head->next;
        if (mHead)
            mHead->prev = nullptr;
        else
        {
            mTail = nullptr;
        }
        head->next = nullptr;
        head->prev = nullptr;
    }

    class iterator
    {
    public:
        T& operator*()
        {
            return *mElem;
        }

        T* operator->()
        {
            return mElem;
        }

        iterator& operator++()
        {
            mElem = mElem->next;
            return *this;
        }

        iterator& operator--()
        {
            mElem = mElem->prev;
            return *this;
        }

        bool operator==(const iterator& other) const
        {
            return mElem == other.mElem;
        }

    private:
        iterator(T* elem)
            : mElem(elem)
        {
        }
    
        T* mElem;
    };

private:
    T* mHead;
    T* mTail;
};

#endif // VIPEROS_ECHIS_INTRUSIVE_LIST_H