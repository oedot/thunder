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

#include "parser.h"
#include "source.h"

struct Th_Parser {
    //
    struct Th_SourceIterator source_iterator[2];
};

int Th_ParserAdvance(struct Th_Parser * parser) {
    // not the best, since we're scanning the same thing twice
    Th_SourceIteratorNext(&parser->source_iterator[0]);
    Th_SourceIteratorNext(&parser->source_iterator[1]);

    if (parser->source_iterator[0].scanned == _THUNDER_ERROR)
        return 0;

    return 1;
}

int Th_ParserConsume(struct Th_Parser * parser, uint32_t scanned) {

    if (parser->source_iterator[0].scanned == scanned) {

        Th_ParserAdvance(parser);
        return 1;
    }

    return 0;
}

enum Th_Precedence {
    _PRECEDENCE_NONE = 0,
    _PRECEDENCE_ASSIGNMENT, // =
    _PRECEDENCE_OR,         // ||
    _PRECEDENCE_AND,        // &&
    _PRECEDENCE_EQUALITY,   // == !=
    _PRECEDENCE_COMPARISON, // < > <= >=
    _PRECEDENCE_TERM,       // + -
    _PREDEDENCE_FACTOR,     // * /
    _PRECEDENCE_UNARY,      // ! -
    _PRECEDENCE_CALL,       // . ()
    _PRECEDENCE_PRIMARY
};

typedef void (*Th_ParseFn)(struct Th_Parser *);

struct Th_ParseRule {
    enum Th_Precedence precedence;
    Th_ParseFn prefix;
    Th_ParseFn infix;
};

void Th_ParseExpression(struct Th_Parser * parser);
void Th_ParseNumber(struct Th_Parser * parser);
void Th_ParseParenthesis(struct Th_Parser * parser);
void Th_ParseUnary(struct Th_Parser * parser);
void Th_ParseBinary(struct Th_Parser * parser);

const struct Th_ParseRule parse_rule[] = {
    { _PRECEDENCE_NONE,   NULL,                NULL           }, // _THUNDER_INDENT
    { _PRECEDENCE_NONE,   NULL,                NULL           }, // _THUNDER_DEDENT

    { _PRECEDENCE_NONE,   NULL,                NULL           }, // _THUNDER_BRACE_OPEN
    { _PRECEDENCE_NONE,   NULL,                NULL           }, // _THUNDER_BRACE_CLOSE

    { _PRECEDENCE_NONE,   NULL,                NULL           }, // _THUNDER_BRACKET_OPEN
    { _PRECEDENCE_NONE,   NULL,                NULL           }, // _THUNDER_BRACKET_CLOSE

    { _PRECEDENCE_NONE,   Th_ParseParenthesis, NULL           }, // _THUNDER_PARENTHESIS_OPEN
    { _PRECEDENCE_NONE,   NULL,                NULL           }, // _THUNDER_PARENTHESIS_CLOSE

    { _PRECEDENCE_NONE,   NULL,                NULL           }, // _THUNDER_AMPERSAND
    { _PRECEDENCE_NONE,   NULL,                NULL           }, // _THUNDER_AMPERSAND_AMPERSAND

    { _PRECEDENCE_NONE,   NULL,                NULL           }, // _THUNDER_PIPE
    { _PRECEDENCE_NONE,   NULL,                NULL           }, // _THUNDER_PIPE_PIPE

    { _PRECEDENCE_NONE,   NULL,                NULL           }, // _THUNDER_BANG
    { _PRECEDENCE_NONE,   NULL,                NULL           }, // _THUNDER_BANG_EQUAL

    { _PRECEDENCE_TERM,   NULL,                Th_ParseBinary }, // _THUNDER_PLUS
    { _PRECEDENCE_TERM,   Th_ParseUnary,       Th_ParseBinary }, // _THUNDER_MINUS
    { _PREDEDENCE_FACTOR, NULL,                Th_ParseBinary }, // _THUNDER_STAR
    { _PREDEDENCE_FACTOR, NULL,                Th_ParseBinary }, // _THUNDER_SLASH

    { _PRECEDENCE_NONE,   NULL,                NULL           }, // _THUNDER_DOT
    { _PRECEDENCE_NONE,   NULL,                NULL           }, // _THUNDER_COMMA
    { _PRECEDENCE_NONE,   NULL,                NULL           }, // _THUNDER_COLON

    { _PRECEDENCE_NONE,   NULL,                NULL           }, // _THUNDER_LESS
    { _PRECEDENCE_NONE,   NULL,                NULL           }, // _THUNDER_LESS_EQUAL

    { _PRECEDENCE_NONE,   NULL,                NULL           }, // _THUNDER_GREATER
    { _PRECEDENCE_NONE,   NULL,                NULL           }, // _THUNDER_GREATER_EQUAL

    { _PRECEDENCE_NONE,   NULL,                NULL           }, // _THUNDER_EQUAL
    { _PRECEDENCE_NONE,   NULL,                NULL           }, // _THUNDER_EQUAL_EQUAL

    { _PRECEDENCE_NONE,   NULL,                NULL           }, // _THUNDER_BREAK
    { _PRECEDENCE_NONE,   NULL,                NULL           }, // _THUNDER_CONTINUE
    { _PRECEDENCE_NONE,   NULL,                NULL           }, // _THUNDER_ELSE
    { _PRECEDENCE_NONE,   NULL,                NULL           }, // _THUNDER_FN
    { _PRECEDENCE_NONE,   NULL,                NULL           }, // _THUNDER_FOR
    { _PRECEDENCE_NONE,   NULL,                NULL           }, // _THUNDER_IF
    { _PRECEDENCE_NONE,   NULL,                NULL           }, // _THUNDER_LET
    { _PRECEDENCE_NONE,   NULL,                NULL           }, // _THUNDER_PASS
    { _PRECEDENCE_NONE,   NULL,                NULL           }, // _THUNDER_RETURN
    { _PRECEDENCE_NONE,   NULL,                NULL           }, // _THUNDER_WHILE

    { _PRECEDENCE_NONE,   NULL,                NULL           }, // _THUNDER_NAME
    { _PRECEDENCE_NONE,   Th_ParseNumber,      NULL           }, // _THUNDER_NUMBER

    { _PRECEDENCE_NONE,   NULL,                NULL           }, // _THUNDER_ERROR
    { _PRECEDENCE_NONE,   NULL,                NULL           }, // _THUNDER_EOS
};

