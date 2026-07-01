#ifndef EMITTER_HPP
#define EMITTER_HPP

class Emitter {
	protected:
		unsigned char maximum_dimming_level;
		unsigned char minimum_dimming_level;
		unsigned int pulse_time;
		unsigned int turn_off_time;
	protected:
		Emitter(unsigned char maximum_dimming_level, unsigned char minimum_dimming_level, unsigned int pulse_time, unsigned int turn_off_time);
	protected:
		virtual void setAsOutput() const = 0;
		virtual void setHigh() const = 0;
		virtual void setLow() const = 0;
		virtual void setStoredDimmingLevel(unsigned char _dimming_level) = 0;
		virtual void incrementStoredDimmingLevel(unsigned char _increment) = 0;
		unsigned char calcNewDimmingLevel(unsigned char _dimming_level, unsigned char _increment);
		unsigned char calcIncrementRequired(unsigned char _dimming_level, unsigned char _desired_dimming_level);
	public:
		virtual void setDimmingLevel(unsigned char _dimming_level) = 0;
		unsigned char getMaximumDimmingLevel() const;
		unsigned char getMinimumDimmingLevel() const;
		unsigned char getNumDimmingLevels() const;
		unsigned int getTurnOffTime() const;
		unsigned int getPulseTime() const;
		void init();
		void turnOff();
		void turnOn();
		void incrementDimmingLevel(unsigned char _increment = 1);
};


class Unbounded_Single_Emitter { 
	friend class Dual_Emitters;
	template <unsigned char num_emitters> friend class Multiple_Emitters;
	template <typename Derived> friend class Single_Emitter_CRTP;
	template <typename Derived> friend class Dual_Emitter_CRTP;
	protected:
		unsigned char pin;
		unsigned char dimming_level;
	public:
		Unbounded_Single_Emitter();
		Unbounded_Single_Emitter(unsigned char emitter_pin, unsigned char dimming_level);
	protected:
		void setAsOutput() const;
		void setHigh() const;
		void setLow() const;
	public:
		unsigned char getDimmingLevel() const;
};


template <typename Derived>
class Single_Emitter_CRTP : public Emitter {
	using Emitter::Emitter;
	private:
		virtual void setAsOutput() const override;
		virtual void setHigh() const override;
		virtual void setLow() const override;
		virtual void setStoredDimmingLevel(unsigned char _dimming_level) override;
		virtual void incrementStoredDimmingLevel(unsigned char _increment) override;
	public:
		virtual void setDimmingLevel(unsigned char _dimming_level) override;
		unsigned char getDimmingLevel() const;
};


class Single_Emitter : public Single_Emitter_CRTP<Single_Emitter> {
	friend class Single_Emitter_CRTP<Single_Emitter>;
	private:
		Unbounded_Single_Emitter emitter;
	public:
		Single_Emitter(unsigned char maximum_dimming_level, unsigned char minimum_dimming_level, unsigned int pulse_time, unsigned int turn_off_time, unsigned char emitter_pin);
	private:
		Unbounded_Single_Emitter& getUnboundedSingleEmitter();
		const Unbounded_Single_Emitter& getUnboundedSingleEmitter() const;
};


class Single_Emitter_Reference : public Single_Emitter_CRTP<Single_Emitter_Reference> {
	friend class Single_Emitter_CRTP<Single_Emitter_Reference>;
	private:
		Unbounded_Single_Emitter* emitter;
	public:
		Single_Emitter_Reference(unsigned char maximum_dimming_level, unsigned char minimum_dimming_level, unsigned int pulse_time, unsigned int turn_off_time, Unbounded_Single_Emitter* emitter);
	private:
		Unbounded_Single_Emitter& getUnboundedSingleEmitter() const;
		
};	


template <typename Derived>
class Dual_Emitter_CRTP : public Emitter {
	using Emitter::Emitter;
	private:
		virtual void setAsOutput() const override;
		virtual void setHigh() const override;
		virtual void setLow() const override;
		virtual void setStoredDimmingLevel(unsigned char _dimming_level) override;
		virtual void incrementStoredDimmingLevel(unsigned char _increment) override;
	public:
		virtual void setDimmingLevel(unsigned char _dimming_level) override;
		unsigned char getOddEmitterDimmingLevel() const;
		unsigned char getEvenEmitterDimmingLevel() const;
		Single_Emitter_Reference getOddEmitter();
		Single_Emitter_Reference getEvenEmitter();
};


class Dual_Emitter : public Dual_Emitter_CRTP<Dual_Emitter> {
	friend class Dual_Emitter_CRTP<Dual_Emitter>;
	private:
		Unbounded_Single_Emitter odd_emitter;
		Unbounded_Single_Emitter even_emitter;
	public:
		Dual_Emitter(unsigned char maximum_dimming_level, unsigned char minimum_dimming_level, unsigned int pulse_time, unsigned int turn_off_time, unsigned char odd_pin, unsigned char even_pin);
	private:
		Unbounded_Single_Emitter& getOddUnboundedSingleEmitter();
		const Unbounded_Single_Emitter& getOddUnboundedSingleEmitter() const;
		Unbounded_Single_Emitter& getEvenUnboundedSingleEmitter();
		const Unbounded_Single_Emitter& getEvenUnboundedSingleEmitter() const;
};


class Dual_Emitter_Reference : public Dual_Emitter_CRTP<Dual_Emitter_Reference> {
	friend class Dual_Emitter_CRTP<Dual_Emitter_Reference>;
	private:
		Unbounded_Single_Emitter* odd_emitter;
		Unbounded_Single_Emitter* even_emitter;
	public:
		Dual_Emitter_Reference(unsigned char maximum_dimming_level, unsigned char minimum_dimming_level, unsigned int pulse_time, unsigned int turn_off_time, Unbounded_Single_Emitter* odd_emitter, Unbounded_Single_Emitter* even_emitter);
	private:
		Unbounded_Single_Emitter& getOddUnboundedSingleEmitter() const;
		Unbounded_Single_Emitter& getEvenUnboundedSingleEmitter() const;
};


template <unsigned char num_emitters>
class Multiple_Emitters : public Emitter {
	private:
		Unbounded_Single_Emitter emitters[num_emitters];
	public:
		template <typename... Integral_Types> Multiple_Emitters(unsigned char maximum_dimming_level, unsigned char minimum_dimming_level, unsigned int pulse_time, unsigned int turn_off_time, Integral_Types... emitter_pins);
	public:
		static constexpr unsigned char getNumEmitters();
	private:
		virtual void setAsOutput() const override;
		virtual void setHigh() const override;
		virtual void setLow() const override;
		virtual void setStoredDimmingLevel(unsigned char _dimming_level) override;
		virtual void incrementStoredDimmingLevel(unsigned char _increment) override;
		template <typename Integral_Type, typename... Integral_Types> void setEmitterPins(unsigned char index, Integral_Type emitter_pin, Integral_Types... emitter_pins);
		template <typename Integral_Type> void setEmitterPins(unsigned char index, Integral_Type emitter_pin);
	public:
		virtual void setDimmingLevel(unsigned char _dimming_level) override;
		unsigned char getDimmingLevel(unsigned char emitter_index) const;
		Single_Emitter_Reference getEmitter(unsigned char emitter_index);
		Dual_Emitter_Reference getDualEmitter(unsigned char odd_emitter_index, unsigned char even_emitter_index);
};


#include "Emitter.tpp"



#endif

