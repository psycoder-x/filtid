/*
MIT License

Copyright (c) 2023 psycoder-x

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <stdio.h>
#include <ctype.h>
#include <string.h>

typedef struct State {
  int ch;
} State;

int ispath(int ch);
int isnumber(int ch);
int isident(int ch);
int token(State *st);
int ident(State *st);
int number(State *st);
int may_path(State *st);
int quoted(State *st, int endch, int onemore);
int may_comment(State *st);
int c_comment(State *st);
int cpp_comment(State *st);

int main() {
  State st = {0};
  st.ch = getchar();
  while (token(&st))
    ;
  return 0;
}

int token(State *st) {
  if (st->ch == EOF)
    return 0;
  if (st->ch == '\"')
    return quoted(st, '\"', 1);
  if (st->ch == '\'')
    return quoted(st, '\'', 1);
  if (st->ch == '<')
    return may_path(st);
  if (st->ch == '/')
    return may_comment(st);
  if (isdigit(st->ch))
    return number(st);
  if (isalnum(st->ch) || st->ch == '_')
    return ident(st);
  st->ch = getchar();
  return 1;
}

int may_path(State *st) {
  st->ch = getchar();
  if (st->ch == EOF)
    return 0;
  if (ispath(st->ch))
    return quoted(st, '>', 0);
  return 1;
}

int ispath(int ch) {
  return isalnum(ch) || strchr("_./\\", ch);
}

int quoted(State *st, int endch, int onemore) {
  if (onemore)
    st->ch = getchar();
  int esc = 0;
  for (;;) {
    if (st->ch == EOF)
      return 0;
    if (!esc) {
      if (st->ch == '\\')
        esc = 1;
      if (st->ch == endch)
        break;
    }
    else {
      esc = 0;
    }
    st->ch = getchar();
  }
  st->ch = getchar();
  return 1;
}

int may_comment(State *st) {
  st->ch = getchar();
  if (st->ch == EOF)
    return 0;
  if (st->ch == '*')
    return c_comment(st);
  if (st->ch == '/')
    return cpp_comment(st);
  return 1;
}

int number(State *st) {
  for (;;) {
    st->ch = getchar();
    if (st->ch == EOF)
      return 0;
    if (!isnumber(st->ch))
      break;
  }
  return 1;
}

int isnumber(int ch) {
  return isalnum(ch) || strchr("_.\'", ch);
}

int c_comment(State *st) {
  int before = 0;
  for (;;) {
    st->ch = getchar();
    if (st->ch == EOF)
      return 0;
    if (before == '*' && st->ch == '/')
      break;
    before = st->ch;
  }
  st->ch = getchar();
  return 1;
}

int cpp_comment(State *st) {
  for (;;) {
    st->ch = getchar();
    if (st->ch == EOF)
      return 0;
    if (st->ch == '\n')
      break;
  }
  st->ch = getchar();
  return 1;
}

int ident(State *st) {
  putchar(st->ch);
  for (;;) {
    st->ch = getchar();
    if (st->ch == EOF) {
      putchar('\n');
      return 0;
    }
    if (!isident(st->ch))
      break;
    putchar(st->ch);
  }
  putchar('\n');
  return 1;
}

int isident(int ch) {
  return isalnum(ch) || ch == '_';
}
