#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#define COMMAND_HANDLER_VERSION "0.1.1"

#include <Arduino.h>

class CommandOption {
public:

	inline CommandOption(const char* command,
						 void (*callback)(const char*))
		: _command(command), _callback(callback) {
		;
	};

	inline const char* getCommand(void) const {
		return _command;
	};

	inline void callback(const char* s) const {
		if (_callback != NULL)
			_callback(s);
	};

private:
	const char* _command;
	void (*_callback)(const char*);
};

class CommandHandler {
public:
	static bool startsWith(const char *match, const char *str, char **ep);
	// static bool startsWith_P(const char *match, const char *str, char **ep);

	inline CommandHandler(void) :
		_buffer(NULL),
		_bufferLength(0),
		_options(NULL),
		_optionsLength(0),
		_unknownCommandCallback(NULL),
		_commandTooLongCallback(NULL),
		_ptr(NULL) {
		;
	};

	void begin(char *buffer, int bufferLength, CommandOption *options, int optionsLength, void (*unknownCommandCallback)(const char*) = NULL, void (*commandTooLongCallback)(int) = NULL);
	bool process(Stream &stream);
	void resetBuffer();

private:
	char *_buffer;
	int _bufferLength;
	CommandOption *_options;
	int _optionsLength;
	void (*_unknownCommandCallback)(const char*);
	void (*_commandTooLongCallback)(int);
	char *_ptr;
};

#endif
