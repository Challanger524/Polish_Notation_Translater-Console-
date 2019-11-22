#include "Header.h"

//"Get operator's priority"
int OperChecker(const char c)
{
	if (c == '+' || c == '-') return 1;
	else if (c == '*' || c == '/' || c == '%') return 2;
	else if (c == '^') return 3;
	else if (c == '!') return-1; //for unar operators
	else				    return 0;
}

//Processing the input:
void Terminal_Single_Thread(const char *input, unique_ptr<char[]> &res1, unique_ptr<char[]> &res2)
{
	Timer t;
	bool correct = false;
	size_t str_size = strlen(input);

	if (str_size < 3) return;

	cout << "Input :[" << input << ']';

	//Define weather expression is Prefix or Postdix or Infix.
	if (OperChecker(input[0])) {//if Prefix
		cout << " - Prefix";
		correct = PrefiSyntaxCheker(input, str_size);
		if (correct) {
			PrefToInf(input, res1, str_size);
			cout << "\nOutput:[" << res1 << "] - Infix";
			PrefToPost(input, res2, str_size);
			cout << "\nOutput:[" << res2 << "] - Postfix";
		}
	}
	else {
		size_t pos = str_size - 1;
		if (input[pos] == ' ' || OperChecker(input[pos]) < 0) while (--pos != 0 && (input[pos] == ' ' || OperChecker(input[pos]) < 0));//if unar operators at the end

		if (OperChecker(input[pos]) > 0) {//if Postfix
			cout << " - Postfix";
			correct = PostfSyntaxCheker(input, str_size);
			if (correct) {
				PostToPref(input, res1, str_size);
				cout << "\nOutput:[" << res1 << "] - Prefix";
				PostToInf(input, res2, str_size);
				cout << "\nOutput:[" << res2 << "] - Infix";
			}
		}
		else if (isalnum(input[pos]) || input[pos] == ')') {//if Infix
			cout << " - Infix";
			correct = InfixSyntaxCheker(input, str_size);
			if (correct) {
				InfToPref(input, res1, str_size);
				cout << "\nOutput:[" << res1 << "] - Prefix";
				InfToPost(input, res2, str_size);
				cout << "\nOutput:[" << res2 << "] - Postfix";
			}
		}
		else cout << "Wrong expression detected.";//if wtf
	}

	if (!correct) { cout << " wrong expression."; }
}
void Terminal_Double_Thread(const char *input, unique_ptr<char[]> &res1, unique_ptr<char[]> &res2)
{//works slower because of "thread" !
	Timer t;
	bool correct = false;
	size_t str_size = strlen(input);

	if (str_size < 3) return;

	cout << "Input :[" << input << ']';

	//Define weather expression is Prefix or Postdix or Infix.
	if (OperChecker(input[0])) {//if Prefix
		cout << " - Prefix";
		correct = PrefiSyntaxCheker(input, str_size);
		if (correct) {
			thread T(PrefToInf, ref(input), ref(res1), str_size);
			PrefToPost(input, res2, str_size);
			if (T.joinable())T.join();
			cout << "\nOutput:[" << res1 << "] - Infix";
			cout << "\nOutput:[" << res2 << "] - Postfix";
		}
	}
	else {
		size_t pos = str_size - 1;
		if (input[pos] == ' ' || OperChecker(input[pos]) < 0) while (--pos != 0 && (input[pos] == ' ' || OperChecker(input[pos]) < 0));//if unar operators at the end

		if (OperChecker(input[pos]) > 0) {//if Postfix
			cout << " - Postfix";
			correct = PostfSyntaxCheker(input, str_size);
			if (correct) {
				thread T(PostToPref, ref(input), ref(res1), str_size);
				PostToInf(input, res2, str_size);
				if (T.joinable())T.join();
				cout << "\nOutput:[" << res1 << "] - Prefix";
				cout << "\nOutput:[" << res2 << "] - Infix";
			}
		}
		else if (isalnum(input[pos]) || input[pos] == ')') {//if Infix
			cout << " - Infix";
			correct = InfixSyntaxCheker(input, str_size);
			if (correct) {
				thread T(InfToPref, ref(input), ref(res1), str_size);
				InfToPost(input, res2, str_size);
				if (T.joinable())T.join();
				cout << "\nOutput:[" << res1 << "] - Prefix";
				cout << "\nOutput:[" << res2 << "] - Postfix";
			}
		}
		else cout << "Wrong expression detected.";//if wtf
	}

	if (!correct) { cout << " wrong expression."; }
}