void Th_ParsePrecedence(struct Th_Parser * parser, uint32_t precedence) {

    Th_ParserAdvance(parser);

    const Th_ParseFn prefix = parse_rule[parser->source_iterator[1].scanned].prefix;
    if (!prefix) {
        // error: expected expression
        return;
    }

    prefix(parser);

    while (precedence <= parse_rule[parser->source_iterator[0].scanned].precedence) {

        Th_ParserAdvance(parser);

        const Th_ParseFn infix = parse_rule[parser->source_iterator[1].scanned].infix;
        infix(parser);
    }
}

void Th_ParseExpression(struct Th_Parser * parser) {

    Th_ParsePrecedence(parser, _PRECEDENCE_ASSIGNMENT);
}

void Th_ParseNumber(struct Th_Parser * parser) {

}

void Th_ParseParenthesis(struct Th_Parser * parser) {

    Th_ParseExpression(parser);
    Th_ParserConsume(parser, _THUNDER_PARENTHESIS_CLOSE);
}

void Th_ParseUnary(struct Th_Parser * parser) {

    const uint32_t operator = parser->source_iterator[1].scanned;

    // compile the operand
    Th_ParsePrecedence(parser, _PRECEDENCE_UNARY);

    // emit the operator instruction
    switch (operator) {

        case _THUNDER_MINUS:
            break;

        case _THUNDER_BANG:
            break;
    }
}

void Th_ParseBinary(struct Th_Parser * parser) {

    const uint32_t operator = parser->source_iterator[1].scanned;

    // compile the operand
    Th_ParsePrecedence(parser, parse_rule[operator].precedence + 1);

    // emit the operator instruction
    switch (operator) {

        case _THUNDER_PLUS:
            break;

        case _THUNDER_MINUS:
            break;

        case _THUNDER_STAR:
            break;

        case _THUNDER_SLASH:
            break;
    }
}

int Th_Parse(const char * source) {

    struct Th_Parser parser = { 0 };

    parser.source_iterator[0] = (struct Th_SourceIterator) {
        .source = source, .indent = 0, .scanned = _THUNDER_ERROR
    };

    parser.source_iterator[1] = (struct Th_SourceIterator) {
        .source = source, .indent = 0, .scanned = _THUNDER_ERROR
    };

    Th_SourceIteratorNext(&parser.source_iterator[0]);

    Th_ParseExpression(&parser);

    return Th_ParserConsume(&parser, _THUNDER_EOS);
}
