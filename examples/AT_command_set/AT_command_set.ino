#include <CommandHandler.h>

// Demonstrate an AT command set interface using CommandHandler.

constexpr uint8_t maxCommandLength = 80;

// Commands
void cmdPrintHelp(const char *);
void cmdAT(const char *s);
void cmdGetVersion(const char *s);
void cmdGetName(const char *s);
void cmdSetName(const char *s);

void errorCommand(const char *s);
void commandTooLong(int);

CommandOption commands[] = {
    CommandOption("HELP", cmdPrintHelp),
    CommandOption("?", cmdPrintHelp),
    CommandOption("AT+GMR", cmdGetVersion),
    
    // There are several ways to implement get and set commands. It is possible to
    // use a single function and test the first character after the match. Here we
    // separate the get and set into two functions.
    CommandOption("AT+NAME=", cmdSetName),

    CommandOption("AT+NAME?", cmdGetName),
    // AT+NAME must be listed after the other similar commands so that they are tested first.
    CommandOption("AT+NAME", cmdGetName), 

    // The plain AT command must be tested last since it matches the start of all other AT commands.
    CommandOption("AT", cmdAT),
};
constexpr uint8_t numCommands = sizeof(commands) / sizeof(commands[0]);

CommandHandler serialHandler;
char serialCommandBuffer[maxCommandLength];

// Since the maximum command length cannot exceed maxCommandLength we know that the value for name can fit.
char name[maxCommandLength] = {'\0'};

void setup(void)
{
    Serial.begin(9600);
    serialHandler.begin(serialCommandBuffer, sizeof(serialCommandBuffer), commands, numCommands, errorCommand, commandTooLong);
    
    // Print some help. No command was issued so don't send "OK"
    printHelp();
}

void loop(void)
{
    // Put your other commands here.
    
    // Have the command handler process any incoming data. If a complete command is ready the matching
    // callback function will be called.
    serialHandler.process(Serial);
}


void printHelp(void)
{
    // Print some help information.
    Serial.println("+HELP  COMMAND HELP");
    Serial.println("+HELP  ============");
    Serial.println("+HELP ");
    Serial.println("+HELP  Valid commands are:");
    Serial.println("+HELP  AT                 Print OK (null command)");
    Serial.println("+HELP ");
    Serial.println("+HELP  AT+GMR             Print software version");
    Serial.println("+HELP ");
    Serial.println("+HELP  AT+NAME");
    Serial.println("+HELP  AT+NAME?           Return current value of NAME");
    Serial.println("+HELP ");
    Serial.println("+HELP  AT+NAME=value      Set NAME to given value");
}

void cmdPrintHelp(const char*)
{
    // Ignore characters after the string match, any command that starts with the correct sequence will print the help
    printHelp();
    Serial.println();
    Serial.println("OK");
}

void cmdAT(const char *s)
{
    if (s && *s == '\0') {
        // String is valid, no more characters after the AT part matched
        Serial.println("OK");
    } else {
        Serial.println("ERROR");
    }
}

void cmdGetVersion(const char *s)
{
    if (s && *s == '\0') {
        // String is valid, no more characters after the AT+GMR part matched
        Serial.println("+GMR:" COMMAND_HANDLER_VERSION);
        Serial.println();
        Serial.println("OK");
    } else {
       Serial.println("ERROR");
    }
}


void cmdSetName(const char *s)
{
    strncpy(name, s, sizeof(name)); // Copy everything after the match
    Serial.println("+NAME:"); Serial.println(name);
    Serial.println();
    Serial.println("OK" );
}

void cmdGetName(const char *s)
{
    if (s && *s == '\0') {
        // String is valid, no unwanted extra characters after the match
        Serial.print("+NAME:"); Serial.println(name);
        Serial.println();
        Serial.println("OK");
    } else {
        Serial.println("ERROR");
    }   
}

void errorCommand(const char *)
{
    Serial.println("ERROR");
}

void commandTooLong(int)
{
    Serial.println("ERROR");
}