//Verifying the correctness of expression
bool InfixSyntaxCheker(const char *_string, const size_t _str_size)
{
	bool OperatorExpected = false;
	int Operators = 0;
	int Operands = 0;
	int RoundBrackets = 0;

	for (size_t i = 0; i < _str_size; i++) {
		if (_string[i] == ' ') continue;
		else if (OperChecker(_string[i]) > 0 && OperatorExpected == true) {
			OperatorExpected = false;
			Operators++;
		}
		else if (isalnum(_string[i]) && OperatorExpected == false) {
			OperatorExpected = true;
			Operands++;

			if (isdigit(_string[i])) {
				while (++i < _str_size && isdigit(_string[i]));
				i--;
			}
		}
		else if (_string[i] == '(') RoundBrackets++;
		else if (_string[i] == ')' && OperatorExpected == true) RoundBrackets--;
		else if (OperChecker(_string[i]) < 0 && OperatorExpected == true) continue;

		else return false;
	}

	if (Operands < 2) return false;
	if (Operands != Operators + 1) return false;
	if (RoundBrackets != 0) return false;

	return true;
}
bool PostfSyntaxCheker(const char *_string, const size_t _str_size)
{
	int Operators = 0;
	int Operands = 0;

	for (size_t i = 0; i < _str_size; i++) {
		if (_string[i] == ' ') continue;
		else if (OperChecker(_string[i]) > 0) Operators++;
		else if (isalnum(_string[i])) {
			Operands++;
			if (isdigit(_string[i])) {
				while (++i < _str_size && isdigit(_string[i]));
				i--;
			}
		}
		else return false;
		if (Operands == Operators) return false;
	}

	if (Operands < 2) return false;
	if (Operators != Operands - 1) return false;

	return true;
}
bool PrefiSyntaxCheker(const char *_string, const size_t _str_size)
{
	bool Stop = false;
	int Operators = 0;
	int Operands = 0;

	for (size_t i = 0; i < _str_size; i++)
	{
		if (_string[i] == ' ') continue;
		else if (OperChecker(_string[i]) > 0) Operators++;
		else if (isalnum(_string[i]))
		{
			Operands++;
			if (isdigit(_string[i])) {
				while (i < _str_size && isdigit(_string[i]))i++;
				i--;
			}
		}
		else if (OperChecker(_string[i]) < 0) continue;
		else return false;

		if (Operators + 1 - Operands == 0) Stop = true;
		else if (Stop == true) return false;
	}

	if (Operands < 2) return false;
	if (Operators != Operands - 1) return false;

	return true;
}

