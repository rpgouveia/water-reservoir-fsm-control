/**
 * @file sm.h
 * @author Prof. Afonso Miguel / PUCPR
 * @date February 27, 2013
 * @brief A minimalist, header-only micro framework for state machines in C.
 *
 * @details This framework uses function pointers and macros to provide a simple
 * and readable API for creating Finite State Machines (FSM).
 * It is ideal for embedded systems and projects that require
 * state control with low overhead.
 *
 * @version 1.1
 *
 * @copyright MIT License
 */

#ifndef SM_H
#define SM_H

// --- Forward Declarations ---
// Allows pStateFunc to use StateMachine before the struct's full definition.
struct StateMachine;

/**
 * @brief Defines the function pointer type for all state functions.
 *
 * @details Each state in the machine is a function that matches this signature.
 * It returns no value and receives a pointer to the state machine
 * instance, allowing the state to modify the machine (e.g., change
 * to the next state).
 *
 * @param struct StateMachine* A pointer to the instance of the state machine
 * being executed.
 */
typedef void (*pStateFunc)(struct StateMachine *);

/**
 * @brief Structure that represents the state machine instance.
 *
 * @details This structure holds all the necessary context for the FSM to operate,
 * including the current state and a flag to detect entry into a
 * new state.
 */
typedef struct StateMachine{
    pStateFunc ptr;     ///< Pointer to the current state's function.
    unsigned char entry;///< Flag indicating entry into a state (1 = just entered, 0 = already in the state).
} StateMachine;

/**
 * @brief Defines the standard signature for a state function.
 * @param name The name of the function that will represent the state.
 * @note The `_sm_` argument is automatically available inside the function
 * defined with this macro.
 */
#define STATE(name)       void name(StateMachine *_sm_)

/**
 * @brief Triggers a transition to a new state.
 * @param name The name of the destination state (function).
 * @note This macro must be called from within a state function.
 */
#define NEXT_STATE(name)  _sm_->ptr = (pStateFunc)name

/**
 * @brief Initializes a state machine instance.
 * @param sm The StateMachine variable to be initialized.
 * @param name The initial state of the machine.
 */
#define INIT(sm,name)     {sm.ptr = (pStateFunc)name; sm.entry = 1;}

/**
 * @brief Executes the current cycle of the state machine.
 *
 * @details This macro calls the current state function and then updates
 * the 'entry' flag for the next cycle. If the state was changed
 * (via NEXT_STATE), the 'entry' flag will be 1 for the next
 * cycle; otherwise, it will be 0.
 *
 * @param sm The StateMachine instance to be executed.
 */
#define EXEC(sm)          {pStateFunc temp = sm.ptr; sm.ptr(&sm); sm.entry = (temp != sm.ptr);}

/**
 * @brief Convenience macro to check the entry flag.
 *
 * @details Returns true (non-zero) if it is the first time the current state
 * is being executed since the last transition. Useful for setup
 * code that should only run once per state entry.
 *
 * @return The value of the `entry` flag (1 or 0).
 * @note This macro must be used from within a state function.
 */
#define ENTRY             (_sm_->entry)


/**
 * @example Complete Usage Example
 * @code
 * #include "sm.h"
 * #include <stdio.h>
 *
 * StateMachine my_fsm; // Declare the state machine instance
 *
 * // State function declarations (or prototypes)
 * STATE(StateA);
 * STATE(StateB);
 *
 * // --- State Definitions ---
 *
 * STATE(StateA) {
 * if (ENTRY) {
 * printf("Entering StateA\n");
 * }
 * printf("Executing StateA...\n");
 * NEXT_STATE(StateB); // Transition to StateB
 * }
 *
 * STATE(StateB) {
 * if (ENTRY) {
 * printf("Entering StateB\n");
 * }
 * printf("Executing StateB...\n");
 * NEXT_STATE(StateA); // Transition back to StateA
 * }
 *
 * int main() {
 * INIT(my_fsm, StateA); // Set StateA as the initial state
 *
 * for(int i = 0; i < 4; i++) {
 * EXEC(my_fsm); // Execute one FSM cycle
 * }
 *
 * return 0;
 * }
 * @endcode
 */

#endif // SM_H