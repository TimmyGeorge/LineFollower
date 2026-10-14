extern "C" void delayMicroseconds(unsigned int duration); 
//dont want to include the entire arduino.h for a single function.


template <typename Derived>
void Single_Emitter_CRTP<Derived>::setAsOutput() const {
	Unbounded_Single_Emitter& emitter = ((Derived*)this)->getUnboundedSingleEmitter();
	emitter.setAsOutput();
}


template <typename Derived>
void Single_Emitter_CRTP<Derived>::setHigh() const {
	Unbounded_Single_Emitter& emitter = ((Derived*)this)->getUnboundedSingleEmitter();
	emitter.setHigh();
}


template <typename Derived>
void Single_Emitter_CRTP<Derived>::setLow() const {
	Unbounded_Single_Emitter& emitter = ((Derived*)this)->getUnboundedSingleEmitter();
	emitter.setLow();
}


template <typename Derived>
void Single_Emitter_CRTP<Derived>::setStoredDimmingLevel(unsigned char _dimming_level){
	Unbounded_Single_Emitter& emitter = ((Derived*)this)->getUnboundedSingleEmitter();
	emitter.dimming_level = _dimming_level;
}


template <typename Derived>
void Single_Emitter_CRTP<Derived>::incrementStoredDimmingLevel(unsigned char _increment) {
	Unbounded_Single_Emitter& emitter = ((Derived*)this)->getUnboundedSingleEmitter();
	emitter.dimming_level = calcNewDimmingLevel(emitter.dimming_level, _increment);
}


template <typename Derived>
void Single_Emitter_CRTP<Derived>::setDimmingLevel(unsigned char _dimming_level) {
	Unbounded_Single_Emitter& emitter = ((Derived*)this)->getUnboundedSingleEmitter();
	incrementDimmingLevel(calcIncrementRequired(emitter.dimming_level, _dimming_level));
}


template <typename Derived>
unsigned char Single_Emitter_CRTP<Derived>::getDimmingLevel() const {
	Unbounded_Single_Emitter& emitter = ((Derived*)this)->getUnboundedSingleEmitter();
	return emitter.dimming_level;
}




template <typename Derived>
void Dual_Emitter_CRTP<Derived>::setAsOutput() const {
	Unbounded_Single_Emitter& odd_emitter = ((Derived*)this)->getOddUnboundedSingleEmitter();
	Unbounded_Single_Emitter& even_emitter = ((Derived*)this)->getEvenUnboundedSingleEmitter();
	odd_emitter.setAsOutput();
	even_emitter.setAsOutput();
}


template <typename Derived>
void Dual_Emitter_CRTP<Derived>::setHigh() const {
	Unbounded_Single_Emitter& odd_emitter = ((Derived*)this)->getOddUnboundedSingleEmitter();
	Unbounded_Single_Emitter& even_emitter = ((Derived*)this)->getEvenUnboundedSingleEmitter();
	odd_emitter.setHigh();
	even_emitter.setHigh();
}


template <typename Derived>
void Dual_Emitter_CRTP<Derived>::setLow() const {
	Unbounded_Single_Emitter& odd_emitter = ((Derived*)this)->getOddUnboundedSingleEmitter();
	Unbounded_Single_Emitter& even_emitter = ((Derived*)this)->getEvenUnboundedSingleEmitter();
	odd_emitter.setLow();
	even_emitter.setLow();
}


template <typename Derived>
void Dual_Emitter_CRTP<Derived>::setStoredDimmingLevel(unsigned char _dimming_level){
	Unbounded_Single_Emitter& odd_emitter = ((Derived*)this)->getOddUnboundedSingleEmitter();
	Unbounded_Single_Emitter& even_emitter = ((Derived*)this)->getEvenUnboundedSingleEmitter();
	odd_emitter.dimming_level = _dimming_level;
	even_emitter.dimming_level = _dimming_level;
}


template <typename Derived>
void Dual_Emitter_CRTP<Derived>::incrementStoredDimmingLevel(unsigned char _increment){
	Unbounded_Single_Emitter& odd_emitter = ((Derived*)this)->getOddUnboundedSingleEmitter();
	Unbounded_Single_Emitter& even_emitter = ((Derived*)this)->getEvenUnboundedSingleEmitter();
	odd_emitter.dimming_level = calcNewDimmingLevel(odd_emitter.dimming_level, _increment);
	even_emitter.dimming_level = calcNewDimmingLevel(even_emitter.dimming_level, _increment);
}


