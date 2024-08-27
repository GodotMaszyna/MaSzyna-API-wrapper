#include <godot_cpp/classes/gd_extension.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "../maszyna/McZapkie/MOVER.h"
#include "TrainMainSwitch.hpp"

namespace godot {
    TrainMainSwitch::TrainMainSwitch() {};

    void TrainMainSwitch::_bind_methods() {};

    void TrainMainSwitch::_do_update_internal_mover(TMoverParameters *mover) {
        bool pushed = get_pushed();

        /* for testing purposes */
        mover->GroundRelay = true;
        mover->NoVoltRelay = true;
        mover->OvervoltageRelay = true;
        mover->DamageFlag = false;
        mover->EngDmgFlag = false;
        mover->ConvOvldFlag = false;
        mover->EnginePowerSource.SourceType = TPowerSource::Accumulator;

        mover->MainSwitch(pushed);
    };
    void TrainMainSwitch::_do_fetch_state_from_mover(TMoverParameters *mover, Dictionary &state) {
        state["enabled"] = mover->Mains;
        state["time_to_initialize"] = mover->MainsInitTimeCountdown;
    }

} // namespace godot