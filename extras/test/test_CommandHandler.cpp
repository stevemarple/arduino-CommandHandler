#include "CommandHandler.h"
#include <iostream>
#include <iomanip>
#include <unistd.h>


using namespace std;


Stream console(cin, cout);

void doCommand(const char *s)
{
	cout << "doCommand received argument \"" << s << "\"" << endl;
}


void cmdEepromRead(const char *s)
{
	cout << "cmdEepromRead received argument \"" << s << "\"" << endl;
}


void cmdEepromWrite(const char *s)
{
	cout << "cmdEepromWrite received argument \"" << s << "\"" << endl;
}


void cmdVerbosity(const char *s)
{
	cout << "cmdVerbosity received argument \"" << s << "\"" << endl;
}


void cmdReboot(const char *s)
{
	cout << "cmdReboot received argument \"" << s << "\"" << endl;
}


void cmdSamplingInterval(const char *s)
{
	cout << "cmdSamplingInterval received argument \"" << s << "\"" << endl;
}


void unknownCommand(const char *s)
{
    cout << "Unknown command: " << s << endl;
}


void commandTooLong(int)
{
    cout << "Previous command was too long for buffer\n";
}


CommandHandler commandHandler;
const int commandBufferLength = 80;
char *commandBuffer = NULL;
const int numCommands = 5;
CommandOption commands[numCommands] = {
    CommandOption("eepromRead=", cmdEepromRead),
    CommandOption("eepromWrite=", cmdEepromWrite),
    CommandOption("verbosity", cmdVerbosity),
    CommandOption("REBOOT=true", cmdReboot),
    CommandOption("samplingInterval_16th_s", cmdSamplingInterval),
};

void setup(void)
{
	if ((commandBuffer = (char*)malloc(commandBufferLength)) == NULL) {
		cerr << "Cannot allocate buffer" << endl;
		exit(1);
	}

	console.println("Reading from cin and writing to cout");
	commandHandler.begin(commandBuffer, commandBufferLength,
						 commands, numCommands,
						 unknownCommand, commandTooLong);
}



void loop()
{
	commandHandler.process(console);
}


int main(void)
{
#ifdef COMMAND_HANDLER_DEBUG
	cout << "COMMAND_HANDLER_DEBUG is defined" << endl;
#endif

	setup();

	while (true) {
		loop();
		usleep(1000);
	}

	return 0;
}


