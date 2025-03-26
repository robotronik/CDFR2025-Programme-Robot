#pragma once

// Consider using enum class for better type safety
typedef enum
{
    FSM_RETURN_WORKING =0x0,
    FSM_RETURN_DONE    =0x1,
    FSM_RETURN_ERROR   =0x2
} ReturnFSM_t;

class ActionFSM{
public:
    ActionFSM();
    ~ActionFSM();
    void Reset();
    bool RunFSM();

private:
    ReturnFSM_t GatherStock();
    ReturnFSM_t ConstructAllTribunesFSM(int zone); // Num of the zone

    typedef enum
    {
        FSM_ACTION_GATHER,
        FSM_ACTION_BUILD,
        FSM_ACTION_DEPLOY_BANNER,
        FSM_ACTION_NAV_HOME
    } StateRun_t;

    StateRun_t runState = FSM_ACTION_GATHER;

    typedef enum
    {
        FSM_GATHER_NAV,
        FSM_GATHER_MOVE,
        FSM_GATHER_COLLECT
    } StateGatherStock_t;

    StateGatherStock_t gatherStockState = FSM_GATHER_NAV;

    typedef enum
    {
        FSM_CONSTRUCT_NAV,
        FSM_CONSTRUCT_MOVE,
        FSM_CONSTRUCT_PREPREVOLVER,
        FSM_CONSTRUCT_BUILD,
        FSM_CONSTRUCT_EXIT
    } StateConstructTribunes_t;

    StateConstructTribunes_t constructAllTribunesState = FSM_CONSTRUCT_NAV;
};