//Translators(standart algorythms from the internet):
void InfToPost(const char *_string, unique_ptr<char[]> &ptr, const size_t _str_size)
{
	stack<char> OperStack;
	size_t pos = 0;//iterator
	int new_size = 0;
	int priority = -1;

	for (size_t i = 0; i < _str_size; i++) { //counting amount of -odd places from '(', ')' and needed for ' ' between numbers		
		if (isdigit(_string[i])) {
			new_size++;
			while (isdigit(_string[++i]));
			i--;
		}
		else if (_string[i] == '(' || _string[i] == ')') new_size--;
	}

	ptr = make_unique<char[]>(_str_size + new_size + 1);

	for (size_t i = 0; i < _str_size; i++)//Infix to Postfix converter
	{
		if (isalnum(_string[i])) {
			if (isdigit(_string[i])) {
				if (pos != 0 && isdigit(ptr[pos - 1])) ptr[pos++] = ' ';//make 123' '															  
				while (isdigit(_string[i])) ptr[pos++] = _string[i++];//make 123' '456
				i--;
			}
			else ptr[pos++] = _string[i];
		}
		else if (OperChecker(_string[i])) {
			int t_priority = OperChecker(_string[i]);

			if (t_priority < 0) ptr[pos++] = _string[i];
			else if (t_priority > priority) {
				OperStack.push(_string[i]);
				priority = t_priority;
			}
			else if (t_priority <= priority) {
				ptr[pos++] = OperStack.top();
				OperStack.pop();
				OperStack.push(_string[i]);
				priority = t_priority;
			}
		}
		else if (_string[i] == '(') {
			OperStack.push('(');
			priority = -1;
		}
		else if (_string[i] == ')') {
			while (OperStack.top() != '(') {
				ptr[pos++] = OperStack.top();
				OperStack.pop();
			}
			OperStack.pop();
			if (!OperStack.empty()) priority = OperChecker(OperStack.top());
			else priority = -1;
		}
	}//Infix to Postfix coverter//end

	while (!OperStack.empty()) {
		ptr[pos++] = OperStack.top();
		OperStack.pop();
	}
	ptr[pos] = '\0';
}
void InfToPref(const char *_string, unique_ptr<char[]> &ptr, const size_t _str_size)
{
	char copy_str[G_SIZER];
	size_t pos = _str_size;
	copy_str[pos--] = '\0';

	//Here is recursive lambda expression instead of simple function - because "I can".
	auto InnerReverse = [] (char *copy, size_t begin, size_t end, const auto &Lambda) -> void {
		char temp;
		size_t it;
		end--;

		while (begin < end)
		{
			it = end;

			if (OperChecker(copy[end]) < 0) {
				while (OperChecker(copy[--it]) < 0);
				if (begin >= it) return;
			}
			else if (OperChecker(copy[end]) > 0) {
				temp = copy[end];
				size_t i = end - 1;
				do { copy[i + 1] = copy[i]; } while (i-- != begin);
				copy[begin++] = temp;
				continue;
			}

			if (isalpha(copy[it])) {
				while (it <= end) {
					temp = copy[it];
					size_t i = it - 1;
					do { copy[i + 1] = copy[i]; } while (i-- != begin);
					copy[begin++] = temp;
					it++;
				}
			}
			else if (isdigit(copy[it])) {
				while (begin < it && isdigit(copy[it])) {
					temp = copy[it];
					size_t i = it - 1;
					do { copy[i + 1] = copy[i]; } while (i-- != begin);
					copy[begin++] = temp;
				}
				while (begin < it && ++it <= end) {//If (it != end)
					temp = copy[it];
					size_t i = it - 1;
					do { copy[i + 1] = copy[i]; } while (i-- != begin);
					copy[begin++] = temp;
				}
			}
			else if (copy[it] == ')') {
				size_t l_begin = begin + 1;
				size_t l_end = it;//last bracket

				int bracket = 1;
				while (bracket != 0) {
					it--;
					if (copy[it] == ')') bracket++;
					else if (copy[it] == '(') bracket--;
				}//it == first bracket

				l_end = l_end - it + begin;//first bracket + distance between brackets

				if (begin < it) while (it <= end) {
					temp = copy[it];
					size_t i = it - 1;
					do { copy[i + 1] = copy[i]; } while (i-- != begin);
					copy[begin++] = temp;
					it++;
				}
				else begin = end;

				Lambda(copy, l_begin, l_end, Lambda);
			}
			else terminate();
		}
	};//InnerReverse(Lambda) end//

#if 1
	memcpy(copy_str, _string, _str_size);
	InnerReverse(copy_str, 0, _str_size, InnerReverse);

#else //Reversing while copying part(a bit faster but less recursive way).
	for (size_t i = 0; i < _str_size; i++, pos--) {
		if (OperChecker(_string[pos]) < 0) {
			size_t unar_end = pos + 1;
			while (OperChecker(_string[--pos]) < 0);

			if (isalpha(_string[pos])) {
				copy_str[i] = _string[pos];
				for (size_t it = pos + 1; it < unar_end; it++) copy_str[++i] = _string[it];
			}
			else if (isdigit(_string[pos])) {
				size_t it = pos + 1;
				while (pos < _str_size && isdigit(_string[pos])) copy_str[i++] = _string[pos--];
				for (it; it < unar_end; it++) copy_str[i++] = _string[it];
				i--;
				pos++;
			}
			else if (_string[pos] == ')') {
				int brackets = 1;
				size_t l_begin = i + 1;
				size_t l_end = pos + 1;

				while (brackets != 0) {
					pos--;
					if (_string[pos] == ')') brackets++;
					else if (_string[pos] == '(') brackets--;
				}//pos - will point to first bracket

				size_t j = pos;
				while (j < l_end) copy_str[i++] = _string[j++];
				l_end = i - 1;
				while (j < unar_end) copy_str[i++] = _string[j++];
				i--;

				InnerReverse(copy_str, l_begin, l_end, InnerReverse);
			}
		}
		else if (_string[pos] == '(') copy_str[i] = ')';
		else if (_string[pos] == ')') copy_str[i] = '(';
		else    copy_str[i] = _string[pos];
	}
#endif

	if (!InfixSyntaxCheker(copy_str, _str_size)) {
		cout << "\n[ERROR] Infix reverse failed\n";
		return;
	}

	InfToPost(copy_str, ptr, _str_size);//InfToPost call

	pos = strlen(ptr.get()) - 1;
	for (size_t i = 0; pos > i; pos--, i++) swap(ptr[i], ptr[pos]);//postfix string reverse
}

