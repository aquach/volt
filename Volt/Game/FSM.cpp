#include "Volt/Game/FSM.h"

namespace Volt {

void FSMState::TransitionTo (string stateName) {
    m_fsm->TransitionTo(stateName);
}

void FSMState::DelayTransitionTo (string stateName) {
    m_fsm->DelayTransitionTo(stateName);
}


FSM::FSM ()
    : m_currentState(NULL) { }

FSM::~FSM () {
    FOR_ (States::iterator, i, m_states)
        delete i->second;
    m_states.clear();
}

void FSM::Update () {
    if (m_currentState == NULL) {
        LOG(WARNING) << "No current state for FSM.";
        return;
    }

    if (m_delayState != "") {
        TransitionTo(m_delayState);
        m_delayState = "";
    }

    m_currentState->Update();
}

void FSM::TransitionTo (string stateName) {
    States::iterator i = m_states.find(stateName);
    CHECK(i != m_states.end()) << "Missing state " << stateName;

    FSMState* nextState = i->second;

    if (m_currentState != NULL)
        m_currentState->OnExit();
    nextState->OnEnter();
    m_currentState = nextState;
}

void FSM::DelayTransitionTo (string stateName) {
    m_delayState = stateName;
}

void FSM::AddState (FSMState* newState, string stateName) {
    CHECK(newState->m_fsm == NULL);
    newState->m_fsm = this;
    newState->m_stateName = stateName;
    pair<States::iterator, bool> result = m_states.insert(
        make_pair(stateName, newState));

    CHECK(result.second) << stateName << " already in the FSM.";
}

}
