#pragma once

#include "Core/Core.h"

namespace Volt {

class FSM;

/* An individual state (must belong to a FSM). */
class FSMState {
public:
    FSMState () : m_fsm(NULL) { }
    virtual ~FSMState () { }

    virtual void Update () { }
    virtual void OnEnter () { }
    virtual void OnExit () { }

    void TransitionTo (string stateName);
    void DelayTransitionTo (string stateName);

protected:
    string m_stateName;

private:
    friend class FSM;
    FSM* m_fsm;

    DISALLOW_COPY_AND_ASSIGN(FSMState);
};

class FSM {
public:
    FSM ();
    ~FSM ();

    void Update ();

    void TransitionTo (string stateName);
    void DelayTransitionTo (string stateName);

    void AddState (FSMState* newState, string stateName);
    string stateName () const {
        return m_currentState->m_stateName;
    }
    FSMState* state () const { return m_currentState; }

private:
    FSMState* m_currentState;
    string m_delayState;
    typedef map<string, FSMState*> States;
    States m_states;
    DISALLOW_COPY_AND_ASSIGN(FSM);
};

}
