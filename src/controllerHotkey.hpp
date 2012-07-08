
#include	"hotkey.hpp"

#ifndef		X_SLEDDING_3000_CONTROLLERHOTKEY_HPP_INCLUDED
	#define	X_SLEDDING_3000_CONTROLLERHOTKEY_HPP_INCLUDED



	namespace	X3000
	{
		/*
			Class responsible for handling a controller key event and passing an event along
		*/
		class	ControllerHotkey	:	public Hotkey
		{
			private:

				//	An array containing the allowed keys for the hotkey.
				std::vector<unsigned long>	_whitelist;
				//	The controller assigned to this kotkey.
				unsigned long				_controller_id;
				//	The key, from the HotkeyKeys enumeration , bound to this hotkey.
				unsigned long				_keybind;
				//	The last keybind. Used when changing the keybind.
				unsigned long				_previous_keybind;
				//	The update rate, from the HotkeyUpdateRate enumeration , used to check if the button was first pressed.
				unsigned long				_update_rate;
				//	The down event that corresponds to the _keybind variable.
				unsigned long				_key_down_event;
				//	The up event that corresponds to the _keybind variable.
				unsigned long				_key_up_event;
				//	The update event that corresponds to the _update_rate variable.
				unsigned long				_update_rate_event;
				//	The event that will be triggered once the button is first pressed.
				unsigned long				_down_event;
				//	Track the current update cycles.
				unsigned int				_update_cycles;
				//	The state of the key.
				bool						_state;
				//	Whether the key was first pressed.
				bool						_state_just_changed;
				//	Whether the button will fire the down event while it is pressed, not only the first time it is pressed
				bool						_continuous_fire;
				//	Whether a change in the keybind was initiated.
				bool						_change_keybind;


				//	Function responsible for registering the events relative to the key.
				void						_register_key();
				//	Function responsible for registering the events of possible keys contained in the whitelist.
				void						_register_possible_keys();
				//	Function responsible for unregistering events relative to the key.
				void						_unregister_key();
				//	Function responsible for unregistering the events of possible keys contained in the whitelist.
				void						_unregister_possible_keys();
				//	Function responsible for initiating a change in the key from user input.
				void						_request_change();
				//	Function responsible for canceling a change in the key from user input and reverting to the last key.
				void						_abort_change();
				//	Function responsible for finalizing a change in the key from user input.
				void						_finalize_change();
				//	Function responsible for changing the update rate and registering and unregistering any relevant events.
				void						_change_update_rate( const unsigned long update_rate );
				//	Function responsible for checking whether an event is within the whitelist.
				bool						_code_available( const unsigned long event_code );


			public:
			
				explicit ControllerHotkey( const unsigned long id = 0  ,  const unsigned long controller_id = 0 , const unsigned long keybind = PSP_START , const unsigned long update_rate = DEFAULT , const unsigned long down_event = 0 , const std::vector<unsigned long>& whitelist = std::vector<unsigned long>(0,0) );
				virtual ~ControllerHotkey();


				void						on_event( const unsigned long event_code , const TEXEngine::Core::Message& parameters );

				//	Enable the hotkey.
				void						enable();
				//	Disable the hotkey.
				void						disable();
				//	Initiate a change of the keybind. This change will occur from input from the user.
				void						change_keybind();
				//	Revert back to the last known keybind, canceling any pending changes in the keybind.
				void						lock_keybind();
				//	Change the controller id.
				void						controller_id( const unsigned long id );
				//	Change the keybind.
				void						keybind( const unsigned long key_code );
				//	Change the modifier.
				void						modifier( const unsigned long modifier );
				//	Change the update rate.
				void						update_rate( const unsigned long event_code );
				//	Change the event that is triggered when the button is first pressed.
				void						down_event( const unsigned long event_code );
				//	Change the list of allowed keys.
				void						whitelist( const std::vector<unsigned long>& whitelist );
				//	Enable firing the down event while the button is down, not only when the button is first pressed
				void						continuous_fire( const bool value );


				//	Get the controller id.
				unsigned long				controller_id() const;
				//	Get the keybind.
				unsigned long				keybind() const;
				//	Get the modifier.
				unsigned long				modifier() const;
				//	Get the update rate.
				unsigned long				update_rate() const;
				//	Get the event that is fired when the button is first pressed.
				unsigned long				down_event() const;
				//	Get the list of allowed keys.
				std::vector<unsigned long>	whitelist() const;
				//	Check if the button is down.
				bool						is_down() const;
				//	Check if the button is down, this function returns true only when the button is first pressed.
				bool						is_pressed() const;
				//	Check if the button will fire the down event while it is pressed
				bool						continuous_fire() const;
				//	Check if a change of keybind is enabled
				bool						change_pending() const;
				//	Get a string representing the hotkey
				std::string					get_as_text() const;
		};



		/*
			Function implementations
		*/


		inline void							ControllerHotkey::enable()													{ this->_register_key(); };
		inline void							ControllerHotkey::disable()													{ this->_unregister_key(); };
		inline void							ControllerHotkey::change_keybind()											{ this->_request_change(); };
		inline void							ControllerHotkey::lock_keybind()											{ this->_abort_change(); };
		inline void							ControllerHotkey::controller_id( const unsigned long id )					{ this->_controller_id = id; };
		inline void							ControllerHotkey::keybind( const unsigned long key_code )
		{
			if ( ( key_code >= PSP_START   &&  key_code <= RIGHTTRIGGER )  ||  key_code == NO_KEYBIND )
			{
				this->_unregister_key();
				this->_keybind = key_code;
				this->_previous_keybind = this->_keybind;
				this->_register_key();
			}
		};

		inline void							ControllerHotkey::modifier( const unsigned long )							{};
		inline void							ControllerHotkey::update_rate( const unsigned long event_code )				{ this->_change_update_rate(event_code); };
		inline void							ControllerHotkey::down_event( const unsigned long event_code )				{ this->_down_event = event_code; };
		inline void							ControllerHotkey::whitelist( const std::vector<unsigned long>& whitelist )	{ this->_whitelist = whitelist; };
		inline void							ControllerHotkey::continuous_fire( const bool value )						{ this->_continuous_fire = value; };


		inline unsigned long				ControllerHotkey::controller_id() const										{ return this->_controller_id; };
		inline unsigned long				ControllerHotkey::keybind() const											{ return this->_keybind; };
		inline unsigned long				ControllerHotkey::modifier() const											{ return NONE; };
		inline unsigned long				ControllerHotkey::update_rate() const										{ return this->_update_rate; };
		inline unsigned long				ControllerHotkey::down_event() const										{ return this->_down_event; };
		inline std::vector<unsigned long>	ControllerHotkey::whitelist() const											{ return this->_whitelist; };
		inline bool							ControllerHotkey::is_down() const											{ return this->_state; };
		inline bool							ControllerHotkey::is_pressed() const										{ return this->_state_just_changed; };
		inline bool							ControllerHotkey::continuous_fire() const									{ return this->_continuous_fire; };
		inline bool							ControllerHotkey::change_pending() const									{ return this->_change_keybind; };
		inline std::string					ControllerHotkey::get_as_text() const										{ return this->_get_key_as_text(this->keybind(),this->modifier()); };


	}	/* X3000 */



#endif		/* X_SLEDDING_3000_CONTROLLERHOTKEY_HPP_INCLUDED */