void PostToInf(const char *_string, unique_ptr<char[]> &ptr, const size_t _str_size)
{
	int priority;//for operator priority
	size_t oper_num = 0;
	size_t unar_num = 0;
	size_t
		up = 0,//stack level iterators
		low = 0,
		j = 1,//iterators
		k = 1;
	size_t new_size;

	for (size_t i = 0; i < _str_size; i++) {
		if (OperChecker(_string[i]) > 0) oper_num++;//counting number of binary operators
		else if (OperChecker(_string[i]) < 0) unar_num++;//counting number of unary operators
	}

	oper_num++;
	new_size = _str_size + (oper_num + unar_num) * 2;// the maximum possible amount of chars in result 

	vector<vector<char>> mas(oper_num, vector<char>(new_size));//matrix as a stack
	ptr = make_unique<char[]>(new_size);

	for (size_t pos = 0; pos < _str_size; pos++)
	{
		if (isalpha(_string[pos]))//if we meet a letter
		{
			j = 2;
			mas[up][0] = '0';//'0' is right
			mas[up][1] = _string[pos];
			if (pos + 1 < _str_size && OperChecker(_string[pos + 1]) < 0) {
				while (pos + 1 < _str_size && OperChecker(_string[++pos]) < 0) mas[up][j++] = _string[pos];
				pos--;
			}
			mas[up][j] = '\0';
			up++;
		}
		else if (isdigit(_string[pos]))//if we meet a number
		{
			j = 1;
			mas[up][0] = '0';//'0' is right
			while (isdigit(_string[pos])) mas[up][j++] = _string[pos++];
			if (pos + 1 < _str_size && OperChecker(_string[pos]) < 0) while (pos + 1 < _str_size && OperChecker(_string[pos]) < 0) mas[up][j++] = _string[pos++];
			mas[up][j] = '\0';
			pos--;
			up++;
		}
		else if (OperChecker(_string[pos]) < 0) {//if we meet an unar operator
			j = 1;
			while (mas[up][j++] != '\0');
			mas[up][j++] = _string[pos];
			mas[up][j] = '\0';
		}
		else if (OperChecker(_string[pos]) > 0)//if we meet a binary operator
		{//we make one string out of two with operator in between
			up--;
			low = up - 1;
			j = 1;
			k = 1;

			ptr[0] = mas[low][0];
			if (pos + 1 < _str_size && OperChecker(_string[pos + 1]) < 0) ptr[j++] = '(';
			while (mas[low][k] != '\0') ptr[j++] = mas[low][k++];//copy from stack to buff
			ptr[j] = '\0';

			priority = OperChecker(_string[pos]);
			j = 1;

			if ((ptr[0] != '0') && (priority > ptr[0] - 48)) {//if first operands have lower priority we put '(' and ')'
				mas[low][j++] = '(';
				k = 1;
				while (ptr[k] != '\0') mas[low][j++] = ptr[k++];
				mas[low][j++] = ')';
			}
			else while (ptr[j] != '\0') mas[low][j] = ptr[j++]; //without brackets '(')'

			mas[low][0] = static_cast<char>(priority + 48);
			mas[low][j++] = _string[pos];

			if ((mas[up][0] != '0') && (priority > mas[up][0] - 48)) {//if second operands have lower priority
				mas[low][j++] = '(';
				k = 1;
				while (mas[up][k] != '\0') mas[low][j++] = mas[up][k++];
				mas[low][j++] = ')';
			}
			else {
				k = 1;
				while (mas[up][k] != '\0') {
					mas[low][j] = mas[up][k++];
					j++;
				}
			}

			if (pos + 1 < _str_size && OperChecker(_string[pos + 1]) < 0) {
				mas[low][0] = '0';
				mas[low][j++] = ')';
				while (pos + 1 < _str_size && OperChecker(_string[++pos]) < 0) mas[low][j++] = _string[pos];
				pos--;
			}
			mas[low][j] = '\0';
		}//if we meet a binary operator//end
	}
	j = 1;
	k = 0;
	while (mas[0][j] != '\0') ptr[k++] = mas[0][j++];//copy result from stack to 'ptr'
	ptr[k] = '\0';
}
void PostToPref(const char*_string, unique_ptr<char[]> &ptr, const size_t _str_size)
{
	stack<char> OperStack;
	stack<int> PairCondition;
	size_t space_count = 0;//' '//
	size_t num_count = 0;
	size_t j = 0;//iterator

	for (size_t i = 0; i < _str_size; i++) {
		if (_string[i] == ' ') space_count++;
		else if (isdigit(_string[i])) {
			while (isdigit(_string[++i]));
			num_count++;
			i--;
		}
	}
	j = _str_size + num_count - space_count + 1;

	ptr = make_unique<char[]>(j);

	ptr[--j] = '\0';
	j--;
	if (num_count)memset(ptr.get(), '$', num_count);

	for (size_t i = _str_size - 1; i < _str_size; )
	{
		if (_string[i] == ' ') i--;
		else if (OperChecker(_string[i]) > 0) {//|+^
			OperStack.push(_string[i--]);
			PairCondition.push(0);
		}
		else if (OperChecker(_string[i]) < 0) OperStack.push(_string[i--]);
		else if (isalpha(_string[i]))
		{
			ptr[j--] = _string[i--];

			if (!OperStack.empty() && OperChecker(OperStack.top()) < 0)//\...+!!!!\ |...a| -> \..+\ |...a!!!!|
				while (!OperStack.empty() && OperChecker(OperStack.top()) < 0) {
					ptr[j--] = OperStack.top();
					OperStack.pop();
				}

			if (!PairCondition.empty()) PairCondition.top()++;

			while (!PairCondition.empty() && PairCondition.top() == 2) {
				ptr[j--] = OperStack.top();
				OperStack.pop();

				if (!OperStack.empty() && OperChecker(OperStack.top()) < 0)//\...+!!!!\ |...a+| -> |...a+!!!!|
					while (!OperStack.empty() && OperChecker(OperStack.top()) < 0) {
						ptr[j--] = OperStack.top();
						OperStack.pop();
					}

				PairCondition.pop();
				if (!PairCondition.empty()) PairCondition.top()++;
			}
		}
		else if (isdigit(_string[i]))
		{
			if (isdigit(ptr[j + 1])) ptr[j--] = ' ';

			while (i < _str_size && isdigit(_string[i])) ptr[j--] = _string[i--];

			if (!OperStack.empty() && OperChecker(OperStack.top()) < 0) //|+!!!!|a| -> |
				while (!OperStack.empty() && OperChecker(OperStack.top()) < 0) {
					ptr[j--] = OperStack.top();
					OperStack.pop();
				}

			if (!PairCondition.empty()) PairCondition.top()++;

			while (!PairCondition.empty() && PairCondition.top() == 2) {
				ptr[j--] = OperStack.top();
				OperStack.pop();

				if (!OperStack.empty() && OperChecker(OperStack.top()) < 0)//\...+!!!!\ |...a+| -> |...a+!!!!|
					while (!OperStack.empty() && OperChecker(OperStack.top()) < 0) {
						ptr[j--] = OperStack.top();
						OperStack.pop();
					}

				PairCondition.pop();
				if (!PairCondition.empty()) PairCondition.top()++;
			}
		}
	}

	while (!OperStack.empty()) {
		ptr[j--] = OperStack.top();
		OperStack.pop();
	}

	while (ptr[0] == '$') {
		j = 0;
		while (ptr[j] != '\0') {
			ptr[j] = ptr[j + 1];
			j++;
		}
		ptr[--j] = '\0';
	}
}

