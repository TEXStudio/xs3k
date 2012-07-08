#include	"keyboardHotkey.hpp"



namespace	X3000
{

	void	KeyboardHotkey::_register_key()
	{
		this->_key_down_event = TEXEngine::Core::KEYBOARD_KEY_CANCEL_DOWN + this->_keybind;
		this->_key_up_event = TEXEngine::Core::KEYBOARD_KEY_CANCEL_UP + this->_keybind;
		this->_modifier_down_event = TEXEngine::Core::KEYBOARD_KEY_SHIFT_DOWN + this->_modifier;
		this->_modifier_up_event = TEXEngine::Core::KEYBOARD_KEY_SHIFT_UP + this->_modifier;

		if ( this->_modifier != NONE )
		{
			this->register_event(this->_modifier_down_event);
			this->register_event(this->_modifier_up_event);
		}
		else
		{
			this->register_event(TEXEngine::Core::KEYBOARD_KEY_SHIFT_DOWN);
			this->register_event(TEXEngine::Core::KEYBOARD_KEY_CONTROL_DOWN);
			this->register_event(TEXEngine::Core::KEYBOARD_KEY_ALT_DOWN);
			this->register_event(TEXEngine::Core::KEYBOARD_KEY_SHIFT_UP);
			this->register_event(TEXEngine::Core::KEYBOARD_KEY_CONTROL_UP);
			this->register_event(TEXEngine::Core::KEYBOARD_KEY_ALT_UP);
			this->_modifier_state = true;
		}


		this->register_event(this->_update_rate_event);
		this->register_event(this->_key_down_event);
		this->register_event(this->_key_up_event);
		this->_state = false;
		this->_state_just_changed = false;
		this->_shift_state = false;
		this->_ctrl_state = false;
		this->_alt_state = false;
	};

	void	KeyboardHotkey::_register_possible_keys()
	{
		for ( std::vector<unsigned long>::const_iterator whitelist_event_iterator = this->_whitelist.begin();  whitelist_event_iterator != this->_whitelist.end();  ++whitelist_event_iterator )
		{
			if ( (*whitelist_event_iterator) >= CANCEL  &&  (*whitelist_event_iterator) < PSP_START )
				this->register_event(TEXEngine::Core::KEYBOARD_KEY_CANCEL_DOWN + (*whitelist_event_iterator));
		}

		for ( unsigned long i = 0;  i < 3;  ++i )
			this->register_event(TEXEngine::Core::KEYBOARD_KEY_SHIFT_DOWN + i );
	};

	void	KeyboardHotkey::_unregister_key()
	{
		this->unregister_event(this->_update_rate_event);
		this->unregister_event(this->_key_down_event);
		this->unregister_event(this->_key_up_event);

		if ( this->_modifier != NONE )
		{
			this->unregister_event(this->_modifier_down_event);
			this->unregister_event(this->_modifier_up_event);
			this->_modifier_state = false;
		}
		else
		{
			this->unregister_event(TEXEngine::Core::KEYBOARD_KEY_SHIFT_DOWN);
			this->unregister_event(TEXEngine::Core::KEYBOARD_KEY_CONTROL_DOWN);
			this->unregister_event(TEXEngine::Core::KEYBOARD_KEY_ALT_DOWN);
			this->unregister_event(TEXEngine::Core::KEYBOARD_KEY_SHIFT_UP);
			this->unregister_event(TEXEngine::Core::KEYBOARD_KEY_CONTROL_UP);
			this->unregister_event(TEXEngine::Core::KEYBOARD_KEY_ALT_UP);
			this->_modifier_state = true;
		}

		this->_state = false;
		this->_state_just_changed = false;
		this->_shift_state = false;
		this->_ctrl_state = false;
		this->_alt_state = false;
	};

	void	KeyboardHotkey::_unregister_possible_keys()
	{
		for ( std::vector<unsigned long>::const_iterator whitelist_event_iterator = this->_whitelist.begin();  whitelist_event_iterator != this->_whitelist.end();  ++whitelist_event_iterator )
		{
			if ( (*whitelist_event_iterator) < PSP_START )
				this->unregister_event(TEXEngine::Core::KEYBOARD_KEY_CANCEL_DOWN + (*whitelist_event_iterator));
		}

		for ( unsigned long i = 0;  i < 3;  ++i )
			this->unregister_event(TEXEngine::Core::KEYBOARD_KEY_SHIFT_DOWN + i );
	};
	
	void	KeyboardHotkey::_request_change()
	{
		this->_change_keybind = true;
		this->_previous_keybind = this->_keybind;
		this->_previous_modifier = this->_modifier;
		this->_unregister_key();
		this->_modifier = NONE;
		this->_modifier_state = false;
		this->_register_possible_keys();
	};
	
	void	KeyboardHotkey::_abort_change()
	{
		this->_change_keybind = false;
		this->_keybind = this->_previous_keybind;
		this->_modifier = this->_previous_modifier;
		this->_unregister_possible_keys();
		this->_register_key();
	};

	void	KeyboardHotkey::_finalize_change()
	{
		this->_change_keybind = false;
		this->_previous_keybind = this->_keybind;
		this->_previous_modifier = this->_modifier;
		this->_unregister_possible_keys();
		this->_register_key();
	};
	
