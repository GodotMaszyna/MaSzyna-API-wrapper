#pragma once
#include <godot_cpp/classes/node.hpp>
#include "TrainPart.hpp"
#include "maszyna/McZapkie/MOVER.h"


namespace godot {
    class TrainSecuritySystem : public TrainPart {
            GDCLASS(TrainSecuritySystem, TrainPart)


        private:
            static void _bind_methods();
            friend class Maszyna::TSecuritySystem;

        protected:
            virtual void _do_update_internal_mover(TMoverParameters *mover) override;
            virtual void _do_fetch_state_from_mover(TMoverParameters *mover, Dictionary &state) override;

        public:
            enum EmergencyBrakeWarningSignal { SIREN_LOW_TONE, SIREN_HIGH_TONE, WHISTLE };

            TrainSecuritySystem();
            ~TrainSecuritySystem() override = default;

        private:
            bool enabled = true;

            bool aware_system_active = false;
            bool aware_system_cabsignal = false;
            bool aware_system_separate_acknowledge = false;
            bool aware_system_sifa = false;

            double aware_delay = 0.0;           // AwareDelay -> SecuritySystem->AwareDelay
            double emergency_brake_delay = 0.0; // EmergencyBrakeDelay -> SecuritySystem->EmergencyBrakeDelay
            EmergencyBrakeWarningSignal emergency_brake_warning_signal =
                    EmergencyBrakeWarningSignal::SIREN_HIGH_TONE; // EmergencyBrakeWarningSignal ->
                                                                  // EmergencyBrakeWarningSignal
            bool radio_stop = true;                               // RadioStop -> SecuritySystem->radiostop_enabled
            double sound_signal_delay = 0.0;  // SoundSignalDelay -> SecuritySystem->SoundSignalDelay
            double shp_magnet_distance = 0.0; // MagnetLocation -> SecuritySystem->MagnetLocation
            double ca_max_hold_time = 0.0;    // MaxHoldTime -> SecuritySystem->MaxHoldTime
            bool reset_pushed = false;

        public:
            // Getters
            bool get_reset_pushed() const;
            bool get_aware_system_active() const;
            bool get_aware_system_cabsignal() const;
            bool get_aware_system_separate_acknowledge() const;
            bool get_aware_system_sifa() const;
            double get_aware_delay() const;
            double get_emergency_brake_delay() const;
            EmergencyBrakeWarningSignal get_emergency_brake_warning_signal() const;
            bool get_radio_stop() const;
            double get_sound_signal_delay() const;
            double get_shp_magnet_distance() const;
            double get_ca_max_hold_time() const;
            bool get_enabled() const;

            // Setters
            void set_reset_pushed(bool p_state);
            void set_aware_system_active(bool p_state);
            void set_aware_system_cabsignal(bool p_state);
            void set_aware_system_separate_acknowledge(bool p_state);
            void set_aware_system_sifa(bool p_state);
            void set_aware_delay(double value);
            void set_emergency_brake_delay(double value);
            void set_emergency_brake_warning_signal(EmergencyBrakeWarningSignal value);
            void set_radio_stop(bool value);
            void set_sound_signal_delay(double value);
            void set_shp_magnet_distance(double value);
            void set_ca_max_hold_time(double value);
            void set_enabled(bool value);
    };
} // namespace godot
VARIANT_ENUM_CAST(TrainSecuritySystem::EmergencyBrakeWarningSignal)