template <typename Derived>
void Dual_Emitter_CRTP<Derived>::setDimmingLevel(unsigned char _dimming_level){
	Unbounded_Single_Emitter& odd_emitter = ((Derived*)this)->getOddUnboundedSingleEmitter();
	Unbounded_Single_Emitter& even_emitter = ((Derived*)this)->getEvenUnboundedSingleEmitter();
	unsigned char even_emitter_increment_required = calcIncrementRequired(even_emitter.dimming_level, _dimming_level);
	unsigned char odd_emitter_increment_required = calcIncrementRequired(odd_emitter.dimming_level, _dimming_level);
	if (even_emitter_increment_required > odd_emitter_increment_required){
		incrementDimmingLevel(odd_emitter_increment_required);
		getEvenEmitter().incrementDimmingLevel(even_emitter_increment_required - odd_emitter_increment_required);
	}
	else {
		incrementDimmingLevel(even_emitter_increment_required);
		getOddEmitter().incrementDimmingLevel(odd_emitter_increment_required - even_emitter_increment_required);
	}
}


template <typename Derived>
unsigned char Dual_Emitter_CRTP<Derived>::getOddEmitterDimmingLevel() const {
	Unbounded_Single_Emitter& odd_emitter = ((Derived*)this)->getOddUnboundedSingleEmitter();
	return odd_emitter.dimming_level;
}


template <typename Derived>
unsigned char Dual_Emitter_CRTP<Derived>::getEvenEmitterDimmingLevel() const {
	Unbounded_Single_Emitter& even_emitter = ((Derived*)this)->getEvenUnboundedSingleEmitter();
	return even_emitter.dimming_level;
}


template <typename Derived>
Single_Emitter_Reference Dual_Emitter_CRTP<Derived>::getOddEmitter() {
	Unbounded_Single_Emitter& odd_emitter = ((Derived*)this)->getOddUnboundedSingleEmitter();
	return Single_Emitter_Reference(maximum_dimming_level, minimum_dimming_level, pulse_time, turn_off_time, &odd_emitter);
}


template <typename Derived>
Single_Emitter_Reference Dual_Emitter_CRTP<Derived>::getEvenEmitter(){
	Unbounded_Single_Emitter& even_emitter = ((Derived*)this)->getEvenUnboundedSingleEmitter();
	return Single_Emitter_Reference(maximum_dimming_level, minimum_dimming_level, pulse_time, turn_off_time, &even_emitter);
}


#define FOR_EACH_EMITTER for (Unbounded_Single_Emitter& emitter : emitters)
#define CFOR_EACH_EMITTER for (const Unbounded_Single_Emitter& emitter : emitters)


template <unsigned char num_emitters>
template <typename... Integral_Types> 
Multiple_Emitters<num_emitters>::Multiple_Emitters(unsigned char maximum_dimming_level, unsigned char minimum_dimming_level, unsigned int pulse_time, unsigned int turn_off_time, Integral_Types... emitter_pins):
	Emitter(maximum_dimming_level, minimum_dimming_level, pulse_time, turn_off_time),
	emitters {}
{
	setEmitterPins(0, emitter_pins...);
}


template <unsigned char num_emitters>
constexpr unsigned char Multiple_Emitters<num_emitters>::getNumEmitters() {
	return num_emitters;
}


template <unsigned char num_emitters>
void Multiple_Emitters<num_emitters>::setAsOutput() const {
	CFOR_EACH_EMITTER{
		emitter.setAsOutput();
	}
}


template <unsigned char num_emitters>
void Multiple_Emitters<num_emitters>::setLow() const {
	CFOR_EACH_EMITTER{
		emitter.setLow();
	}
}


template <unsigned char num_emitters>
void Multiple_Emitters<num_emitters>::setHigh() const {
	CFOR_EACH_EMITTER{
		emitter.setHigh();
	}
}


template <unsigned char num_emitters>
void Multiple_Emitters<num_emitters>::setStoredDimmingLevel(unsigned char dimming_level){
	FOR_EACH_EMITTER{
		emitter.dimming_level = dimming_level;
	}
	
}


