#include <vector>
#include <memory>

// Every operation fit for undo-redo will inherit from base_operation.
namespace ur
{
class base_operation
{
protected:
    bool m_state;
public:
    base_operation(bool state);
    virtual ~base_operation() = default;
    virtual void operator()() = 0;
    void invert();
};


class undo_redo
{
private:
    std::vector<std::unique_ptr<base_operation>> m_undo_buf, m_redo_buf;
    size_t m_undo_head, m_undo_tail, m_undo_size;
    size_t m_redo_head, m_redo_tail, m_redo_size;
    size_t const m_capacity;

    // redo_stack is private as it can only be called by undo_pop()
    void redo_stack(std::unique_ptr<base_operation> op);

public:
    undo_redo(size_t const capacity); 
    void undo_stack(std::unique_ptr<base_operation> op);
    void undo_pop();
    void redo_pop();
};

}

