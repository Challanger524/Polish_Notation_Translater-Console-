#include "stdafx.h"
#include "CppUnitTest.h"

#include "../Polish_Notation_Translater(Console)/Header.h"
#include "../Polish_Notation_Translater(Console)/Functions.cpp"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Polish_Notation_Translater_Console_UnitTest
{
	TEST_CLASS(FAIL_CLass)
	{
	public://Fail trigerring		
		TEST_METHOD(Spacebar_is_not_an_Oper) { Assert::IsTrue(OperChecker(' '), L"Is Spacebar an operator???"); }
	};

	TEST_CLASS(OperChecker_F_Check)
	{
	public:

		TEST_METHOD(Oper_factorial) { Assert::IsTrue(OperChecker('!') == -1, L"Factorial's '!'(fact) priority must be -1 or negative"); }

		TEST_METHOD(Oper_plus) { Assert::AreEqual(1, OperChecker('+'), L"Operator's '+'(plus) priority must be 1"); }

		TEST_METHOD(Oper_minus) { Assert::AreEqual(1, OperChecker('-'), L"Operator's '-'(minus) priority must be 1"); }

		TEST_METHOD(Oper_mult) { Assert::AreEqual(2, OperChecker('*'), L"Operator's '*'(mult) priority must be 2"); }

		TEST_METHOD(Oper_div) { Assert::AreEqual(2, OperChecker('/'), L"Operator's '/'(div) priority must be 2"); }

		TEST_METHOD(Oper_mod) { Assert::AreEqual(2, OperChecker('%'), L"Operator's '%'(mod) priority must be 2"); }

		TEST_METHOD(Oper_pow) { Assert::AreEqual(3, OperChecker('^'), L"Operator's '^'(pow) priority must be 3"); }

		TEST_METHOD(Not_Oper) { Assert::IsFalse(OperChecker('='), L"Assign '=' is not an operator"); }

		TEST_METHOD(Yes_Oper) { Assert::IsTrue(OperChecker('*'), L"Operator '*'(mult) is an operator"); }

	};

	TEST_CLASS(SpaceRemover_Check)
	{
	public:

		TEST_METHOD(Massive_Ckeck) {
			char input[] = "  +          123                 456";
			const char expect[] = "+123 456";

			SpaceRemover(input);
			Assert::AreEqual(expect, input, L"Space remove incorrect");
		}

		TEST_METHOD(String_Ckeck) {
			string input = "  +        123                 456";
			const char expect[] = "+123 456";

			SpaceRemover(input);
			Assert::AreEqual(expect, input.c_str(), L"Space remove incorrect");
		}
	};

	TEST_CLASS(SyntaxCheker_Fs_Check)
	{
	public:

		TEST_METHOD(InfixSyntaxCheker_Check1) { Assert::IsTrue(InfixSyntaxCheker("(a+b!!)*12!"), L"Must be correct"); }
		TEST_METHOD(InfixSyntaxCheker_Check2) { Assert::IsFalse(InfixSyntaxCheker("(ab!!)*12!"), L"Must not be correct"); }

		TEST_METHOD(PostfSyntaxCheker_Check1) { Assert::IsTrue(PostfSyntaxCheker("a!b!+ !c!* !1!23!456!- !/ !^ !"), L"Must be correct"); }
		TEST_METHOD(PostfSyntaxCheker_Check2) { Assert::IsFalse(PostfSyntaxCheker("1a!b!+ !c!* !1!23!456!- !/ !^ !"), L"Must not be correct"); }

		TEST_METHOD(PrefiSyntaxCheker_Check1) { Assert::IsTrue(PrefiSyntaxCheker("!^!*!c+!a!b/!-12!345 6"), L"Must be correct"); }
		TEST_METHOD(PrefiSyntaxCheker_Check2) { Assert::IsFalse(PrefiSyntaxCheker("- !^!*!c+!a!b/!-12!345 6"), L"Must not be correct"); }
	};

	TEST_CLASS(Translators_Check)
	{
		char infix[36] = " ( ( (a! !+ b) !* 15)^2 !) !";
		char postfix[36] = " a ! b !+ !c!* !1!23 456!- !/ !^ ! ";
		char prefix[36] = " ! ^!*!c + !a !b /! -12!345 6";

	public:
		TEST_METHOD(InfToPost_Check) {
			
			const char expect[] = "a!!b+!15*2!^!";
			unique_ptr<char[]> result(nullptr);

			InfToPost(infix, result);
			Assert::AreEqual(expect, result.get(), L"Translation incorrect");
		}

		TEST_METHOD(InfToPref_Check) {
			const char expect[] = "!^*!+!!ab15!2";
			unique_ptr<char[]> result(nullptr);

			//SpaceRemover(infix);//!!!!!!!!!!!
			InfToPref(infix, result);
			Assert::AreEqual(expect, result.get(), L"Translation incorrect");
		}

		TEST_METHOD(PostToInf_Check) {
			const char expect[] = "(((a!+b!)!*c!)!^(1!/(23-456!)!)!)!";
			unique_ptr<char[]> result(nullptr);

			PostToInf(postfix, result);
			Assert::AreEqual(expect, result.get(), L"Translation incorrect");
		}

		TEST_METHOD(PostToPref_Check) {
			const char expect[] = "!^!*!+!a!b!c!/!1!-23!456";
			unique_ptr<char[]> result(nullptr);

			PostToPref(postfix, result);
			Assert::AreEqual(expect, result.get(), L"Translation incorrect");
		}

		TEST_METHOD(PrefToInf_Check) {
			const char expect[] = "((c!*(a!+b!))!^((12-345!)!/6))!";
			unique_ptr<char[]> result(nullptr);

			PrefToInf(prefix, result);
			Assert::AreEqual(expect, result.get(), L"Translation incorrect");
		}

		TEST_METHOD(PrefToPost_Check) {
			const char expect[] = "c!a!b!+*!12 345!-!6/^!";
			unique_ptr<char[]> result(nullptr);

			PrefToPost(prefix, result);
			Assert::AreEqual(expect, result.get(), L"Translation incorrect");
		}
		
	};
}

#if 0

namespace Quick_UnitTest {
	TEST_CLASS(Quick_Test)
	{
	public:
		TEST_METHOD(Quick_Check) {
			Assert::AreEqual(1, int(strlen("1")), L"Translation incorrect");
		}
	};
}

#endif