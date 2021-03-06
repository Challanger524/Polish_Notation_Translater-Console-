#include "Header.h"
//#include <string>
#include <regex>

#define TEST
#undef TEST //Comment for partitial test

function<void(string_view, unique_ptr<char[]> &, unique_ptr<char[]> &)> Terminal;

int main()
{
	Timer t;
	char input[G_SIZER];
	size_t str_siz;
	const regex rule(R"([^\s\(\)A-Za-z1-9\d*+\-\*/\^%!])");//if specific (not defined) character detected('^' in front == not).
	cmatch odd;

	if (thread::hardware_concurrency() > 1u)Terminal = Terminal_Double_Thread;//slower with threads
	else Terminal = Terminal_Single_Thread;
	cout.precision(4);

#ifndef TEST

	vector<string> Test_input =
	{" 12 ! + a ", "(a+b!!)*12!", "((a!+b)!*15)^2!", "(((a!!+b)!*15)^2!)!",      //infix
	 " ! +! a! 12", "!*!+!a!1 23", "!^!/*!+a!b1!23!4", " ! ^!*!c + !a !b /! -12!345 6",//prefix
	 " a !12 !+ !", "a!1 23-+", "a!b+c*3!1 2-!/^", "a!b!+!c!*!1!23 456!-!/!^!",//postfix
	 "123!+", "-+a", "((a+b)-2", "!&(a+1)", "?#&$"};                          //wrong
#else //partitial test

	vector<string> Test_input = {" !+! a! 12"/*, "!*!+!a!1 23"*/};
#endif

	cout << "------------------------------ test examples -----------------------------------";
	for (auto &str : Test_input)
	{
		unique_ptr<char[]> res1(nullptr);
		unique_ptr<char[]> res2(nullptr);
		smatch sodd;

		if (regex_search(str, sodd, rule)) {
			cout << "[Alarm]: Odd character '" << sodd.str() << "' at position: " << sodd.position() << " detected.\n";
			cout << " Input : [" << str << ']' <<endl;
			continue;
		}

		//SpaceRemover(str);
		Terminal(str, res1, res2);

		//if (!res1 && !res2) cout << "Wrong input\n";
		if (res1 && res2) if (!Check(res1) && !Check(res2)) {
			cout << "Wrong translation\n";
			system("pause");
		}
		cout << endl;
	}
	cout << "-------------------------- test examples end -----------------------------------\n";


	cout << "Allowed: latters(A-z), digits(0-9), operators(+ - * / ^ % !); 123 456(spacebar).";
	while (true) {
		cout << "\n Write your expression:";
		unique_ptr<char[]> res1(nullptr);
		unique_ptr<char[]> res2(nullptr);

		fflush(stdin);
		fgets(input, G_SIZER, stdin);

		str_siz = strlen(input);

		if (str_siz < 2) break;
		if (input[str_siz - 1] == '\n') input[--str_siz] = '\0';
		
		SpaceRemover(input);
		
		if (regex_search(input, odd, rule)) {
			cout << "[Alarm]: Odd character '" << odd.str() << "' at position: " << odd.position() << " detected.\n";
			continue;
		}
		
		Terminal(input, res1, res2);
	}

	cout.precision(2);
	cout << " No input, program closes (operating time: " << static_cast<std::chrono::duration<float>>(t.get()).count() << "sec).\n";
	
	system("pause");
	return 0;
}
