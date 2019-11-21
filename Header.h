#pragma once
#include <iostream>
#include <stack>
#include <vector>
#include <thread>
constexpr unsigned int SIZER = 128;

using namespace std;

void Terminal_Single_Thread(const char *input, char *&res1, char *&res2);
void Terminal_Double_Thread(const char *input, char *&res1, char *&res2);

int OperChecker(const char Operator);

bool InfixSyntaxCheker(const char *, const size_t);
bool PostfSyntaxCheker(const char *, const size_t);
bool PrefiSyntaxCheker(const char *, const size_t);

void InfToPost(const char *_string, char* &ptr, const size_t _str_size);
void InfToPref(const char *_string, char* &ptr, const size_t _str_size);

void PostToInf(const char *_string, char* &ptr, const size_t _str_size);
void PostToPref(const char *_string, char* &ptr, const size_t _str_size);

void PrefToPost(const char *_string, char* &ptr, const size_t _str_size);
void PrefToInf(const char *_string, char* &ptr, const size_t _str_size);

void PrefToInfMyOwn(const char *_string, char* &ptr, const size_t _str_size);