void PrefToInf(const char *_string, unique_ptr<char[]> &ptr, const size_t _str_size)
{
	size_t oper_num = 0;
	size_t unar_num = 0;
	size_t
		up = 0,//stack level iterators
		low = 0,
		j = 1,//iterators
		k = 1;
	size_t new_size;
	int priority;//for operator priority

	for (size_t i = 0; i < _str_size; i++) {
		if (OperChecker(_string[i]) > 0) oper_num++;//counting number of binary operators
		else if (OperChecker(_string[i]) < 0) unar_num++;//counting number of unary operators
	}

	oper_num++;
	new_size = _str_size + (oper_num + unar_num) * 2;// the maximum possible amount of chars in result 

	vector<vector<char>> mas(oper_num, vector<char>(new_size));//matrix as a stack
	ptr = make_unique<char[]>(new_size);

	for (size_t pos = _str_size - 1; pos < _str_size; pos--)
	{
		if (_string[pos] == ' ') continue;
		if (isalpha(_string[pos]))//if we meet a letter
		{
			j = 2;
			mas[up][0] = '0';//'0' is right
			mas[up][1] = _string[pos];
			if (pos - 1 < _str_size && OperChecker(_string[pos - 1]) < 0) {
				while (pos - 1 < _str_size && OperChecker(_string[--pos]) < 0) mas[up][j++] = _string[pos];
				pos++;
			}
			mas[up][j] = '\0';
			up++;
		}
		else if (isdigit(_string[pos]))//if we meet a number
		{
			size_t it = pos;
			size_t j_it;

			while (isdigit(_string[--it]));
			j_it = pos - it;
			j = j_it + 1;
						
			while (it < pos)  mas[up][j_it--] = _string[pos--];

			if (pos < _str_size && OperChecker(_string[pos]) < 0) while (pos < _str_size && OperChecker(_string[pos]) < 0) mas[up][j++] = _string[pos--];

			mas[up][0] = '0';//'0' is right
			mas[up][j] = '\0';
			pos++;
			up++;
		}
		else if (OperChecker(_string[pos]) < 0) {//if we meet an unar operator
			j = 1;
			while (mas[up][j++] != '\0');
			mas[up][j++] = _string[pos];
			mas[up][j] = '\0';
		}
		else if (OperChecker(_string[pos]) > 0)//if we meet a binary operator
		{//we make one string out of two with operator in between
			up--;
			low = up - 1;
			j = 1;
			k = 1;

			priority = OperChecker(_string[pos]);

			ptr[0] = mas[low][0];
			while (mas[low][k] != '\0') {
				ptr[k] = mas[low][k];//copy from stack to buff
				k++;
			}
			ptr[k] = '\0';
			k = 1;

			if (pos - 1 < _str_size && OperChecker(_string[pos - 1]) < 0) mas[low][j++] = '(';

			if ((mas[up][0] != '0') && (priority > mas[up][0] - 48)) {//if upper operands have lower priority
				mas[low][j++] = '(';
				while (mas[up][k] != '\0') mas[low][j++] = mas[up][k++];
				mas[low][j++] = ')';
			}
			else {
				while (mas[up][k] != '\0') mas[low][j++] = mas[up][k++];
			}
			//
			mas[low][0] = static_cast<char>(priority + 48);
			mas[low][j++] = _string[pos];
			//
			k = 1;
			if ((ptr[0] != '0') && (priority > ptr[0] - 48)) {//if //lower operands have lower priority we put '(' and ')'
				mas[low][j++] = '(';
				while (ptr[k] != '\0') mas[low][j++] = ptr[k++];
				mas[low][j++] = ')';
			}
			else {
				while (ptr[k] != '\0') mas[low][j++] = ptr[k++]; //without brackets
			}

			if (pos - 1 < _str_size && OperChecker(_string[pos - 1]) < 0) {
				mas[low][0] = '0';
				mas[low][j++] = ')';
				while (pos - 1 < _str_size && OperChecker(_string[--pos]) < 0) mas[low][j++] = _string[pos];
				pos++;
			}
			mas[low][j] = '\0';
		}//if we meet a binary operator//end
	}
	j = 1;
	k = 0;
	while (mas[0][j] != '\0') ptr[k++] = mas[0][j++];//copy result from stack to 'ptr'
	ptr[k] = '\0';
}
void PrefToPost(const char*_string, unique_ptr<char[]> &ptr, const size_t _str_size)
{
	stack<char> OperStack;
	stack<int> PairCondition;
	size_t space_count = 0;//' '//
	size_t num_count = 0;
	size_t j = 0;//iterator

	for (size_t i = 0; i < _str_size; i++) {
		if (_string[i] == ' ') space_count++;
		else if (isdigit(_string[i])) {
			while (isdigit(_string[++i]));
			num_count++;
			i--;
		}
	}

	ptr = make_unique<char[]>(_str_size + num_count - space_count + 1);

	for (size_t i = 0; i < _str_size; )
	{
		if (_string[i] == ' ') i++;
		else if (OperChecker(_string[i]) > 0) {
			OperStack.push(_string[i++]);
			PairCondition.push(0);
		}
		else if (OperChecker(_string[i]) < 0) OperStack.push(_string[i++]);
		else if (isalpha(_string[i]))
		{
			ptr[j++] = _string[i++];

			if (!OperStack.empty() && OperChecker(OperStack.top()) < 0)//\...+!!!!\ |...a| -> \..+\ |...a!!!!|
				while (!OperStack.empty() && OperChecker(OperStack.top()) < 0) {
					ptr[j++] = OperStack.top();
					OperStack.pop();
				}

			PairCondition.top()++;

			while (!PairCondition.empty() && PairCondition.top() == 2) {
				ptr[j++] = OperStack.top();
				OperStack.pop();

				if (!OperStack.empty() && OperChecker(OperStack.top()) < 0)//\...+!!!!\ |...a+| -> |...a+!!!!|
					while (!OperStack.empty() && OperChecker(OperStack.top()) < 0) {
						ptr[j++] = OperStack.top();
						OperStack.pop();
					}

				PairCondition.pop();
				if (!PairCondition.empty()) PairCondition.top()++;
			}
		}
		else if (isdigit(_string[i]))
		{
			if (j > 0 && isdigit(ptr[j - 1])) ptr[j++] = ' ';
			while (isdigit(_string[i])) ptr[j++] = _string[i++];

			if (!OperStack.empty() && OperChecker(OperStack.top()) < 0) //|+!!!!|a| -> |
				while ((OperChecker(OperStack.top()) < 0)) {
					ptr[j++] = OperStack.top();
					OperStack.pop();
				}

			PairCondition.top()++;

			while (!PairCondition.empty() && PairCondition.top() == 2) {
				ptr[j++] = OperStack.top();
				OperStack.pop();

				if (!OperStack.empty() && OperChecker(OperStack.top()) < 0)//\...+!!!!\ |...a+| -> |...a+!!!!|
					while (!OperStack.empty() && OperChecker(OperStack.top()) < 0) {
						ptr[j++] = OperStack.top();
						OperStack.pop();
					}

				PairCondition.pop();
				if (!PairCondition.empty()) PairCondition.top()++;
			}
		}
	}

	while (!OperStack.empty()) {
		ptr[j++] = OperStack.top();
		OperStack.pop();
	}
	ptr[j] = '\0';
}

