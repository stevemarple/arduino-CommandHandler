#include <string.h>

#include <CommandHandler.h>

//#define COMMAND_HANDLER_DEBUG


bool CommandHandler::startsWith(const char *match, const char *str, char **ep)
{
	char c;
	while ((c = *match) != '\0') {
		if (*str++ != c)
			return false;
		else
			++match;
	}
	if (ep)
		*ep = (char*)str;
	return true;
}


/*
bool CommandHandler::startsWith_P(const char *match, const char *str, char **ep)
{
	char c;
	while ((c = pgm_read_byte_far((uint_farptr_t)match)) != '\0') {
		if (*str++ != c)
			return false;
		else
			++match;
	}
	if (ep)
		*ep = (char*)str;
	return true;
}
*/


void CommandHandler::begin(char *buffer, int bufferLength, CommandOption *options, int optionsLength, void (*unknownCommandCallback)(const char*), void (*commandTooLongCallback)(int))
{
	_buffer = buffer;
	_bufferLength = bufferLength;
	_options = options;
	_optionsLength = optionsLength;
	_unknownCommandCallback = unknownCommandCallback;
	_commandTooLongCallback = commandTooLongCallback;
	resetBuffer();
}


void CommandHandler::resetBuffer(void)
{
	memset(_buffer, 0, _bufferLength);
	_ptr = _buffer; 
}


bool CommandHandler::process(Stream &stream)
{
	if (!stream.available() || _buffer == NULL || _bufferLength == 0)
		return false;
	
	char c = stream.read();

	if ((c == '\r' || c == '\n')) {
		// Length of command, excluding terminating NULL. This is not
		// the length of the string as commands which are too long are
		// not stored completely!
		int commandLength = _ptr - _buffer;

		if (commandLength >= _bufferLength) {
			if (_commandTooLongCallback != NULL)
				_commandTooLongCallback(commandLength);
			resetBuffer();
			return true;
		}

		if (_ptr == _buffer)
			// Buffer is empty (only received '\r' or '\n')
			return true;

		// Ensure command string is terminated
		*_ptr = '\0';

#ifdef COMMAND_HANDLER_DEBUG
		stream.println("EOL received");
		stream.print("Buffer: ");
		stream.println(_buffer);
		stream.print("Command length: ");
		stream.println(strlen(_buffer));
		stream.println((long)_buffer, HEX);
		stream.println((long)_ptr, HEX);
		stream.flush();
#endif

		// Check for matching substring
		bool found = false;
		for (int i = 0; i < _optionsLength; ++i) {
#ifdef COMMAND_HANDLER_DEBUG
			stream.print("Checking for command ");
			stream.println(_options[i].getCommand());
#endif
			char *ep;
			if (startsWith(_options[i].getCommand(), _buffer, &ep)) {
#ifdef COMMAND_HANDLER_DEBUG 
				stream.print("Found matching command: "); 
				stream.println(_options[i].getCommand());
#endif
				found = true;
				_options[i].callback(ep);
				break;
			}
		}

		if (!found && _unknownCommandCallback != NULL) {
#ifdef COMMAND_HANDLER_DEBUG
			stream.print("Command is unknown: ");
			stream.println(_buffer);
#endif
			_unknownCommandCallback(_buffer);
		}
		
		resetBuffer();

	}
	else {
		if (_ptr - _buffer < (_bufferLength - 1))
			// Insert character into buffer to process later, leaving
			// room to append a terminating '\0' character.
			*_ptr = c;

		++_ptr;
	}
	
	return true;
}