	void	KeyboardHotkey::_change_update_rate( const unsigned long  update_rate )
	{
		if ( update_rate != this->_update_rate )
		{
			if ( update_rate >= DEFAULT  &&  update_rate <= SLOW )
			{
				this->unregister_event(this->_update_rate_event);
				this->_update_rate = update_rate;
				this->_update_rate_event = TEXEngine::Core::UPDATE + this->_update_rate;
				this->register_event(this->_update_rate_event);
			}
		}
	};

	bool	KeyboardHotkey::_code_available( const unsigned long event_code )
	{
		std::vector<unsigned long>::iterator	whitelist_event_iterator = this->_whitelist.begin();
		bool									found = false;



		while ( !found  &&  whitelist_event_iterator != this->_whitelist.end() )
		{
			if ( (TEXEngine::Core::KEYBOARD_KEY_CANCEL_DOWN + (*whitelist_event_iterator)) == event_code )
				found = true;
			else
				++whitelist_event_iterator;
		}


		return found;
	};


	KeyboardHotkey::KeyboardHotkey( const unsigned long id ,  const unsigned long keybind , const unsigned long modifier , const unsigned long update_rate , const unsigned long down_event , const std::vector<unsigned long>& whitelist )	:	
		Hotkey(id) , 
		_whitelist(whitelist) , 
		_keybind(0) , _modifier(0) ,
		_previous_keybind(_keybind) , _previous_modifier(_modifier) , 
		_update_rate(update_rate) ,
		_key_down_event(0) , _key_up_event(0) , 
		_modifier_down_event(0) , _modifier_up_event(0) ,
		_update_rate_event(TEXEngine::Core::UPDATE + update_rate) ,
		_down_event(down_event) , 
		_update_cycles(0) , 
		_shift_state(false) , _ctrl_state(false) , _alt_state(false) , 
		_state(false) ,	_modifier_state(false) , _state_just_changed(false) , _continuous_fire(false) , 
		_change_keybind(false)
	{
		this->keybind(keybind);
		this->modifier(modifier);
		this->enable();
	};

	KeyboardHotkey::~KeyboardHotkey()	{ this->disable(); };


	void	KeyboardHotkey::on_event( const unsigned long event_code , const TEXEngine::Core::Message& parameters )
	{
		if ( this->_change_keybind )
		{
			if ( event_code >= TEXEngine::Core::KEYBOARD_KEY_SHIFT_DOWN  &&  event_code <= TEXEngine::Core::KEYBOARD_KEY_ALT_DOWN )
			{
				if ( !this->_modifier_state )
				{
					this->_modifier = event_code - TEXEngine::Core::KEYBOARD_KEY_SHIFT_DOWN;
					this->_modifier_state = true;
				}
			}
			else if ( this->_code_available(event_code) )
			{
				if ( !this->_state )
				{
					this->_keybind = event_code - TEXEngine::Core::KEYBOARD_KEY_CANCEL_DOWN;
					this->_state = true;
				}
			}


			if ( this->_state )
				this->_finalize_change();
		}
		else
		{
			if ( event_code == TEXEngine::Core::KEYBOARD_KEY_SHIFT_DOWN )
				this->_shift_state = true;
			else if ( event_code == TEXEngine::Core::KEYBOARD_KEY_CONTROL_DOWN )
				this->_ctrl_state = true;
			else if ( event_code == TEXEngine::Core::KEYBOARD_KEY_ALT_DOWN )
				this->_alt_state = true;
			else if ( event_code == TEXEngine::Core::KEYBOARD_KEY_SHIFT_UP )
				this->_shift_state = false;
			else if ( event_code == TEXEngine::Core::KEYBOARD_KEY_CONTROL_UP )
				this->_ctrl_state = false;
			else if ( event_code == TEXEngine::Core::KEYBOARD_KEY_ALT_UP )
				this->_alt_state = false;

			if ( event_code == this->_key_down_event )
			{
				if ( this->_modifier == NONE )
				{
					if ( !this->_shift_state   &&  !this->_ctrl_state  &&  !this->_alt_state )
					{
						if ( !this->_state )
						{
							this->_update_cycles = 0;
							this->_state_just_changed = true;
						}
					}
				}
				else if ( !this->_state  &&  this->_modifier_state )
				{
					this->_update_cycles = 0;
					this->_state_just_changed = true;
				}

				this->_state = true;
			}
			else if ( event_code == this->_key_up_event )
			{
				this->_state = false;
				this->_state_just_changed = false;
			}
			else if ( event_code == this->_modifier_down_event )
			{
				this->_modifier_state = true;
			}
			else if ( event_code == this->_modifier_up_event )
			{
				this->_modifier_state = false;
				this->_state_just_changed = false;
			}
			else if ( event_code == this->_update_rate_event )
			{
				++this->_update_cycles;

				if ( this->_update_cycles == Hotkey::KEEP_UP_CYCLES )
				{
					this->_state_just_changed = false;
					this->_update_cycles = 0;
				}
			}


			if ( this->_down_event != 0  )
			{
				if ( ( this->_state_just_changed  &&  this->_update_cycles == 0 )  ||  ( this->continuous_fire()  &&  this->is_down() ) )
					TEXEngine::Core::trigger_event(this->_down_event,TEXEngine::Core::Message(0,0,0));
			}
		}
	};

};	/* X3000 */