extends GenericTrainPart
class_name PoweredTrainPart

## Example of a customization based on a GenericTrainPart interface.
## PoweredTrainPart will call [method _process_powered] when low power is
## available in a train (24V or 110V), [method _process_unpowered] otherwise.


func _process_train_part(delta):
    var state = get_train_state()
    var power_avail = state.get("power24_available") or state.get("power110_available")
    if power_avail and has_method("_process_powered"):
        call("_process_powered", delta)
    elif not power_avail and has_method("_process_unpowered"):
        call("_process_unpowered", delta)
    call("_process_regardless_of_power", delta)


## Will be called when low power is available (24V or 110V)
func _process_powered(delta):
    pass


## Will be called when low power is not available (no 24V nor 110V)
func _process_unpowered(delta):
    pass


## Will be called regardless of low power supply
func _process_regardless_of_power(delta):
    pass