
#include	<vector>
#include	<string>
#include	"message\TEXEngineInterface.hpp"

#ifndef		X_SLEDDING_3000_HOTKEY_HPP_INCLUDED
	#define	X_SLEDDING_3000_HOTKEY_HPP_INCLUDED



	namespace	X3000
	{

		//	An enumeration that contains the possible modifiers to the key.
		enum	HotkeyModifier		{	NONE = 3 , SHIFT = 0 , CTRL , ALT };
		//	An enumeration that contains the possible update rates of the key
		enum	HotkeyUpdateRate	{	DEFAULT = 0 , FAST , MEDIUM , SLOW};
		//	An enumeration that contains the possible keys that can be associated with the hotkey.
		enum	HotkeyKeys			{	CANCEL = 0 , BACKSPACE = 1 , TAB = 2 , CLEAR = 5 , RETURN = 6 , PAUSE = 12 , CAPSLOCK = 13 , ESC = 20 , SPACE = 25 , PAGEUP = 26 , PAGEDOWN = 27 ,
										END = 28 , HOME = 29 , LEFTARROW = 30 , UPARROW = 31 , RIGHTARROW = 32 , DOWNARROW = 33 , SELECT = 34 , EXECUTE = 36 , PRINTSCREEN = 37 , INSERT = 38 , KDELETE = 39 , HELP = 40 , ZERO = 41 ,
										ONE = 42 , TWO = 43 , THREE = 44 , FOUR = 45 , FIVE = 46 , SIX = 47 , SEVEN = 48 , EIGHT = 49 , NINE = 50 , A = 58 , B = 59 , C = 60 , D = 61 , E = 62 , F = 63 , G = 64 , H = 65 , I = 66 ,
										J = 67 , K = 68 , L = 69 , M = 70 , N = 71 , O = 72 , P = 73 , Q = 74 , R = 75 , S = 76 , T = 77 , U = 78 , V = 79 , W = 80 , X = 81 , Y = 82 , Z = 83 , LEFTWINDOWS = 84 , RIGHTWINDOWS = 85 ,
										APPLICATION = 86 , NUMPAD0 = 89 , NUMPAD1 = 90 , NUMPAD2 = 91 , NUMPAD3 = 92 , NUMPAD4 = 93 , NUMPAD5 = 94 , NUMPAD6 = 95 , NUMPAD7 = 96 , NUMPAD8 = 97 , NUMPAD9 = 98 , MULTIPLY = 99 , ADD = 100 ,
										SEPARATOR = 101 , SUBTRACT = 102 , DECIMAL = 103 , DIVIDE = 104 , F1 = 105 , F2 = 106 , F3 = 107 , F4 = 108 , F5 = 109 , F6 = 110 , F7 = 111 , F8 = 112 , F9 = 113 , F10 = 114 , F11 = 115 , F12 = 116 ,
										F13 = 117 , F14 = 118 , F15 = 119 , F16 = 120 , F17 = 121 , F18 = 122 , F19 = 123 , F20 = 124 , F21 = 125 , F22 = 126 , F23 = 127 , F24 = 128 , NUMLOCK = 137 , SCROLLLOCK = 138 , SEMICOLON = 180 ,
										PLUS = 181 , COMMA = 182 , MINUS = 183 , DOT = 184 , FORWARDSLASH = 185 , TILDE = 186 , LEFTBRACKET = 215 , BACKWARDSSLASH = 216 , RIGHTBRACKET = 217 , DOUBLEQUOTES = 218 , PACKET = 227 , ATTN = 244 ,
										CRSEL = 245 , EXSEL = 246 , EREOF = 247 , PLAY = 248 , ZOOM = 249 , NONAME = 250 , PA1 = 251 , OEMCLEAR = 252 ,
										PSP_START = 763 , PSP_SELECT , PSP_UPARROW , PSP_LEFTARROW , PSP_DOWNARROW , PSP_RIGHTARROW , PSP_TRIANGLE , PSP_SQUARE , PSP_CROSS , PSP_CIRCLE , PSP_LEFTTRIGGER , PSP_RIGHTTRIGGER ,
										PS3_SELECT = 1033 , PS3_START , PS3_UPARROW , PS3_DOWNARROW , PS3_LEFTARROW , PS3_RIGHTARROW , PS3_CIRCLE , PS3_CROSS , PS3_SQUARE , PS3_TRIANGLE , PS3_L1TRIGGER , PS3_L2TRIGGER , PS3_L3TRIGGER ,
										PS3_R1TRIGGER , PS3_R2TRIGGER , PS3_R3TRIGGER ,
										DPAD_UP = 1277 , DPAD_DOWN, DPAD_LEFT , DPAD_RIGHT , START , BACK , LEFTSTICK , RIGHTSTICK , LEFTSHOULDER , RIGHTSHOULDER ,
										XB_A , XB_B , XB_X , XB_Y , LEFTTRIGGER , RIGHTTRIGGER , 
										NO_KEYBIND = 999999 };
		/*
			Class responsible for handling a key event and passing an event along
		*/
		class	Hotkey	:	public TEXEngine::Core::Object
		{
			protected:

				static const unsigned int			KEEP_UP_CYCLES = 1;

				static std::string					_get_key_as_text( const unsigned int key , const unsigned int modifier );


			public:

				static	bool						check_key( const unsigned int key ); 

				explicit Hotkey( const unsigned long id = 0 );
				virtual ~Hotkey();


				//	Enable the hotkey.
				virtual void						enable() = 0;
				//	Disable the hotkey.
				virtual void						disable() = 0;
				//	Initiate a change of the keybind. This change will occur from input from the user.
				virtual void						change_keybind() = 0;
				//	Revert back to the last known keybind, canceling any pending changes in the keybind.
				virtual void						lock_keybind() = 0;
				//	Change the keybind.
				virtual void						keybind( const unsigned long key_code ) = 0;
				//	Change the modifier.
				virtual void						modifier( const unsigned long modifier ) = 0;
				//	Change the update rate.
				virtual void						update_rate( const unsigned long event_code ) = 0;
				//	Change the event that is triggered when the button is first pressed.
				virtual void						down_event( const unsigned long event_code ) = 0;
				//	Change the list of allowed keys.
				virtual void						whitelist( const std::vector<unsigned long>& whitelist ) = 0;
				//	Enable firing the down event while the button is down, not only when the button is first pressed
				virtual void						continuous_fire( const bool value ) = 0;


				//	Get the keybind.
				virtual unsigned long				keybind() const = 0;
				//	Get the modifier.
				virtual unsigned long				modifier() const = 0;
				//	Get the update rate.
				virtual unsigned long				update_rate() const = 0;
				//	Get the event that is fired when the button is first pressed.
				virtual unsigned long				down_event() const = 0;
				//	Get the list of allowed keys.
				virtual std::vector<unsigned long>	whitelist() const = 0;
				//	Check if the button is down.
				virtual bool						is_down() const = 0;
				//	Check if the button is down, this function returns true only when the button is first pressed.
				virtual bool						is_pressed() const = 0;
				//	Check if the button will fire the down event while it is pressed
				virtual bool						continuous_fire() const = 0;
				//	Check if a change of keybind is enabled
				virtual bool						change_pending() const = 0;
				//	Get a string representing the hotkey
				virtual std::string					get_as_text() const = 0;
		};



		/*
			Function Implementations
		*/


		inline std::string	Hotkey::_get_key_as_text( const unsigned int key , const unsigned int modifier )
		{
			std::string	text("");



			if ( modifier == SHIFT )
				text = "SHIFT+";
			else if ( modifier == CTRL )
				text = "CTRL+";
			else if ( modifier == ALT )
				text = "ALT+";

			switch ( key )
			{
				case	BACKSPACE:
										text += "BCKSPC";
										break;
				case	TAB:
										text += "TAB";
										break;
				case	CLEAR:
										text += "CLR";
										break;
				case	RETURN:
										text += "RTRN";
										break;
				case	PAUSE:
										text += "PAUSE";
										break;
				case	CAPSLOCK:
										text += "CAPS";
										break;
				case	ESC:
										text += "ESC";
										break;
				case	SPACE:
										text += "SPACE";
										break;
				case	PAGEUP:
										text += "PAGEUP";
										break;
				case	PAGEDOWN:
										text += "PAGEDN";
										break;
				case	END:
										text += "END";
										break;
				case	HOME:
										text += "HOME";
										break;
				case	LEFTARROW:
										text += "LEFT";
										break;
				case	UPARROW:
										text += "UP";
										break;
				case	RIGHTARROW:
										text += "RIGHT";
										break;
				case	DOWNARROW:
										text += "DOWN";
										break;
				case	SELECT:
										text += "SELECT";
										break;
				case	EXECUTE:
										text += "EXECUTE";
										break;
				case	PRINTSCREEN:
										text += "PRNTSCRN";
										break;
				case	INSERT:
										text += "INS";
										break;
				case	KDELETE:
										text += "DEL";
										break;
				case	HELP:
										text += "HELP";
										break;
				case	ZERO:
										text += "0";
										break;
				case	ONE:
										text += "1";
										break;
				case	TWO:
										text += "2";
										break;
				case	THREE:
										text += "3";
										break;
				case	FOUR:
										text += "4";
										break;
				case	FIVE:
										text += "5";
										break;
				case	SIX:
										text += "6";
										break;
				case	SEVEN:
										text += "7";
										break;
				case	EIGHT:
										text += "8";
										break;
				case	NINE:
										text += "9";
										break;
				case	A:
										text += "A";
										break;
				case	B:
										text += "B";
										break;
				case	C:
										text += "C";
										break;
				case	D:
										text += "D";
										break;
				case	E:
										text += "E";
										break;
				case	F:
										text += "F";
										break;
				case	G:
										text += "G";
										break;
				case	H:
										text += "H";
										break;
				case	I:
										text += "I";
										break;
				case	J:
										text += "J";
										break;
				case	K:
										text += "K";
										break;
				case	L:
										text += "L";
										break;
				case	M:
										text += "M";
										break;
				case	N:
										text += "N";
										break;
				case	O:
										text += "O";
										break;
				case	P:
										text += "P";
										break;
				case	Q:
										text += "Q";
										break;
				case	R:
										text += "R";
										break;
				case	S:
										text += "S";
										break;
				case	T:
										text += "T";
										break;
				case	U:
										text += "U";
										break;
				case	V:
										text += "V";
										break;
				case	W:
										text += "W";
										break;
				case	X:
										text += "X";
										break;
				case	Y:
										text += "Y";
										break;
				case	Z:
										text += "Z";
										break;
				case	LEFTWINDOWS:
										text += "LWIN";
										break;
				case	RIGHTWINDOWS:
										text += "RWIN";
										break;
				case	APPLICATION:
										text += "APP";
										break;
				case	NUMPAD0:
										text += "NUM0";
										break;
				case	NUMPAD1:
										text += "NUM1";
										break;
				case	NUMPAD2:
										text += "NUM2";
										break;
				case	NUMPAD3:
										text += "NUM3";
										break;
				case	NUMPAD4:
										text += "NUM4";
										break;
				case	NUMPAD5:
										text += "NUM5";
										break;
				case	NUMPAD6:
										text += "NUM6";
										break;
				case	NUMPAD7:
										text += "NUM7";
										break;
				case	NUMPAD8:
										text += "NUM8";
										break;
				case	NUMPAD9:
										text += "NUM9";
										break;
				case	MULTIPLY:
										text += "MULT";
										break;
				case	ADD:
										text += "ADD";
										break;
				case	SEPARATOR:
										text += "SEP";
										break;
				case	SUBTRACT:
										text += "SUB";
										break;
				case	DECIMAL:
										text += "DEC";
										break;
				case	DIVIDE:
										text += "DIV";
										break;
				case	F1:
										text += "F1";
										break;
				case	F2:
										text += "F2";
										break;
				case	F3:
										text += "F3";
										break;
				case	F4:
										text += "F4";
										break;
				case	F5:
										text += "F5";
										break;
				case	F6:
										text += "F6";
										break;
				case	F7:
										text += "F7";
										break;
				case	F8:
										text += "F8";
										break;
				case	F9:
										text += "F9";
										break;
				case	F10:
										text += "F10";
										break;
				case	F11:
										text += "F11";
										break;
				case	F12:
										text += "F12";
										break;
				case	F13:
										text += "F13";
										break;
				case	F14:
										text += "F14";
										break;
				case	F15:
										text += "F15";
										break;
				case	F16:
										text += "F16";
										break;
				case	F17:
										text += "F17";
										break;
				case	F18:
										text += "F18";
										break;
				case	F19:
										text += "F19";
										break;
				case	F20:
										text += "F20";
										break;
				case	F21:
										text += "F21";
										break;
				case	F22:
										text += "F22";
										break;
				case	F23:
										text += "F23";
										break;
				case	F24:
										text += "F24";
										break;
				case	NUMLOCK:
										text += "NUMLCK";
										break;
				case	SCROLLLOCK:
										text += "SCRLLCK";
										break;
				case	SEMICOLON:
										text += "SEMICLN";
										break;
				case	PLUS:
										text += "=";
										break;
				case	COMMA:
										text += ",";
										break;
				case	MINUS:
										text += "-";
										break;
				case	DOT:
										text += ".";
										break;
				case	FORWARDSLASH:
										text += "/";
										break;
				case	TILDE:
										text += "~";
										break;
				case	LEFTBRACKET:
										text += "[";
										break;
				case	BACKWARDSSLASH:
										text += "\\";
										break;
				case	RIGHTBRACKET:
										text += "]";
										break;
				case	DOUBLEQUOTES:
										text += "'";
										break;
				case	PACKET:
										text += "PACKET";
										break;
				case	ATTN:
										text += "ATTN";
										break;
				case	CRSEL:
										text += "CRSEL";
										break;
				case	EXSEL:
										text += "EXSEL";
										break;
				case	EREOF:
										text += "EREOF";
										break;
				case	PLAY:
										text += "PLAY";
										break;
				case	ZOOM:
										text += "ZOOM";
										break;
				case	NONAME:
										text += "NONAME";
										break;
				case	PA1:
										text += "PA1";
										break;
				case	OEMCLEAR:
										text += "OEMCLEAR";
										break;
				case	PSP_START:
										text += "START";
										break;
				case	PSP_SELECT:
										text += "SELECT";
										break;
				case	PSP_UPARROW:
										text += "UP";
										break;
				case	PSP_LEFTARROW:
										text += "LEFT";
										break;
				case	PSP_DOWNARROW:
										text += "DOWN";
										break;
				case	PSP_RIGHTARROW:
										text += "RIGHT";
										break;
				case	PSP_TRIANGLE:
										text += "TRIANGLE";
										break;
				case	PSP_SQUARE:
										text += "SQUARE";
										break;
				case	PSP_CROSS:
										text += "CROSS";
										break;
				case	PSP_CIRCLE:
										text += "CIRCLE";
										break;
				case	PSP_LEFTTRIGGER:
										text += "L";
										break;
				case	PSP_RIGHTTRIGGER:
										text += "R";
										break;
				case	PS3_SELECT:
										text += "SELECT";
										break;
				case	PS3_START:
										text += "START";
										break;
				case	PS3_UPARROW:
										text += "UP";
										break;
				case	PS3_DOWNARROW:
										text += "DOWN";
										break;
				case	PS3_LEFTARROW:
										text += "LEFT";
										break;
				case	PS3_RIGHTARROW:
										text += "RIGHT";
										break;
				case	PS3_CIRCLE:
										text += "CIRCLE";
										break;
				case	PS3_CROSS:
										text += "CROSS";
										break;
				case	PS3_SQUARE:
										text += "SQUARE";
										break;
				case	PS3_TRIANGLE:
										text += "TRIANGLE";
										break;
				case	PS3_L1TRIGGER:
										text += "L1";
										break;
				case	PS3_L2TRIGGER:
										text += "L2";
										break;
				case	PS3_L3TRIGGER:
										text += "L3";
										break;
				case	PS3_R1TRIGGER:
										text += "R1";
										break;
				case	PS3_R2TRIGGER:
										text += "R2";
										break;
				case	PS3_R3TRIGGER:
										text += "R3";
										break;
				case	DPAD_UP:
										text += "UP";
										break;
				case	DPAD_DOWN:
										text += "DOWN";
										break;
				case	DPAD_LEFT:
										text += "LEFT";
										break;
				case	DPAD_RIGHT:
										text += "RIGHT";
										break;
				case	START:
										text += "START";
										break;
				case	BACK:
										text += "BACK";
										break;
				case	LEFTSTICK:
										text += "LSTICK";
										break;
				case	RIGHTSTICK:
										text += "RSTICK";
										break;
				case	LEFTSHOULDER:
										text += "L";
										break;
				case	RIGHTSHOULDER:
										text += "R";
										break;
				case	XB_A:
										text += "A";
										break;
				case	XB_B:
										text += "B";
										break;
				case	XB_X:
										text += "X";
										break;
				case	XB_Y:
										text += "Y";
										break;
				case	LEFTTRIGGER:
										text += "LTRIGGER";
										break;
				case	RIGHTTRIGGER:
										text += "RTRIGGER";
										break;
			}


			return text;
		};

		inline bool	Hotkey::check_key( const unsigned int key )
		{
			bool						return_value = false;
			std::string					text("");
			


			text += Hotkey::_get_key_as_text(key,NONE);

			if ( text != "" )
				return_value = true;


			return return_value;
		};

	}	/* X3000 */



#endif		/* X_SLEDDING_3000_HOTKEY_HPP_INCLUDED */