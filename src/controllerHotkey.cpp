#include	"controllerHotkey.hpp"



namespace	X3000
{

	void	ControllerHotkey::_register_key()
	{
		if ( this->_keybind >= DPAD_UP )
		{
			this->_key_down_event = TEXEngine::Core::KEYBOARD_KEY_CANCEL_DOWN + this->_keybind;
			this->_key_up_event = this->_key_down_event + (TEXEngine::Core::XBOX360_CONTROLLER_DPAD_UP_UP - TEXEngine::Core::XBOX360_CONTROLLER_DPAD_UP_DOWN);
		
		}
		

		this->register_event(this->_update_rate_event);
		this->register_event(this->_key_down_event);
		this->register_event(this->_key_up_event);
		this->_state = false;
		this->_state_just_changed = false;
	};

	void	ControllerHotkey::_register_possible_keys()
	{
		for ( std::vector<unsigned long>::const_iterator whitelist_event_iterator = this->_whitelist.begin();  whitelist_event_iterator != this->_whitelist.end();  ++whitelist_event_iterator )
		{
			if ( (*whitelist_event_iterator) >= PSP_START )
				this->register_event(TEXEngine::Core::KEYBOARD_KEY_CANCEL_DOWN + (*whitelist_event_iterator));
		}
	};

	void	ControllerHotkey::_unregister_key()
	{
		this->unregister_event(this->_update_rate_event);
		this->unregister_event(this->_key_down_event);
		this->unregister_event(this->_key_up_event);
		this->_state = false;
		this->_state_just_changed = false;
	};

	void	ControllerHotkey::_unregister_possible_keys()
	{
		for ( std::vector<unsigned long>::const_iterator whitelist_event_iterator = this->_whitelist.begin();  whitelist_event_iterator != this->_whitelist.end();  ++whitelist_event_iterator )
		{
			if ( (*whitelist_event_iterator) >= PSP_START  &&  (*whitelist_event_iterator) <= RIGHTTRIGGER )
				this->unregister_event(TEXEngine::Core::KEYBOARD_KEY_CANCEL_DOWN + (*whitelist_event_iterator));
		}
	};
	
	void	ControllerHotkey::_request_change()
	{
		this->_change_keybind = true;
		this->_previous_keybind = this->_keybind;
		this->_unregister_key();
		this->_register_possible_keys();
	};
	
	void	ControllerHotkey::_abort_change()
	{
		this->_change_keybind = false;
		this->_keybind = this->_previous_keybind;
		this->_unregister_possible_keys();
		this->_register_key();
	};

	void	ControllerHotkey::_finalize_change()
	{
		this->_change_keybind = false;
		this->_previous_keybind = this->_keybind;
		this->_unregister_possible_keys();
		this->_register_key();
	};
	
	void	ControllerHotkey::_change_update_rate( const unsigned long  update_rate )
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

	bool	ControllerHotkey::_code_available( const unsigned long event_code )
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


	ControllerHotkey::ControllerHotkey( const unsigned long id ,  const unsigned long controller_id , const unsigned long keybind , const unsigned long update_rate , const unsigned long down_event , const std::vector<unsigned long>& whitelist )	:	
		Hotkey(id) , 
		_controller_id(controller_id) , 
		_whitelist(whitelist) , 
		_keybind(PSP_START) , _previous_keybind(_keybind) ,
		_update_rate(update_rate) ,
		_key_down_event(0) , _key_up_event(0) , 
		_update_rate_event(TEXEngine::Core::UPDATE + update_rate) ,
		_down_event(down_event) ,  
		_update_cycles(0) , 
		_state(false) ,	_state_just_changed(false) , _continuous_fire(false) , _change_keybind(false)
	{
		this->keybind(keybind);
		this->enable();
	};

	ControllerHotkey::~ControllerHotkey()	{ this->disable(); };


	void	ControllerHotkey::on_event( const unsigned long event_code , const TEXEngine::Core::Message& parameters )
	{
		if ( parameters.get_integral_data(2) == this->controller_id() )
		{
			if ( this->_change_keybind )
			{
				if ( this->_code_available(event_code) )
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
				TEXEngine::Core::Message	out_parameters(3,0,0);



				out_parameters.add_integral_data(0,static_cast<TEXEngine::Core::MESSAGE_INTEGRAL_DATA_TYPE>(this->get_id()));
				out_parameters.add_integral_data(2,static_cast<TEXEngine::Core::MESSAGE_INTEGRAL_DATA_TYPE>(this->controller_id()));

				if ( event_code == this->_key_down_event )
				{
					if ( !this->_state )
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
						TEXEngine::Core::trigger_event(this->_down_event,out_parameters);
				}
			}
		}
	};

};	/* X3000 */