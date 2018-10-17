#pragma once

namespace jk::utl {
	template<class StateType>
	class continuum_state {
		StateType states_[2];
		

	public:
		continuum_state() = default;
		continuum_state(const StateType & initial);
		
		bool is_continuum(StateType && state) const;
		bool is_continuum(const StateType & state) const;

		bool is_not_continuum(StateType && state) const;
		bool is_not_continuum(const StateType & state) const;

		StateType get_last() const;

		StateType get_prev() const;

		StateType operator=(const StateType & state);
		StateType & operator=(StateType && state);

		operator StateType() const;
	};


	template<class StateType>
	inline continuum_state<StateType>::continuum_state(const StateType & initial) {
		states_[0] = states_[1] = initial;
	}

	template<class StateType>
	inline bool continuum_state<StateType>::is_continuum(StateType && state) const {
		return is_continuum(state);
	}

	template<class StateType>
	inline bool continuum_state<StateType>::is_continuum(const StateType & state) const {
		return states_[0] == states_[1];
	}

	template<class StateType>
	inline bool continuum_state<StateType>::is_not_continuum(StateType && state) const {
		return is_not_continuum(state);
	}

	template<class StateType>
	inline bool continuum_state<StateType>::is_not_continuum(const StateType & state) const {
		return states_[1] == state && states_[0] != states_[1];
	}

	template<class StateType>
	inline StateType continuum_state<StateType>::get_last() const {
		return states_[1];
	}

	template<class StateType>
	inline StateType continuum_state<StateType>::get_prev() const {
		return states_[0];
	}

	template<class StateType>
	inline StateType continuum_state<StateType>::operator=(const StateType & state) {
		states_[0] = states_[1];
		states_[1] = state;
		return state;
	}

	template<class StateType>
	inline StateType & continuum_state<StateType>::operator=(StateType && state) {
		return *this = state;
	}

	template<class StateType>
	inline continuum_state<StateType>::operator StateType() const {
		return get_last();
	}

}