//Translator made by myself
//void PrefToInfMyOwn(const char *_string, unique_ptr<char[]> &ptr, const size_t _str_size)
#if 0
void PrefToInfMyOwn(const char *_string, unique_ptr<char[]> &ptr, const size_t _str_size)
{
	bool block_ready = 0;
	bool one_operand = 0;
	size_t oper_num = 0;
	size_t
		s = 0, //stack iterators
		t = 0,
		j = 1,//iterators
		k = 1;

	for (size_t i = 0; i < _str_size; i++) if (OperChecker(_string[i])) oper_num++;//counting nuber of binary operators(determine stack depth)
	oper_num++;
	size_t p_size = _str_size + oper_num * 2;// the maximum possible amount of chars in result 

	ptr = new char[p_size];

	char** mas = new char*[oper_num];//stack from massives            
	for (size_t i = 0; i < oper_num; i++) mas[i] = new char[p_size];


	for (size_t pos = 0; pos < _str_size; pos++)//Prefix to Infix converter
	{
		if (_string[pos] == ' ') continue;//!?
		else if (OperChecker(_string[pos]) > 0) {//if we meet binary operator
			mas[s][0] = '0';//'0' is right
			mas[s][1] = _string[pos];
			mas[s][2] = '\0';
			s++;
		}
		else if (OperChecker(_string[pos]) < 0) {//if we meet an unar operator make/!!!.../ -> |0!3|
			int  unar_num = 1;
			char unar_temp = _string[pos];

			while (_string[++pos] == unar_temp) unar_num++;
			if (unar_num > 9) { unar_num = 9; cout << "ATTENTION:too many similar unar operators\n"; }
			mas[s][0] = '0';//'0' is right
			mas[s][1] = unar_temp;
			mas[s][2] = unar_num + 48;
			mas[s][3] = '\0';

			pos--;
			s++;
		}
		else if (isalpha(_string[pos]))//if we meet a letter
		{
			s--;
			if (OperChecker(mas[s][1]) < 0) {//if empty level of stack only with UNARY operator make|0!3|/a.../ -> |0a!!!|
				char unar_temp = mas[s][1];//=! if |0!3|
				int  unar_num = mas[s][2] - 48;//=3 if |0!3|
				j = 2;
				t = s - 1;

				mas[s][0] = '0';
				mas[s][1] = _string[pos];
				while (unar_num--) mas[s][j++] = unar_temp;//make|0a!!!|					
				mas[s][j] = '\0';

				//if program accepts more than 1 unar operators:sin,cos etc.
				/*while (OperChecker(mas[s - 1][1]) < 0) {
				t = s - 1;
				char unar_temp = mas[t][1];//=! if |0!3|
				int unar_num = mas[t][2] - 48;//=3 if |0!3|
				j = 1;

				mas[t][0] = '0';
				while (mas[s][j] != '\0') mas[t][j] = mas[s][j++];
				//mas[t][j] = string[pos];
				while (unar_num--) mas[s][j++] = unar_temp;//make|0a!!!sinsinsin|
				mas[s][j] = '\0';
				s--;
				}*/

				if (OperChecker(mas[t][1]) > 0) {//if empty level of stack only with BINARY operator make|0+|/a!!!.../ -> |#a!!!+|

					j = 1;
					char oper_temp = mas[t][1];

					mas[t][0] = '#';//'#' means partically filled stack level
					while (mas[s][j] != '\0') mas[t][j] = mas[s][j++];
					mas[t][j++] = oper_temp;
					mas[t][j] = '\0';

					s--;
				}
				else if (mas[t][0] == '#') {//if partically filld stack level convert from|#b+|a!!!| -> |1b+a!!!|
					j = 0;
					k = 1;

					while (mas[t][++j] != '\0');
					mas[t][0] = OperChecker(mas[t][j - 1]) + 48;
					while (mas[s][k] != '\0') mas[t][j++] = mas[s][k++];
					mas[t][j] = '\0';

					s--;
					block_ready = true;
				}
			}
			else if (OperChecker(mas[s][1]) > 0) {//if empty level of stack only with BINARY operator make|0+|/a.../ -> |#a+|
				mas[s][0] = '#';//'#' means partically filled stack level
				mas[s][2] = mas[s][1];
				mas[s][1] = _string[pos];
				mas[s][3] = '\0';
			}
			else if (mas[s][0] == '#') {//if partically filld stack level convert from|#b+|/a.../ -> |b+a|
				j = 0;
				while (mas[s][++j] != '\0');
				mas[s][0] = OperChecker(mas[s][j - 1]) + 48;
				mas[s][j++] = _string[pos];
				mas[s][j] = '\0';

				block_ready = true;
			}
			s++;
		}//else if (isalpha(string[pos]))//end
		else if (isdigit(_string[pos]))//if we meet a number
		{
			s--;
			if (OperChecker(mas[s][1]) < 0) {//if empty level of stack only with UNARY operator make|0!3|/a.../ -> |0a!!!|
				char unar_temp = mas[s][1];//=! if |0!3|
				int  unar_num = mas[s][2] - 48;//=3 if |0!3|
				j = 1;
				t = s - 1;

				mas[s][0] = '0';
				while (isdigit(_string[pos])) mas[s][j++] = _string[pos++];//
				while (unar_num--) mas[s][j++] = unar_temp;//make|0a!!!|					
				mas[s][j] = '\0';

				pos--;
				//if program accepts more than 1 unar operators:sin,cos etc.
				/*while (OperChecker(mas[s - 1][1]) < 0) {
				t = s - 1;
				char unar_temp = mas[t][1];//=! if |0!3|
				int unar_num = mas[t][2] - 48;//=3 if |0!3|
				j = 1;

				mas[t][0] = '0';
				while (mas[s][j] != '\0') mas[t][j] = mas[s][j++];
				//mas[t][j] = string[pos];
				while (unar_num--) mas[s][j++] = unar_temp;//make|0a!!!sinsinsin|
				mas[s][j] = '\0';
				s--;
				}*/

				if (OperChecker(mas[t][1]) > 0) {//if empty level of stack only with BINARY operator make|0+|/a!!!.../ -> |#a!!!+|

					j = 1;
					char oper_temp = mas[t][1];

					mas[t][0] = '#';//'#' means partically filled stack level
					while (mas[s][j] != '\0') mas[t][j] = mas[s][j++];
					mas[t][j++] = oper_temp;
					mas[t][j] = '\0';

					s--;
				}
				else if (mas[t][0] == '#') {//if partically filld stack level convert from|#b+|123!!!| -> |1b+123!!!|
					j = 0;
					k = 1;

					while (mas[t][++j] != '\0');
					mas[t][0] = OperChecker(mas[t][j - 1]) + 48;
					while (mas[s][k] != '\0') mas[t][j++] = mas[s][k++];
					mas[t][j] = '\0';

					s--;
					block_ready = true;
				}
			}
			else if (OperChecker(mas[s][1]) > 0) {//if empty level of stack only with BINARY operator make|0+|/a.../ -> |#a+|
				char oper_temp = mas[s][1];
				j = 1;

				mas[s][0] = '#';//'#' means partically filled stack level
				while (isdigit(_string[pos])) mas[s][j++] = _string[pos++];//				
				mas[s][j++] = oper_temp;
				mas[s][j] = '\0';

				pos--;
			}
			else if (mas[s][0] == '#') {//if partically filld stack level convert from|#b+|/a.../ -> |b+a|
				j = 0;
				while (mas[s][++j] != '\0');
				mas[s][0] = OperChecker(mas[s][j - 1]) + 48;
				while (isdigit(_string[pos])) mas[s][j++] = _string[pos++];//				
				mas[s][j] = '\0';

				pos--;
				block_ready = true;
			}
			s++;
		}//else if (isdigit(string[pos]))//if we meet a number//end

		while (block_ready && s > 1)//we recive |0!2| or |0+| or |#c+| with |1a+b| -> |(a!+b!!)!| \\ |a!+b!!| \\ |#a*(a!+b!!)|
		{
			s--;
			t = s - 1;

			if (mas[t][0] == '#' && mas[s][0] != '#') {//if |#d^|#c*|1a+b| -> |#d^|2c*(a+b)| -> |3d^(c*(a+b))|
				j = 0;
				k = 1;

				while (mas[t][++j] != '\0');

				if (mas[s][0] - 48 < OperChecker(mas[t][j - 1])) {//if operands have lower priority				
					mas[t][0] = OperChecker(mas[t][j - 1]);
					mas[t][j++] = '(';
					while (mas[s][k] != '\0') mas[t][j++] = mas[s][k++];
					mas[t][j++] = ')';
				}
				else {
					mas[t][0] = mas[s][0];
					while (mas[s][k] != '\0') mas[t][j++] = mas[s][k++];
				}

				mas[t][j] = '\0';
				s--;
			}
			else if (mas[s][0] != '#' && OperChecker(mas[t][1]) > 0) {//if |0*|1a+b| -> |#(a+b)*|
				j = 1;
				k = 1;
				char oper_temp = mas[t][1];

				if (mas[s][0] - 48 < OperChecker(mas[t][1])) {//if operands have lower priority				
					mas[t][j++] = '(';
					while (mas[s][k] != '\0') mas[t][j++] = mas[s][k++];
					mas[t][j++] = ')';
				}
				else while (mas[s][j] != '\0') mas[t][j] = mas[s][j++];

				mas[t][0] = '#';
				mas[t][j++] = oper_temp;
				mas[t][j] = '\0';
				s--;
			}
			else if (mas[s][0] != '0' && mas[s][0] != '#' && OperChecker(mas[t][1]) < 0) {//|0!N|1a+b| -> |1(a+b)!/N/| :N-quantity
				j = 2;
				k = 1;

				char unar_temp = mas[t][1];
				int  unar_num = mas[t][2] - 48;

				mas[t][0] = '1';
				mas[t][1] = '(';
				while (mas[s][k] != '\0') mas[t][j++] = mas[s][k++];
				mas[t][j++] = ')';
				while (unar_num--) mas[t][j++] = unar_temp;//make |?(a+b)!!!|
				mas[t][j] = '\0';

				s--;
			}
			else block_ready = false;
			s++;
		}//while (block_ready && s > 1)//end
		block_ready = false;
	}//for (size_t pos = 0; pos < _str_size; pos++)//end

	j = 1;
	k = 0;
	while (mas[0][j] != '\0') ptr[k++] = mas[0][j++];//copy result from stack to 'ptr'
	ptr[k] = '\0';

	for (size_t i = 0; i < oper_num; i++)
		delete[] mas[i];
	delete[] mas;
	//PrefToInf//end
}
#endif