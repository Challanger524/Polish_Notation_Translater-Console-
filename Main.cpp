#include "Header.h"

#define TEST
#ifdef TEST 
#include <string>
#endif
void(*Terminal)(const char *, char *&, char *&) = Terminal_Single_Thread;


int main()
{
	char input[SIZER];
	char *res1 = nullptr;
	char *res2 = nullptr;
	size_t str_size;

	if (thread::hardware_concurrency() > 1u)Terminal = Terminal_Double_Thread;//slower with threads

#ifdef TEST
#if 1
	vector<string> Test_input =
	{"12+3", "(a+b!!)*12!!!", "((a!!+b)!*15)^2!", "(((a!!+b)!*15)^2!)!", "(a+b)*!12",
	 "+ab", "!!!+!!a!b", "!*!+!a!b!c", "!^!/*!+a!bc!d!e", "!^!*!c+!a!b/!-1!2 3",
	 "ab+!", "abc-+", "ab+c-", "ab+c*3 1 2-/^", "a!b!+!c!*!3!1!2!-!/!^!", "abc+",
	 "a+b!!!", "a!b!!c!-+!!!", "(a+b)!*2*(c+d)!!"};
#else
	vector<string> Test_input = {"12+3"};
#endif

	for (const auto &str : Test_input) {
		Terminal(str.c_str(), res1, res2);
		cout << endl;
		if (res1) delete[] res1;
		if (res2) delete[] res2;
	}
#else

	cout << "Allowed: latters(A-z), digits(0-9), operators(+ - * / ^ % !); 123 456(spacebar).";
	while (true) {
		cout << "\n Write your expression:";


		fflush(stdin);
		fgets(input, SIZER, stdin);

		str_size = strlen(input);
		if (str_size < 3) break;
		if (input[str_size - 1] == '\n') input[--str_size] = '\0';
		Terminal(input, res1, res2);

		if (res1) delete[] res1;
		if (res2) delete[] res2;
	}
#endif

	system("pause");
	return 0;
}
