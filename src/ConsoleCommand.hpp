#ifndef XSLEDDING_CONSOLECOMMAND_HPP_INCLUDED
#define XSLEDDING_CONSOLECOMMAND_HPP_INCLUDED

class ConsoleCommand{
public:
	ConsoleCommand();
	virtual ~ConsoleCommand();

	virtual int execute(const char* params) = 0;
};

class ClearCommand : public ConsoleCommand{
public:
	ClearCommand();
	~ClearCommand();
	int execute(const char* params);
};


class QuitCommand : public ConsoleCommand{
public:
	QuitCommand();
	~QuitCommand();
	int execute(const char* params);
};

class DebugRenderingCommand : public ConsoleCommand{
public:
	DebugRenderingCommand();
	~DebugRenderingCommand();
	int execute(const char* params);
};

class TreatHackCommand : public ConsoleCommand{
public:
	TreatHackCommand();
	~TreatHackCommand();
	int execute(const char* params);
};

class ClearColorCommand : public ConsoleCommand{
public:
	ClearColorCommand();
	~ClearColorCommand();
	int execute(const char* params);
};

class ConsoleBackgroundColorCommand : public ConsoleCommand{
public:
	ConsoleBackgroundColorCommand();
	~ConsoleBackgroundColorCommand();
	int execute(const char* params);
};



class NoMechanicsCommand : public ConsoleCommand{
public:
	NoMechanicsCommand();
	~NoMechanicsCommand();
	int execute(const char* params);
};



class PlayerPositionCommand : public ConsoleCommand{
public:
	PlayerPositionCommand();
	~PlayerPositionCommand();
	int execute(const char* params);
};

class AIPositionCommand : public ConsoleCommand{
public:
	AIPositionCommand();
	~AIPositionCommand();
	int execute(const char* params);
};


#endif