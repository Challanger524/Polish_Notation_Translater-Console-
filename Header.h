#pragma once
#include <iostream>
#include <string_view>
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

void Terminal_Single_Thread(string_view input, unique_ptr<char[]> &res1, unique_ptr<char[]> &res2);
void Terminal_Double_Thread(string_view input, unique_ptr<char[]> &res1, unique_ptr<char[]> &res2);

int OperChecker(const char Operator);

bool InfixSyntaxCheker(string_view view);
bool PostfSyntaxCheker(string_view view);
bool PrefiSyntaxCheker(string_view view);

void InfToPost(string_view _string, unique_ptr<char[]> &ptr);
void InfToPref(string_view _string, unique_ptr<char[]> &ptr);

void PostToInf(string_view _string, unique_ptr<char[]> &ptr);
void PostToPref(string_view _string, unique_ptr<char[]> &ptr);

void PrefToPost(string_view _string, unique_ptr<char[]> &ptr);
void PrefToInf(string_view _string, unique_ptr<char[]> &ptr);

//void PrefToInfMyOwn(string_view _string, unique_ptr<char[]> &ptr);

bool Check(unique_ptr<char[]> &res);