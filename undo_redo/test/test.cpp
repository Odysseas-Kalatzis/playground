#include <iostream>
#include <vector>
#include <memory>
#include "undo_redo.h"

// All operations are represented as functors, this allows us to save an 
// internal state during construction and invoke via simply calling (),
// providing a universal interface agnostic of specific details.
struct add_sub : public ur::base_operation
{
    int m_x, m_y, m_z;
    add_sub(int x, int y, bool state=true) : 
        base_operation(state), m_x(x), m_y(y), m_z(0) {}

    void operator()() override
    {
        if (m_state)
        {
            std::cout << m_z << " += " <<  m_x << " + " << m_y;
            add();
            std::cout << " = " << m_z << '\n';
        }
        else
        {
            std::cout << m_z << " -= " <<  m_x << " + " << m_y;
            sub();
            std::cout << " = " << m_z << '\n';
        }
        m_z = m_x + m_y;
    }

    void add() { m_z += m_x + m_y; }
    void sub() { m_z -= m_x + m_y; }
};

struct square_sqrt : public ur::base_operation
{
    int m_x, m_y;
    square_sqrt(int x, bool state=true) : base_operation(state), m_x(x), m_y(x) {}

    void operator()() override
    {
        if (m_state)
        {
            square();
            std::cout << m_x << "^2 = " << m_y << '\n';
        }
        else
        {
            sqrt();
            std::cout << "sqrt(" << m_x*m_x << ") = " << m_y << '\n';
        }
    }

    void square() { m_y = m_x * m_x; }
    void sqrt() { m_y = m_x; }
};


int main()
{
    // Simple test case, create our undo redo buffer, size of 5.
    ur::undo_redo undo_redo(5);

    std::cout << "\nUndo operations ======================================\n";

    undo_redo.undo_stack(std::make_unique<add_sub>(1, 2));
    undo_redo.undo_stack(std::make_unique<add_sub>(3, 4));
    undo_redo.undo_stack(std::make_unique<square_sqrt>(5));
    undo_redo.undo_pop();
    undo_redo.undo_stack(std::make_unique<add_sub>(6, 7));
    undo_redo.undo_stack(std::make_unique<add_sub>(8, 9));
    undo_redo.undo_stack(std::make_unique<square_sqrt>(10));
    undo_redo.undo_stack(std::make_unique<square_sqrt>(11));
    undo_redo.undo_pop();
    undo_redo.undo_pop();
    undo_redo.undo_pop();
    undo_redo.undo_pop();
    undo_redo.undo_pop();

    std::cout << "Redo operations ======================================\n";

    undo_redo.redo_pop();
    undo_redo.redo_pop();
    undo_redo.redo_pop();
    undo_redo.redo_pop();
    undo_redo.redo_pop();

    std::cout << "======================================================\n\n";

    return 0;
}

