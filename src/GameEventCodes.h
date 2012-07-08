#ifndef XSLEDDING_GAMEVENTCODES_H_INCLUDED
#define XSLEDDING_GAMEVENTCODES_H_INCLUDED

static const unsigned int FETCH_MENU_PARAMS = 0x10000;
static const unsigned int BACK_TO_MENU = 0x10001;
static const unsigned int QUIT_GAME = 0x10002;
static const unsigned int SCENE_PAUSED = 0x10003; 
static const unsigned int SCENE_QUIT = 0x10004;
static const unsigned int NEXT_CAMERA = 0x10005;
static const unsigned int PREVIOUS_CAMERA = 0x10006;
static const unsigned int REAR_VIEW = 0x10007;
static const unsigned int RESET_POSITION = 0x10008;

static const unsigned int SAVE_OPTIONS = 0x10010;
static const unsigned int CANCEL_OPTIONS = 0x10011;
static const unsigned int CANCEL_KEYBIND_CHANGE = 0x10012;
static const unsigned int KEYBIND_UNBOUND_MESSAGE = 0x10013;
static const unsigned int TOGGLE_CONSOLE = 0x10014;
static const unsigned int ENABLE_MENU_KEYS = 0x10015;
static const unsigned int DISABLE_MENU_KEYS = 0x10016;

static const unsigned int REQUEST_WHIP_PCKEY_REBIND = 0x10020;
static const unsigned int REQUEST_WHIP_CONTROLLERKEY_REBIND = 0x10021;
static const unsigned int REQUEST_TREAT_PCKEY_REBIND = 0x10022;
static const unsigned int REQUEST_TREAT_CONTROLLERKEY_REBIND = 0x10023;
static const unsigned int REQUEST_BRAKE_PCKEY_REBIND = 0x10024;
static const unsigned int REQUEST_BRAKE_CONTROLLERKEY_REBIND = 0x10025;
static const unsigned int REQUEST_LEFT_PCKEY_REBIND = 0x10026;
static const unsigned int REQUEST_LEFT_CONTROLLERKEY_REBIND = 0x10027;
static const unsigned int REQUEST_RIGHT_PCKEY_REBIND = 0x10028;
static const unsigned int REQUEST_RIGHT_CONTROLLERKEY_REBIND = 0x10029;
static const unsigned int REQUEST_NEXTCAMERA_PCKEY_REBIND = 0x1002a;
static const unsigned int REQUEST_NEXTCAMERA_CONTROLLERKEY_REBIND = 0x1002b;
static const unsigned int REQUEST_PREVIOUSCAMERA_PCKEY_REBIND = 0x1002c;
static const unsigned int REQUEST_PREVIOUSCAMERA_CONTROLLERKEY_REBIND = 0x1002d;
static const unsigned int REQUEST_RESETPOSITION_PCKEY_REBIND = 0x1002e;
static const unsigned int REQUEST_RESETPOSITION_CONTROLLERKEY_REBIND = 0x1002f;
static const unsigned int REQUEST_REARVIEW_PCKEY_REBIND = 0x10030;
static const unsigned int REQUEST_REARVIEW_CONTROLLERKEY_REBIND = 0x10031;

//trigger one of these events giving the text representation of the key as first pointer parameter so that lua can display the new hotkey.
static const unsigned int WHIP_PCKEY_REBOUND = 0x10050;
static const unsigned int WHIP_CONTROLLERKEY_REBOUND = 0x10051;
static const unsigned int TREAT_PCKEY_REBOUND = 0x10052;
static const unsigned int TREAT_CONTROLLERKEY_REBOUND = 0x10053;
static const unsigned int BRAKE_PCKEY_REBOUND = 0x10054;
static const unsigned int BRAKE_CONTROLLERKEY_REBOUND = 0x10055;
static const unsigned int LEFT_PCKEY_REBOUND = 0x10056;
static const unsigned int LEFT_CONTROLLERKEY_REBOUND = 0x10057;
static const unsigned int RIGHT_PCKEY_REBOUND = 0x10058;
static const unsigned int RIGHT_CONTROLLERKEY_REBOUND = 0x10059;
static const unsigned int NEXT_CAMERA_PCKEY_REBOUND = 0x1005a;
static const unsigned int NEXT_CAMERA_CONTROLLERKEY_REBOUND = 0x1005b;
static const unsigned int PREVIOUS_CAMERA_PCKEY_REBOUND = 0x1005c;
static const unsigned int PREVIOUS_CAMERA_CONTROLLERKEY_REBOUND = 0x1005d;
static const unsigned int RESET_POSITION_PCKEY_REBOUND = 0x1005e;
static const unsigned int RESET_POSITION_CONTROLLERKEY_REBOUND = 0x1005f;
static const unsigned int REARVIEW_PCKEY_REBOUND = 0x10060;
static const unsigned int REARVIEW_CONTROLLERKEY_REBOUND = 0x10061;


static const unsigned int SET_SOUND_VOLUME = 0x10080;
static const unsigned int SET_MUSIC_VOLUME = 0x10081;
static const unsigned int RESIZE_WINDOWS = 0x10082;

static const unsigned int END_SPLASH1 = 0x10090;
static const unsigned int SEND_SCORES = 0x10091;
//static const unsigned int SEND_SCORES = 0x10091;


static const unsigned int PLAY_OnClick = 0x100a0;
static const unsigned int PLAY_OverClick = 0x100a1;
static const unsigned int PLAY_GO = 0x100a2;
static const unsigned int PLAY_UNGO = 0x100a3;
static const unsigned int PLAY_MENU_MUSIC = 0x100a4;


static const unsigned int PLAYER_STATS = 0x10100;
static const unsigned int SCENE_ISPAUSED = 0x10101;
static const unsigned int SCENE_ISRESUMED = 0x10102;
static const unsigned int SCENE_FINISHED = 0x10103;
static const unsigned int PLAYER_POSITION = 0x10104;

static const unsigned int WHIP_HAPPENED = 0x10110;
static const unsigned int TREAT_HAPPENED = 0x10111;
static const unsigned int BRAKE_HAPPENED = 0x10112;
static const unsigned int LEFT_HAPPENED = 0x10113;
static const unsigned int RIGHT_HAPPENED = 0x10114;


static const unsigned int TEST_ASPECT_RATIO = 0x10120;

static const unsigned int CONSOLE_LOG = 0x10130;
static const unsigned int UPDATE_LOG = 0x10131;



#endif