template <unsigned char num_emitters>
void Multiple_Emitters<num_emitters>::incrementStoredDimmingLevel(unsigned char _increment) {
	FOR_EACH_EMITTER {
		emitter.dimming_level = calcNewDimmingLevel(emitter.dimming_level,_increment);
	}
}


template <unsigned char num_emitters>
template <typename Integral_Type, typename... Integral_Types> 
void Multiple_Emitters<num_emitters>::setEmitterPins(unsigned char index, Integral_Type emitter_pin, Integral_Types... emitter_pins){
	emitters[index].pin = emitter_pin;
	setEmitterPins(index + 1, emitter_pins...);
}	


template <unsigned char num_emitters>
template <typename Integral_Type> 
void Multiple_Emitters<num_emitters>::setEmitterPins(unsigned char index, Integral_Type emitter_pin){
	emitters[index].pin = emitter_pin;
}


template <unsigned char num_emitters>
void Multiple_Emitters<num_emitters>::setDimmingLevel(unsigned char _dimming_level) {
	struct Entry {
		Unbounded_Single_Emitter* emitter;
		Entry* next_entry;
	};
	unsigned char num_increments_required[num_emitters];
	for(unsigned char i = 0; i < num_emitters; ++i)
		num_increments_required[i] = calcIncrementRequired(emitters[i].dimming_level, _dimming_level);
	Entry entries[num_emitters]; //circular singly linked list
	unsigned char num_entries = 0;
	unsigned char min_num_increments_required = getNumDimmingLevels();
	for(unsigned char i = 0; i < num_emitters; ++i){
		if (num_increments_required[i]){
			entries[num_entries].emitter = emitters + i;
			entries[num_entries].next_entry = entries + num_entries + 1;
			++num_entries;
			if (num_increments_required[i] < min_num_increments_required)
				min_num_increments_required = num_increments_required[i];
		}
	}
	entries[num_entries - 1].next_entry = entries;
	Entry* entry = entries;
	for(unsigned char removed = 0; num_entries; num_entries -= removed, removed = 0){
		for(int j = 0; j < min_num_increments_required - 1; ++j){
			for(int i = 0; i < num_entries; ++i, entry = entry->next_entry)
				entry->emitter->setLow();
			delayMicroseconds(pulse_time);
			entry = entries;
			for(int i = 0; i < num_entries; ++i, entry = entry->next_entry)
				entry->emitter->setHigh();
			delayMicroseconds(pulse_time);
			
		}
		for(int i = 0; i < num_entries; ++i, entry = entry->next_entry)
			entry->emitter->setLow();
		delayMicroseconds(pulse_time);
		unsigned char new_min_num_increments_required = getNumDimmingLevels();
		for (int i = 0; i < num_entries; ++i){
			entry->emitter->setHigh();
			unsigned char* num_increments_required_for_emitter = num_increments_required + (entry->emitter - emitters);
			*num_increments_required_for_emitter -= min_num_increments_required;
			if (*num_increments_required_for_emitter){
				entry = entry->next_entry;
				if (*num_increments_required_for_emitter < new_min_num_increments_required)
					new_min_num_increments_required = *num_increments_required_for_emitter;
			}
			else {
				//MySerial::print("Removed ", entry->emitter->pin, "");
				entry->emitter = entry->next_entry->emitter;
				entry->next_entry = entry->next_entry->next_entry;
				++removed;
				
			}	
		}
		delayMicroseconds(pulse_time);
		min_num_increments_required = new_min_num_increments_required;
	}
	setStoredDimmingLevel(_dimming_level);
}


template <unsigned char num_emitters>
unsigned char Multiple_Emitters<num_emitters>::getDimmingLevel(unsigned char emitter_index) const {
	return emitters[emitter_index].dimming_level;
}


template <unsigned char num_emitters>
Single_Emitter_Reference Multiple_Emitters<num_emitters>::getEmitter(unsigned char emitter_index) {
	return Single_Emitter_Reference(maximum_dimming_level, minimum_dimming_level, pulse_time, turn_off_time, emitters + emitter_index);
}


template <unsigned char num_emitters>
Dual_Emitter_Reference Multiple_Emitters<num_emitters>::getDualEmitter(unsigned char odd_emitter_index, unsigned char even_emitter_index){
	return Dual_Emitter_Reference(maximum_dimming_level, minimum_dimming_level, pulse_time, turn_off_time, emitters + odd_emitter_index, emitters + even_emitter_index);
}