//

#include <gtest/gtest.h>

extern "C" {
#include "source.h"
#include "thunder/thunder.h"
}

TEST(SourceIterator, SourceIteratorNext) {
    // the source code being parsed
    const char * source =
        "# this is a comment\n"
        "fn do_something():\n"
        "\tpass\n"
        "fn main():\n"
        "\tlet a = 0\n"
        "\tif (a == 0):\n"
        "\t\tdo_something()";

    struct Expected {
        // the most recent token
        int scanned;
        // current indentation level
        int indent;
        // the parsed value if the token is a literal
        Th_Value value;
    };

    const Expected expected[] = {
        { _THUNDER_FN,                 0, 0.0 },
        { _THUNDER_NAME,               0, 0.0 },
        { _THUNDER_PARENTHESIS_OPEN,   0, 0.0 },
        { _THUNDER_PARENTHESIS_CLOSE,  0, 0.0 },
        { _THUNDER_COLON,              0, 0.0 },

        { _THUNDER_INDENT,             1, 0.0 },
        { _THUNDER_PASS,               1, 0.0 },
        { _THUNDER_DEDENT,             0, 0.0 },

        { _THUNDER_FN,                 0, 0.0 },
        { _THUNDER_NAME,               0, 0.0 },
        { _THUNDER_PARENTHESIS_OPEN,   0, 0.0 },
        { _THUNDER_PARENTHESIS_CLOSE,  0, 0.0 },
        { _THUNDER_COLON,              0, 0.0 },

        { _THUNDER_INDENT,             1, 0.0 },
        { _THUNDER_LET,                1, 0.0 },
        { _THUNDER_NAME,               1, 0.0 },
        { _THUNDER_EQUAL,              1, 0.0 },
        { _THUNDER_NUMBER,             1, 0.0 },

        { _THUNDER_IF,                 1, 0.0 },
        { _THUNDER_PARENTHESIS_OPEN,   1, 0.0 },
        { _THUNDER_NAME,               1, 0.0 },
        { _THUNDER_EQUAL_EQUAL,        1, 0.0 },
        { _THUNDER_NUMBER,             1, 0.0 },
        { _THUNDER_PARENTHESIS_CLOSE,  1, 0.0 },
        { _THUNDER_COLON,              1, 0.0 },

        { _THUNDER_INDENT,             2, 0.0 },
        { _THUNDER_NAME,               2, 0.0 },
        { _THUNDER_PARENTHESIS_OPEN,   2, 0.0 },
        { _THUNDER_PARENTHESIS_CLOSE,  2, 0.0 },

        { _THUNDER_DEDENT,             1, 0.0 },
        { _THUNDER_DEDENT,             0, 0.0 },

        { _THUNDER_EOS,                0, 0.0 }
    };

    Th_SourceIterator source_iterator = { source, 0, _THUNDER_ERROR, 0.0 };

    for (int index = 0; source_iterator.scanned != _THUNDER_EOS; index++) {
        Th_SourceIteratorNext(&source_iterator);

        // check the token type
        EXPECT_EQ(source_iterator.scanned, expected[index].scanned);
        // check the indentation level
        EXPECT_EQ(source_iterator.indent, expected[index].indent);
        // check the value
        EXPECT_EQ(source_iterator.value, expected[index].value);
    }
}
