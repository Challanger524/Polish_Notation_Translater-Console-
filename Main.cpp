#include "Header.h"
#include <string>

#define TEST
//#undef TEST //Uncomment for console  input

void(*Terminal)(const char *, unique_ptr<char[]> &, unique_ptr<char[]> &) = Terminal_Single_Thread;

int main()
{
	if (thread::hardware_concurrency() > 1u)Terminal = Terminal_Double_Thread;//slower with threads

#ifdef TEST
#if 1
	vector<string> Test_input =
	{"12!+a", "(a+b!!)*12!", "((a!+b)!*15)^2!", "(((a!!+b)!*15)^2!)!",     //infix
	 "+a12!", "!*!+!a!1 23", "!^!/*!+a!b1!23!4", "!^!*!c+!a!b/!-12!345 6",//prefix
	 "a12+!", "a!1 23-+", "ab+c*3 1 2-/^", "a!b!+!c!*!1!23!456!-!/!^!",  //postfix
	 "123!+", "-+a", "((a+b)-2", "!(a+1)"};//wrong
#else
	vector<string> Test_input = {"!+!!!12!!34"};
#endif

	for (const auto &str : Test_input) {
		unique_ptr<char[]> res1(nullptr);
		unique_ptr<char[]> res2(nullptr);

		Terminal(str.c_str(), res1, res2);
		cout << endl;
	}
#else

	char input[G_SIZER];
	size_t str_siz;

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

		Terminal(input, res1, res2);
	}
	cout << " No input, program terminates: ";
#endif
	
	system("pause");
	return 0;
}
