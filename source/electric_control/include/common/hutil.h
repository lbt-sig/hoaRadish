// MIT License

// Copyright (c) 2023 LBT_sig

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

// some common func and marcos
#ifndef ELEC_COMMON_HUTIL_H_
#define ELEC_COMMON_HUTIL_H_

// Write C/C++ general-purpose programs.
#ifdef __cplusplus
extern "C" {
#endif

// - str options.

// Compare two str use definite
// if same return true
#define EQ_STR(str1, str2) (*(a) == *(b) && strcmp((str1), (str2)) == 0)


// - log.
#define LOG_PASS(fmt, ...) do{ printf(" [\033[32mpass\033[0m] " fmt "", ##__VA_ARGS__);} while(0)
#define LOG_UNPASS(fmt, ...) do{ printf("[\033[35munpass\033[0m] " fmt "", ##__VA_ARGS__);} while(0)
#define PRINT_LINE_HEAD(fmt) do{ printf("\n------------------ %s -----------------\n", fmt);}while(0)
#define PRINT_LINE_END() do{ printf("---------------------- End ----------------------\n\n");}while(0)

// - check
#define LBT_ASSERT(x) assert(x)
#define CHECK_NULL(x, msg)    if(!(x)) \
                              { printf("[\033[35munpass\033[0m] : %s", msg); return -1;}
#define CHECK_RC(x, param) do{ if(x == 0) LOG_PASS("%s",param); else LOG_UNPASS("%s", param); }while(0)
#define CHECK_NRC(x, param) do{ if(x != 0) LOG_PASS("%s",param); else LOG_UNPASS("%s", param); }while(0)









#ifdef __cplusplus
}
#endif
#endif //! ELEC_COMMON_HUTIL_H_