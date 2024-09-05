#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/gd_extension.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "../brakes/TrainBrake.hpp"
#include "../core/TrainController.hpp"
#include "../core/TrainPart.hpp"
#include "../core/TrainSwitch.hpp"
#include "../engines/TrainEngine.hpp"
#include "../systems/TrainSecuritySystem.hpp"

namespace godot {

    void TrainController::_bind_methods() {
        ClassDB::bind_method(D_METHOD("set_state"), &TrainController::set_state);
        ClassDB::bind_method(D_METHOD("get_state"), &TrainController::get_state);
        ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "state"), "set_state", "get_state");
        ClassDB::bind_method(D_METHOD("get_brake"), &TrainController::get_brake);
        ClassDB::bind_method(D_METHOD("get_engine"), &TrainController::get_engine);
        ClassDB::bind_method(D_METHOD("get_security_system"), &TrainController::get_security_system);

        ClassDB::bind_method(D_METHOD("get_mover_state"), &TrainController::get_mover_state);
        ClassDB::bind_method(D_METHOD("update_mover"), &TrainController::update_mover);
        ClassDB::bind_method(D_METHOD("main_controller_increase"), &TrainController::main_controller_increase);
        ClassDB::bind_method(D_METHOD("main_controller_decrease"), &TrainController::main_controller_decrease);
        ClassDB::bind_method(D_METHOD("forwarder_increase"), &TrainController::forwarder_increase);
        ClassDB::bind_method(D_METHOD("forwarder_decrease"), &TrainController::forwarder_decrease);

        ClassDB::bind_method(
                D_METHOD("_on_train_part_config_changed"), &TrainController::_on_train_part_config_changed);
        ClassDB::bind_method(D_METHOD("set_type_name"), &TrainController::set_type_name);
        ClassDB::bind_method(D_METHOD("get_type_name"), &TrainController::get_type_name);
        ClassDB::bind_method(D_METHOD("set_mass"), &TrainController::set_mass);
        ClassDB::bind_method(D_METHOD("get_mass"), &TrainController::get_mass);
        ClassDB::bind_method(D_METHOD("set_power"), &TrainController::set_power);
        ClassDB::bind_method(D_METHOD("get_power"), &TrainController::get_power);
        ClassDB::bind_method(D_METHOD("set_max_velocity"), &TrainController::set_max_velocity);
        ClassDB::bind_method(D_METHOD("get_max_velocity"), &TrainController::get_max_velocity);
        ClassDB::bind_method(D_METHOD("set_axle_arrangement"), &TrainController::set_axle_arrangement);
        ClassDB::bind_method(D_METHOD("get_axle_arrangement"), &TrainController::get_axle_arrangement);
        ClassDB::bind_method(D_METHOD("set_battery_enabled"), &TrainController::set_battery_enabled);
        ClassDB::bind_method(D_METHOD("get_battery_enabled"), &TrainController::get_battery_enabled);
        ClassDB::bind_method(D_METHOD("set_nominal_battery_voltage"), &TrainController::set_nominal_battery_voltage);
        ClassDB::bind_method(D_METHOD("get_nominal_battery_voltage"), &TrainController::get_nominal_battery_voltage);

        ClassDB::bind_method(D_METHOD("set_engine_path"), &TrainController::set_engine_path);
        ClassDB::bind_method(D_METHOD("get_engine_path"), &TrainController::get_engine_path);
        ADD_PROPERTY(
                PropertyInfo(Variant::NODE_PATH, "parts/engine", PROPERTY_HINT_NODE_PATH_VALID_TYPES, "TrainEngine"),
                "set_engine_path", "get_engine_path");

        ClassDB::bind_method(D_METHOD("set_brake_path"), &TrainController::set_brake_path);
        ClassDB::bind_method(D_METHOD("get_brake_path"), &TrainController::get_brake_path);
        ADD_PROPERTY(
                PropertyInfo(Variant::NODE_PATH, "parts/brake", PROPERTY_HINT_NODE_PATH_VALID_TYPES, "TrainBrake"),
                "set_brake_path", "get_brake_path");

        ClassDB::bind_method(D_METHOD("set_security_system_path"), &TrainController::set_security_system_path);
        ClassDB::bind_method(D_METHOD("get_security_system_path"), &TrainController::get_security_system_path);
        ADD_PROPERTY(
                PropertyInfo(
                        Variant::NODE_PATH, "parts/security_system", PROPERTY_HINT_NODE_PATH_VALID_TYPES,
                        "TrainSecuritySystem"),
                "set_security_system_path", "get_security_system_path");

        ADD_PROPERTY(PropertyInfo(Variant::STRING, "type_name"), "set_type_name", "get_type_name");
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "dimensions/mass"), "set_mass", "get_mass");
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "power"), "set_power", "get_power");
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "max_velocity"), "set_max_velocity", "get_max_velocity");
        ADD_PROPERTY(PropertyInfo(Variant::STRING, "axle_arrangement"), "set_axle_arrangement", "get_axle_arrangement");

        /* FIXME: move to TrainPower section? */
        ADD_PROPERTY(
                PropertyInfo(Variant::BOOL, "switches/battery_enabled"), "set_battery_enabled", "get_battery_enabled");
        ADD_PROPERTY(
                PropertyInfo(Variant::FLOAT, "nominal_battery_voltage", PROPERTY_HINT_RANGE, "0,500,1"),
                "set_nominal_battery_voltage", "get_nominal_battery_voltage");
    }

    TrainController::TrainController() {
        // FIXME: move to _init or _ready signal?
    }

    TMoverParameters *TrainController::get_mover() const {
        return mover;
    }

    void TrainController::set_brake_path(const NodePath &p_path) {
        if (brake_path != p_path) {
            brake_path = p_path;
            _brake = nullptr;
            _dirty = true;
        }
    }

    NodePath TrainController::get_brake_path() const {
        return brake_path;
    }

    TrainBrake *TrainController::get_brake() const {
        if (_brake) {
            return _brake;
        }

        if (!brake_path.is_empty()) {
            Node *node = get_node_or_null(brake_path);
            if (node) {
                _brake = cast_to<TrainBrake>(node);
            }
        }
        return _brake;
    }

    void TrainController::set_engine_path(const NodePath &p_path) {
        if (engine_path != p_path) {
            engine_path = p_path;
            _engine = nullptr;
            _dirty = true;
        }
    }

    NodePath TrainController::get_engine_path() const {
        return engine_path;
    }

    TrainEngine *TrainController::get_engine() const {
        if (_engine) {
            return _engine;
        }

        if (!engine_path.is_empty()) {
            Node *node = get_node_or_null(engine_path);
            if (node) {
                _engine = cast_to<TrainEngine>(node);
            }
        }
        return _engine;
    }

    void TrainController::set_security_system_path(const NodePath &p_path) {
        if (security_system_path != p_path) {
            security_system_path = p_path;
            _security_system = nullptr;
            _dirty = true;
        }
    }

    NodePath TrainController::get_security_system_path() const {
        return security_system_path;
    }

    TrainSecuritySystem *TrainController::get_security_system() const {
        if (_security_system) {
            return _security_system;
        }

        if (!security_system_path.is_empty()) {
            Node *node = get_node_or_null(security_system_path);
            if (node) {
                _security_system = cast_to<TrainSecuritySystem>(node);
            }
        }
        return _security_system;
    }

    void TrainController::initialize_mover() {
        mover = new TMoverParameters(
                this->initial_velocity, std::string(this->type_name.utf8().ptr()),
                std::string(this->get_name().left(this->get_name().length()).utf8().ptr()), this->cabin_number);

        _dirty = true;
        _dirty_prop = true;
        _update_mover_config_if_dirty();

        /* FIXME: CheckLocomotiveParameters should be called after (re)initialization */
        mover->CheckLocomotiveParameters(true, 0); // FIXME: brakujace parametery

        /* FIXME: remove test data */
        mover->CabActive = 1;
        mover->CabMaster = true;
        mover->CabOccupied = 1;
        mover->AutomaticCabActivation = true;
        mover->CabActivisationAuto();
        mover->CabActivisation();

        /* switch_physics() raczej trzeba zostawic */
        mover->switch_physics(true);

        UtilityFunctions::print("[MaSzyna::TMoverParameters] Mover initialized successfully");
    }

    void TrainController::set_type_name(const String &p_type_name) {
        type_name = p_type_name;
    }

    String TrainController::get_type_name() const {
        return type_name;
    }

    void TrainController::_connect_signals_to_train_part(TrainPart *part) {
        if (part != nullptr) {
            const Callable _c = Callable(this, "_on_train_part_config_changed").bind(part);
            part->connect("config_changed", _c); // Clang-Tidy: The value returned by this function should not be
                                                 // disregarded; neglecting it may lead to errors
        }
    }
    void TrainController::_ready() {
        /* nie daj borze w edytorze */
        if (Engine::get_singleton()->is_editor_hint()) {
            return;
        }

        _connect_signals_to_train_part(get_brake());
        _connect_signals_to_train_part(get_engine());
        _connect_signals_to_train_part(get_security_system());

        /* eksperymentalna obsluga switchy umieszczanych jako dzieci TrainControllera
         * automatycznie podpina sygnaly bez koniecznosci podpinania switchy przez NodePath/Assign */
        Vector<TrainSwitch *> switches = get_train_switches();
        for (const auto switche: switches) {
            TrainSwitch *_s = cast_to<TrainSwitch>(switche);
            if (_s) {
                _connect_signals_to_train_part(_s);
            }
        }

        initialize_mover();

        UtilityFunctions::print("TrainController::_ready() signals connected to train parts");
    }

    void TrainController::_update_mover_config_if_dirty() {
        if (_dirty) {
            /* zmienia sie istotny property glownego kontrolera
             * trzeba wszystkie czesci odswiezyc
             */
            _on_train_part_config_changed(get_brake());
            _on_train_part_config_changed(get_engine());
            _on_train_part_config_changed(get_security_system());

            /* eksperymentalna obsluga switchy umieszczanych jako dzieci TrainControllera */
            const Vector<TrainSwitch *> switches = get_train_switches();
            for (auto switche: switches) {
                _on_train_part_config_changed(switche);
            }

            // mover->CheckLocomotiveParameters(true, 0);

            _dirty = false;
            _dirty_prop = true; // sforsowanie odswiezenia stanu lokalnych propsow
        }

        if (_dirty_prop) {
            update_mover();
            _dirty_prop = false;
        }
    }

    void TrainController::_process_mover(const double delta) {
        TrainPart *brake;
        TrainPart *security;
        TrainPart *engine;

        brake = (TrainPart *)get_brake();
        if (brake != nullptr) {
            brake->_process_mover(this, delta);
        }

        security = (TrainPart *)get_security_system();
        if (security != nullptr) {
            security->_process_mover(this, delta);
        }

        engine = (TrainPart *)get_engine();
        if (engine != nullptr) {
            engine->_process_mover(this, delta);
        }

        mover->ComputeTotalForce(delta);
        mover->compute_movement_(delta);

        state.merge(internal_state, true);

        if (brake != nullptr) {
            state.merge(brake->get_mover_state(this), true);
        }
        if (engine != nullptr) {
            state.merge(engine->get_mover_state(this), true);
        }
        if (security != nullptr) {
            state.merge(security->get_mover_state(this), true);
        }
    }

    void TrainController::_process(const double delta) {
        /* nie daj borze w edytorze */
        if (Engine::get_singleton()->is_editor_hint()) {
            return;
        }

        _update_mover_config_if_dirty();
        _process_mover(delta);
    }

    void TrainController::_do_update_internal_mover(TMoverParameters *mover) const {
        mover->Mass = mass;
        mover->Power = power;
        mover->Vmax = max_velocity;

        mover->ComputeMass();
        mover->NPoweredAxles = Maszyna::s2NPW(axle_arrangement.ascii().get_data());
        mover->NAxles = mover->NPoweredAxles + Maszyna::s2NNW(axle_arrangement.ascii().get_data());

        // FIXME: move to TrainPower
        mover->Battery = sw_battery_enabled;
        mover->NominalBatteryVoltage = nominal_battery_voltage;
        mover->BatteryVoltage = nominal_battery_voltage;
    }

    void TrainController::_on_train_part_config_changed(TrainPart *part) const {
        if (part == nullptr) {
            return;
        }
        part->update_mover(this);
    }

    double TrainController::get_nominal_battery_voltage() const {
        return nominal_battery_voltage;
    }

    void TrainController::set_nominal_battery_voltage(const double p_nominal_battery_voltage) {
        nominal_battery_voltage = p_nominal_battery_voltage;
        _dirty_prop = true;
    }

    void TrainController::set_battery_enabled(const bool p_state) {
        sw_battery_enabled = p_state;
        _dirty_prop = true;
    }

    bool TrainController::get_battery_enabled() const {
        return sw_battery_enabled;
    }

    void TrainController::set_mass(const double p_mass) {
        mass = p_mass;
        _dirty_prop = true;
    }

    double TrainController::get_mass() const {
        return mass;
    }

    void TrainController::set_power(const double p_power) {
        power = p_power;
        _dirty_prop = true;
    }

    double TrainController::get_power() const {
        return power;
    }

    void TrainController::set_max_velocity(const double p_value) {
        max_velocity = p_value;
        _dirty_prop = true;
    }

    double TrainController::get_max_velocity() const {
        return max_velocity;
    }

    void TrainController::set_axle_arrangement(String p_value) {
        axle_arrangement = p_value;
    }

    String TrainController::get_axle_arrangement() const {
        return axle_arrangement;
    }

    Vector<TrainSwitch *> TrainController::get_train_switches() {
        Vector<TrainSwitch *> train_switches;
        _collect_train_switches(this, train_switches);
        return train_switches;
    }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    void TrainController::_collect_train_switches(
            const Node *node, Vector<TrainSwitch *> &train_switches) { // NOLINT(*-no-recursion)
        if (node == nullptr) {
            return;
        }
        for (int i = 0; i < node->get_child_count(); ++i) {
            Node *child = node->get_child(i);

            TrainSwitch *train_switch = dynamic_cast<TrainSwitch *>(child);
            if (train_switch != nullptr) {
                train_switches.push_back(train_switch);
            }
            _collect_train_switches(child, train_switches);
        }
    }

    void TrainController::main_controller_increase() {
        // FIXME: mover can be null; change to enqueue command
        if (mover != nullptr) {
            mover->IncMainCtrl(1);
        }
    }

    void TrainController::main_controller_decrease() {
        // FIXME: mover can be null; change to enqueue command
        if (mover != nullptr) {
            mover->DecMainCtrl(1);
        }
    }

    void TrainController::forwarder_decrease() {
        // FIXME: mover can be null; change to enqueue command
        if (mover != nullptr) {
            mover->DirectionBackward();
        }
    }

    void TrainController::forwarder_increase() {
        // FIXME: mover can be null; change to enqueue command
        if (mover != nullptr) {
            mover->DirectionForward();
        }
    }

    void TrainController::update_mover() const {
        TMoverParameters *mover = get_mover();
        if (mover != nullptr) {
            _do_update_internal_mover(mover);
        } else {
            UtilityFunctions::push_warning("TrainController::update_mover() failed: internal mover not initialized");
        }
    }

    Dictionary TrainController::get_mover_state() {
        TMoverParameters *mover = get_mover();
        if (mover != nullptr) {
            _do_fetch_state_from_mover(mover, state);
        } else {
            UtilityFunctions::push_warning("TrainController::get_mover_state() failed: internal mover not initialized");
        }
        return internal_state;
    }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    void TrainController::_do_fetch_state_from_mover(
            TMoverParameters *mover, Dictionary &state) { // NOLINT(*-convert-member-functions-to-static)
        internal_state["mass_total"] = mover->TotalMass;
        internal_state["velocity"] = mover->V;
        internal_state["speed"] = mover->Vel;
        internal_state["total_distance"] = mover->DistCounter;
        internal_state["direction"] = mover->DirActive;
        internal_state["cabin"] = mover->CabActive;
        internal_state["cabin_controleable"] = mover->IsCabMaster();
        internal_state["cabin_occupied"] = mover->CabOccupied;

        /* FIXME: should be just a config property IMO; better to fix AxleArangement */
        internal_state["axles_powered_count"] = mover->NPoweredAxles;
        internal_state["axles_count"] = mover->NAxles;


        /* FIXME: move to TrainPower section? */
        internal_state["battery_voltage"] = mover->BatteryVoltage;

        /* FIXME: move to TrainPower section */
        internal_state["power24_voltage"] = mover->Power24vVoltage;
        internal_state["power24_available"] = mover->Power24vIsAvailable;
        internal_state["power110_available"] = mover->Power110vIsAvailable;
        internal_state["current0"] = mover->ShowCurrent(0);
        internal_state["current1"] = mover->ShowCurrent(1);
        internal_state["current2"] = mover->ShowCurrent(2);
        internal_state["relay_novolt"] = mover->NoVoltRelay;
        internal_state["relay_overvoltage"] = mover->OvervoltageRelay;
        internal_state["relay_ground"] = mover->GroundRelay;
        internal_state["train_damage"] = mover->DamageFlag;
        internal_state["controller_second_position"] = mover->ScndCtrlPos;
        internal_state["controller_main_position"] = mover->MainCtrlPos;
    }

    void TrainController::set_state(const Dictionary p_state) {
        state = p_state;
    }

    Dictionary TrainController::get_state() {
        return state;
    }

} // namespace godot
