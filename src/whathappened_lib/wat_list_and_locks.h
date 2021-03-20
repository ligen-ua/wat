#pragma once
#include "wat_std.h"

namespace wat
{

struct base_node
{
    base_node * pNext;
    base_node()
        :
            pNext(0)
    {
    }
};

class token_list_impl
{
    token_list_impl(const token_list_impl &);
    token_list_impl & operator = (const token_list_impl &);

protected:
    base_node   m_rootNode;
    base_node * m_pFirst;
    base_node * m_pLast;
    long m_rootNodeInList;
public:
    token_list_impl()
        :  m_rootNodeInList(1)
    {
        m_rootNode.pNext = 0;
        m_pFirst = m_pLast = &m_rootNode;
    }
    void push_back(base_node * pNode)
    {
        pNode->pNext = 0;
        base_node * pOldLast = exchange(&m_pLast, pNode);
        exchange(&pOldLast->pNext, pNode);
    }
    base_node * pop_front(bool popRootNode)
    {
        // single thread mode, caller should be locked
        while(1)
        {
            if (!m_pFirst->pNext)
            {
                if (m_pFirst == &m_rootNode)
                {
                    return 0;
                }
                if (!m_rootNodeInList)
                {
                    m_rootNodeInList = 1;
                    push_back(&m_rootNode);
                    while (!m_pFirst->pNext) {}
                }                
            }
            base_node * pTop = exchange(&m_pFirst, m_pFirst->pNext);
            if (pTop == &m_rootNode)
            {
                m_rootNodeInList = 0;
                if (popRootNode)
                {
                    pTop->pNext = 0;
                    return pTop;
                }
            }
            else
            {
                if (popRootNode)
                {
                    throw std::runtime_error("RootNode changed");
                }
                pTop->pNext = 0;             
                return pTop;
            }  

        }
    }
};


class token_list_lock:private token_list_impl
{
    base_node * m_pRootLock;
public:
    token_list_lock()
       : m_pRootLock(0)
    {
    }
    void lock(base_node * pThreadNode)
    {
        push_back(pThreadNode);
 
        while(1)
        {
            if (m_pFirst == pThreadNode)
               return;

            if (m_pFirst == &m_rootNode)
            {
               if (!compare_and_swap(&m_pRootLock, pThreadNode, (base_node *)0))
               {
                   if (m_pFirst == &m_rootNode)
                   {
                       pop_front(true);
                   }
                   exchange(&m_pRootLock, (base_node *)0);
               }
            }
        }
    }
    void unlock()
    {
        pop_front(false);
    }
};


template<>
class scoped_lock<token_list_lock>
{
    scoped_lock(const scoped_lock&);
    scoped_lock & operator = (const scoped_lock&);

    base_node m_threadNode;
    token_list_lock * m_pLock;
public:
    scoped_lock(token_list_lock * pLock)
       : m_pLock(pLock)
    {
       m_pLock->lock(&m_threadNode);
    }
    ~scoped_lock()
    {  
       m_pLock->unlock();
    }
};

template<class ChildType>
struct list_base_node:public base_node
{
    ChildType * object_ptr;
    list_base_node(ChildType * object_ptr_in)
        :
            object_ptr(object_ptr_in)
    {
    }
};
template<class TargetType>
class intrusive_list
{
    token_list_impl m_listImpl;
    token_list_lock m_popLock;
public:
    intrusive_list()
    {
    }
    void push_back(list_base_node<TargetType> * pNode)
    {
       m_listImpl.push_back(pNode);
    }
    TargetType * pop_front()
    {
       scoped_lock<token_list_lock> guard(&m_popLock);
       list_base_node<TargetType> * pnode = static_cast<list_base_node<TargetType> * >(m_listImpl.pop_front(false));
       if (!pnode)
       {
           return 0;
       }
       return pnode->object_ptr;
    }
};


template<class Type>
struct value_node:public base_node
{
    Type m_value;
    value_node(const Type & value)
        : 
            m_value(value)
    {
    }
};
template<class ValueType>
class list
{
    typedef value_node<ValueType> Node_type;
    intrusive_list<Node_type> m_list;
public:
    list()
    {
    }
    void push_back(const ValueType & object)
    {
        m_list.push_back(new Node_type(object));
    }
    bool pop_front(ValueType * pResult)
    {
        Node_type * pNode = m_list.pop_front();
        if (!pNode)
            return false;

        WAT_AUTO_PTR<Node_type> guard(pNode);
        *pResult = pNode->m_value;
        return true;
    }
};

} // namespace

