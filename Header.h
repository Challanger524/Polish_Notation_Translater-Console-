#pragma once
#include <iostream>
#include <string_view>
#include <stack>
#include <vector>
#include <thread>
#include <future>
#include <memory>

using namespace std;

constexpr unsigned int G_SIZER = 128;

struct Timer {
	Timer() { start = std::chrono::steady_clock::now(); }
	Timer(const Timer&) = delete;
	Timer operator = (const Timer&) = delete;
	~Timer() { cout << "\nTimer : " << static_cast<std::chrono::duration<float>>(std::chrono::steady_clock::now() - start).count() * 1000 << "ms\n"; }
	
	operator std::chrono::duration<float>() const { return std::chrono::steady_clock::now() - start; }
	void Lap(){	cout << "\nLap : " << static_cast<std::chrono::duration<float>>(std::chrono::steady_clock::now() - start).count() * 1000 << "ms\n"; }
private:
	std::chrono::time_point<std::chrono::steady_clock> start;
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