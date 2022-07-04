// MIT License
//
// Copyright (c) 2022 oedot
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef __THUNDER_SOURCE_H
#define __THUNDER_SOURCE_H

#include "thunder/thunder.h"

enum {
    _THUNDER_INDENT = 0,
    _THUNDER_DEDENT,

    _THUNDER_BRACE_OPEN,
    _THUNDER_BRACE_CLOSE,

    _THUNDER_BRACKET_OPEN,
    _THUNDER_BRACKET_CLOSE,

    _THUNDER_PARENTHESIS_OPEN,
    _THUNDER_PARENTHESIS_CLOSE,

    _THUNDER_AMPERSAND,
    _THUNDER_AMPERSAND_AMPERSAND,

    _THUNDER_PIPE,
    _THUNDER_PIPE_PIPE,

    _THUNDER_BANG,
    _THUNDER_BANG_EQUAL,

    _THUNDER_PLUS,
    _THUNDER_MINUS,
    _THUNDER_STAR,
    _THUNDER_SLASH,

    _THUNDER_DOT,
    _THUNDER_COMMA,
    _THUNDER_COLON,

    _THUNDER_LESS,
    _THUNDER_LESS_EQUAL,

    _THUNDER_GREATER,
    _THUNDER_GREATER_EQUAL,

    _THUNDER_EQUAL,
    _THUNDER_EQUAL_EQUAL,

    _THUNDER_BREAK,
    _THUNDER_CONTINUE,
    _THUNDER_ELSE,
    _THUNDER_FN,
    _THUNDER_FOR,
    _THUNDER_IF,
    _THUNDER_LET,
    _THUNDER_PASS,
    _THUNDER_RETURN,
    _THUNDER_WHILE,

    _THUNDER_NAME,
    _THUNDER_NUMBER,

    _THUNDER_ERROR,
    _THUNDER_EOS
};

struct Th_SourceIterator {
    // the source code being parsed
    const char * source;
    // current indentation level
    int indent;
    // the most recent token
    int scanned;
    // the parsed value if the token is a literal
    Th_Value value;
};

// read the next token and store it in 'source_iterator'
int Th_SourceIteratorNext(struct Th_SourceIterator * source_iterator);

#endif // __THUNDER_SOURCE_H
