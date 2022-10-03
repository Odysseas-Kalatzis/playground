#include <vector>
#include <memory>
#include "undo_redo.h"

using namespace ur;

base_operation::base_operation(bool state) : m_state(state) {}
void base_operation::invert() { m_state = !m_state; }


undo_redo::undo_redo(size_t const capacity) : 
    m_undo_head(0), m_undo_tail(0), m_undo_size(0), 
    m_redo_head(0), m_redo_tail(0), m_redo_size(0), m_capacity(capacity)  
{
    m_undo_buf.resize(m_capacity);
    m_redo_buf.resize(m_capacity);
}


void undo_redo::undo_stack(std::unique_ptr<base_operation> op)
{
    ++m_undo_size;
    
    // Increasing the head and tail of our circular buffer
    // when we have more than one element.
    if (m_undo_size > 1)
    {
        ++m_undo_head %= m_capacity;
        m_undo_tail = (m_undo_tail + (m_undo_tail == m_undo_head)) % m_capacity;
    }

    m_undo_buf[m_undo_head] = std::move(op);
}

void undo_redo::undo_pop()
{
    // Make sure our buffer is not empty.
    if (m_undo_size > 0)
    {
        // Invoke the operation.
        (*m_undo_buf[m_undo_head])(); 

        // Send inverted functor to redo buffer.
        m_undo_buf[m_undo_head]->invert();
        redo_stack(std::move(m_undo_buf[m_undo_head])); 

        --m_undo_size;

        // Decrement the head of our circular buffer 
        // when we have more than one element.
        if (m_undo_size > 1)
        {
            // Move head (note that tail cannot move during pop).
            (m_undo_head > 0) ? --m_undo_head : m_undo_head = m_capacity - 1;
        }
    }
}

void undo_redo::redo_pop()
{
    // Make sure our buffer is not empty.
    if (m_redo_size > 0)
    {
        // Invoke the operation.
        (*m_redo_buf[m_redo_head])(); 

        // Send inverted functor to undo buffer.
        m_redo_buf[m_redo_head]->invert();
        undo_stack(std::move(m_redo_buf[m_redo_head])); 

        --m_redo_size;

        // Decrement the head of our circular buffer 
        // when we have more than one element.
        if (m_redo_size > 1)
        {
            // Move head (note that tail cannot move during pop).
            (m_redo_head > 0) ? --m_redo_head : m_redo_head = m_capacity - 1;
        }
    }
}

void undo_redo::redo_stack(std::unique_ptr<base_operation> op)
{
    ++m_redo_size;
    
    // Increasing the head and tail of our circular buffer
    // when we have more than one element.
    if (m_redo_size > 1)
    {
        ++m_redo_head %= m_capacity;
        m_redo_tail = (m_redo_tail + (m_redo_tail == m_redo_head)) % m_capacity;
    }

    m_redo_buf[m_redo_head] = std::move(op);
}


