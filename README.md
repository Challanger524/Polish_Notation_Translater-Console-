# Polish Notation Translaters (WinAPI and Console + native tests)

Notation Translator - is a program which converts mathematical expressions from given notation to two another(a+b -> ab+ or +ab).
Both programs are written in C++ language in Microsoft Visual Studio IDE.

Program exists in two variants: 1-Console version and 2-Windowed(provided with GUI).
Console(1) interface provides basic i/o operations without any special options. Otherwise Windowed(2) has addition option for copying result to the buffer clip and choosing the examples.


## So how the program translates?

### Step[0] – Retrieving the Input(the only interface dependent part):
Console – trough a while loop inside main() function.
WinAPI – through edit box and message processing function(case VK_RETURN) of the Main window.
With regex expression input is checked for presense of odd symbols.

###Step[1]  - Processing the Input:
Implemented through calling Terminal(input,...) std::function wrapper which determining the notation type and verifying the correctness of the passed expression by calling certain SpellCheckers().		              After calls translator functions which converts the input from presented notation to another two             (like InfToPost() – infix to postfix func.).

###Step[2] – Actually the Translating:
Presented as 6 different functions with translating algorithms taken from the internet (Wiki, StackOveflow etc.). 
These functions responsible for: 
1. Allocating enough memory for new expression before translation (length could change).
2. Execution the translation algorithm.


## Going backwards:

After Step[2] Terminal() func. will print to the console translated expressions and their notation type.

After Step[1] (after Terminal() execution) two smart pointers will be left pointing to the c-strings with translation or they will be null pointers if input was incorrect.

GUI version outputs the result in another way after Terminal() completion.

Manual button is responsible for a tabbed window which provides basic information about the program (key combination for console opening, input requirements etc.).
