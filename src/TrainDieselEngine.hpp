#pragma once
#include <godot_cpp/classes/node.hpp>
#include "TrainEngine.hpp"
#include "maszyna/McZapkie/MOVER.h"


namespace godot {
    class TrainController;

    class TrainDieselEngine : public TrainEngine {
            GDCLASS(TrainDieselEngine, TrainEngine)

        private:
            static void _bind_methods();
            double oil_min_pressure = 0.0;   // OilMinPressure -> OilPump.pressure_minimum
            double oil_max_pressure = 0.65;  // OilMaxPressure -> OilPump.pressure_maximum
            double traction_force_max = 0.0; // Ftmax -> Ftmax

        protected:
            virtual TEngineType get_engine_type() override;
            void _do_update_internal_mover(TMoverParameters *mover) override;
            void _do_fetch_state_from_mover(TMoverParameters *mover, Dictionary &state) override;

        public:
            double get_oil_min_pressure() const;
            void set_oil_min_pressure(double value);

            double get_oil_max_pressure() const;
            void set_oil_max_pressure(double value);

            double get_traction_force_max() const;
            void set_traction_force_max(double value);

            TrainDieselEngine();
            ~TrainDieselEngine() override = default;
    };
} // namespace godot