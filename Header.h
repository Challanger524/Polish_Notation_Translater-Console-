#pragma once
#include <iostream>
#include <stack>
#include <vector>
#include <thread>
#include <memory>

using namespace std;

constexpr unsigned int G_SIZER = 128;

struct Timer {
	std::chrono::time_point<std::chrono::steady_clock> start;
	std::chrono::duration<float> elapse;

	Timer() { start = std::chrono::steady_clock::now(); }
	operator std::chrono::duration<float>() const { return std::chrono::steady_clock::now() - start; }
	~Timer() {
		elapse = std::chrono::steady_clock::now() - start;
		cout << "\nTimer : " << elapse.count() * 1000 << "ms\n";
	}
	Timer(const Timer&) = delete;
	Timer operator = (const Timer&) = delete;
};

void Terminal_Single_Thread(const char *input, unique_ptr<char[]> &res1, unique_ptr<char[]> &res2);
void Terminal_Double_Thread(const char *input, unique_ptr<char[]> &res1, unique_ptr<char[]> &res2);

int OperChecker(const char Operator);

bool InfixSyntaxCheker(const char *, const size_t);
bool PostfSyntaxCheker(const char *, const size_t);
bool PrefiSyntaxCheker(const char *, const size_t);

void InfToPost(const char *_string, unique_ptr<char[]> &ptr, const size_t _str_size);
void InfToPref(const char *_string, unique_ptr<char[]> &ptr, const size_t _str_size);

void PostToInf(const char *_string, unique_ptr<char[]> &ptr, const size_t _str_size);
void PostToPref(const char *_string, unique_ptr<char[]> &ptr, const size_t _str_size);

void PrefToPost(const char *_string, unique_ptr<char[]> &ptr, const size_t _str_size);
void PrefToInf(const char *_string, unique_ptr<char[]> &ptr, const size_t _str_size);

//void PrefToInfMyOwn(const char *_string, unique_ptr<char[]> &ptr, const size_t _str_size);