#ifndef _ARDUINO_COMPAT_H
#define _ARDUINO_COMPAT_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
//#include <sys/types.h>
//#include <sys/stat.h>
#include <fcntl.h>

#include <ctype.h>

#include <string.h>

#include <iostream>
using namespace std;

#define DEC 10
#define HEX 16
#define OCT 8
#ifdef BIN // Prevent warnings if BIN is previously defined in "iotnx4.h" or similar
#undef BIN
#endif
#define BIN 2


typedef bool boolean;
//typedef iostream Stream;

class Stream {
public:

	Stream(istream &inStream, ostream &outStream) :
		in(inStream),
		out(outStream)
	{
		;
	}

	inline bool available(void) {
		return in.peek() != EOF;
	}

	inline int read(void) {
		if (available()) {
			char c;
			in.read(&c, 1);
			return c;
		}
		else
			return -1;
	}

	inline size_t println(void) {
		const char *eol = "\r\n";
		const size_t n = strlen(eol);
		out.write("\r\n", n);
		return n;
	}

	inline size_t print(const char *s) {
		const size_t n = strlen(s);
		out.write(s, n);
		return n;
	}

	inline size_t println(const char *s) {
		return print(s) + println();
	}

	inline size_t print(int n, int base = DEC) {
		return print((long)n, base);
	}

	inline size_t println(int n, int base = DEC) {
		return print((long)n, base) + println();
	}

	inline size_t print(size_t n, int base = DEC) {
		return print((long)n, base);
	}

	inline size_t println(size_t n, int base = DEC) {
		return print((long)n, base) + println();
	}

	inline size_t print(long n, int base = DEC) {
		streampos start = in.tellg();
		if (base == 0) {
			out.write((const char*)&n, sizeof(n));
		}
		else if (base == OCT) {
			out << std::oct << n;
		}
		else if (base == DEC) {
			out << std::dec << n;
		}
		else if (base == HEX) {
			out << std::hex << n;
		}
		else {
			; // Don't supprt other bases (different from Arduino)
		}
		return in.tellg() - start;
	}

	inline size_t println(long n, int base = DEC) {
		return print(n, base) + println();
	}

	inline void flush(void) {
		;
	}

private:
	istream &in;
	ostream &out;

};

//typedef CompatStream Stream;

#endif
