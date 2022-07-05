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

#ifndef __THUNDER_H
#define __THUNDER_H

#include <stdint.h>
#include <string.h>

typedef uint64_t Th_Value;

#define THUNDER_NAN ((Th_Value)0x7FFC000000000000)

#define THUNDER_MASK_NONE   ((Th_Value)0x0)

#define THUNDER_MASK_TRUE   ((Th_Value)0x1)
#define THUNDER_MASK_FALSE  ((Th_Value)0x2)

#define THUNDER_MASK        ((Th_Value)0x7)

#define THUNDER_NONE    ((Th_Value)(THUNDER_NAN | THUNDER_MASK_NONE))

#define THUNDER_TRUE    ((Th_Value)(THUNDER_NAN | THUNDER_MASK_TRUE))
#define THUNDER_FALSE   ((Th_Value)(THUNDER_NAN | THUNDER_MASK_FALSE))

inline Th_Value Th_MakeNumber(double param) {

    Th_Value outcome = { 0 };
    memcpy(&outcome, &param, sizeof(outcome));
    return outcome;
}

inline double Th_CastNumber(Th_Value param) {

    double outcome = { 0 };
    memcpy(&outcome, &param, sizeof(outcome));
    return outcome;
}

#endif // __THUNDER_H
