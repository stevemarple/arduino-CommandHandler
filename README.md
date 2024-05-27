# ComandHandler

CommandHandler is a library for Arduino and other Wiring-type
environments to implement command parsing from serial data streams
where characters arrive separately. Multiple serial data streams
can be processed simultaneously by assigning a separate CommandHandler object
for each serial stream. To minimise memory usage the command handlers can be
shared between serial data streams.

The library does not use any dynamic memory allocation. The caller
is required to pass in the buffer required, thereby allowing full
control of the memory used.

## License

The CommandHandler library is licensed under the GNU Lesser General Public
License. See LICENSE.txt for details.

## Version 2.0 changes

### Partial command matching now possible

`CommandOption` now takes an optional additional parameter, `partialMatch`.
Previously all commands tested by CommandHandler were partial matches. This
meant short commands such as `AT` were required to be placed after longer
commands in the `CommandOptions` array, otherwise it would match the longer
commands. By setting `partialMatch` to `false` CommandHandler can now be
instructed to call the handler only on a full match for a given command.

### Incompatible handler changes

Version 2.0 changes the function parameters and return type of the command
handlers. The change was introduced so that callback handlers
can be used with multiple serial `Stream` objects simultaneously.

The handler callback is now given the command string (after any full or
partial match has been eliminated), a reference to the serial `Stream`
object from which the command was read (enabling output and errors to be
printed to the approriate destination), and also a reference to the
`CommandOption` object being processed. The handler is now expected to
return a boolean value, with `true` interpreted as meaning the command was
successfully identified, and `false` meaning the command was not identified.
In the case of commands which were marked as a partial match the `false`
return value from the handler signifies that the handler(s) for other
commands with matching command strings should also be called.
