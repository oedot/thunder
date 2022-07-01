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

#include <stdlib.h>

#include "source.h"

int _source_iterator_number(struct _source_iterator_t * source_iterator) {

    return (*source_iterator->source >= '0' && *source_iterator->source <= '9');
}

int _source_iterator_next(struct _source_iterator_t * source_iterator) {

    for (;;) {

        switch (*source_iterator->source) {

            case '\0': {

                if (source_iterator->indent > 0)
                    return (source_iterator->indent--, source_iterator->scanned = _THUNDER_DEDENT);

                return (source_iterator->source++, source_iterator->scanned = _THUNDER_EOS);
            }

            case '\n': {

                source_iterator->source++;

                const char * source = source_iterator->source;
                while (*source_iterator->source == '\t')
                    source_iterator->source++;

                const int indent = source_iterator->source - source;

                if (indent < source_iterator->indent)
                    return (source_iterator->indent = indent, source_iterator->scanned = _THUNDER_DEDENT);

                if (indent > source_iterator->indent)
                    return (source_iterator->indent = indent, source_iterator->scanned = _THUNDER_INDENT);

                continue;
            }

            case '{':
                return (source_iterator->source++, source_iterator->scanned = _THUNDER_BRACE_OPEN);
            case '}':
                return (source_iterator->source++, source_iterator->scanned = _THUNDER_BRACE_CLOSE);

            case '[':
                return (source_iterator->source++, source_iterator->scanned = _THUNDER_BRACKET_OPEN);
            case ']':
                return (source_iterator->source++, source_iterator->scanned = _THUNDER_BRACKET_CLOSE);

            case '(':
                return (source_iterator->source++, source_iterator->scanned = _THUNDER_PARENTHESIS_OPEN);
            case ')':
                return (source_iterator->source++, source_iterator->scanned = _THUNDER_PARENTHESIS_CLOSE);

            case '&': {

                source_iterator->source++;

                switch (*source_iterator->source) {

                    case '&':
                        return (source_iterator->source++, source_iterator->scanned = _THUNDER_AMPERSAND_AMPERSAND);

                    default:
                        return (source_iterator->scanned = _THUNDER_AMPERSAND);
                }
            }

            case '|': {

                source_iterator->source++;

                switch (*source_iterator->source) {

                    case '|':
                        return (source_iterator->source++, source_iterator->scanned = _THUNDER_PIPE_PIPE);

                    default:
                        return (source_iterator->scanned = _THUNDER_PIPE);
                }
            }

            case '!': {

                source_iterator->source++;

                switch (*source_iterator->source) {

                    case '=':
                        return (source_iterator->source++, source_iterator->scanned = _THUNDER_BANG_EQUAL);

                    default:
                        return (source_iterator->scanned = _THUNDER_BANG);
                }
            }

            case '+':
                return (source_iterator->source++, source_iterator->scanned = _THUNDER_PLUS);
            case '-':
                return (source_iterator->source++, source_iterator->scanned = _THUNDER_MINUS);
            case '*':
                return (source_iterator->source++, source_iterator->scanned = _THUNDER_STAR);
            case '/':
                return (source_iterator->source++, source_iterator->scanned = _THUNDER_SLASH);

            case '.':
                return (source_iterator->source++, source_iterator->scanned = _THUNDER_DOT);
            case ',':
                return (source_iterator->source++, source_iterator->scanned = _THUNDER_COMMA);
            case ':':
                return (source_iterator->source++, source_iterator->scanned = _THUNDER_COLON);

            case '<': {

                source_iterator->source++;

                switch (*source_iterator->source) {

                    case '=':
                        return (source_iterator->source++, source_iterator->scanned = _THUNDER_LESS_EQUAL);

                    default:
                        return (source_iterator->scanned = _THUNDER_LESS);
                }
            }

            case '>': {

                source_iterator->source++;

                switch (*source_iterator->source) {

                    case '=':
                        return (source_iterator->source++, source_iterator->scanned = _THUNDER_GREATER_EQUAL);

                    default:
                        return (source_iterator->scanned = _THUNDER_GREATER);
                }
            }

            case '=': {

                source_iterator->source++;

                switch (*source_iterator->source) {

                    case '=':
                        return (source_iterator->source++, source_iterator->scanned = _THUNDER_EQUAL_EQUAL);

                    default:
                        return (source_iterator->scanned = _THUNDER_EQUAL);
                }
            }

            case ' ':
            case '\r':
            case '\t': {

                source_iterator->source++;
                continue;
            }

            case '#': {

                source_iterator->source++;

                while (*source_iterator->source != '\n' && *source_iterator->source != '\0')
                    source_iterator->source++;

                continue;
            }

            default: {

                if (_source_iterator_number(source_iterator)) {

                    const char * source = source_iterator->source++;

                    while (_source_iterator_number(source_iterator))
                        source_iterator->source++;

                    if (*source_iterator->source == '.') {
                        source_iterator->source++;

                        while (_source_iterator_number(source_iterator))
                            source_iterator->source++;
                    }

                    return (source_iterator->value = strtod(source, NULL), source_iterator->scanned = _THUNDER_NUMBER);
                }

                return (source_iterator->source++, source_iterator->scanned = _THUNDER_ERROR);
            }
        }
    }
}
