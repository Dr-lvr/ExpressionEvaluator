#include <stack>
#include <string>
#include <sstream>
#include <ostream>
#include <iostream>
#include <iomanip>
#include <unordered_map>

std::string manageNegativeNumbers(std::string expression) {
	
	std::stack<char> firstStack;
	std::stack<std::string> secondStack;
	std::vector<std::string> infix_expressionVec;
	std::string f_pivotString;
	std::string s_pivotString;
	for (int i = 0; i < (int)expression.size(); ++i) {
		if (expression.at(i) == '-') {
			if (firstStack.empty()) {
				firstStack.push(' ');
				firstStack.push('0');
			}
			else {
				switch (firstStack.top()) {
				case '+':case '-':case '*':case '/':
					firstStack.push(' ');
					firstStack.push('0');
					break;
				case ')':case '(':
					if (expression.at(i + 2) != '(') {
						firstStack.push(' ');
						firstStack.push('0');
						break;
					}
					break;
				default:
					break;
				}

			}
		}
		if (expression.at(i) != ' ') {
			firstStack.push(expression.at(i));
		}
	}
	while (!firstStack.empty()) {
		s_pivotString.push_back(firstStack.top());
		firstStack.pop();
	}
	std::reverse(s_pivotString.begin(), s_pivotString.end());
	for (int i = 0; i < (int)s_pivotString.size(); ++i) {
		while (std::isdigit(s_pivotString.at(i)) || s_pivotString.at(i) == '.') {
			f_pivotString += s_pivotString.at(i);
			++i;
			if (i == (int)s_pivotString.size()) {
				break;
			}
		}
		if (!f_pivotString.empty()) {
			infix_expressionVec.push_back(f_pivotString);
			f_pivotString.erase(f_pivotString.begin(), f_pivotString.end());
		}
		if (i == (int)s_pivotString.size()) {
			break;
		}
		infix_expressionVec.push_back(std::string(1, s_pivotString.at(i)));
	}
	for (int i = 0; i < (int)infix_expressionVec.size(); ++i) {
		if (infix_expressionVec.at(i) == " ") {
			infix_expressionVec.at(i) = "(";
			for (int j = i + 1; j < (int)infix_expressionVec.size(); ++j) {
				if (infix_expressionVec.at(j).at(0) == '(') {
					while (infix_expressionVec.at(j).at(0) != ')') {
						++j;
					}
					if (j < (int)infix_expressionVec.size() - 1) {
						infix_expressionVec.insert((infix_expressionVec.begin() + (j + 1)), std::string(1, ')'));
					}
					else {
						infix_expressionVec.push_back(std::string(1, ')'));
					}
					break;
				}
				if (std::isdigit(infix_expressionVec.at(j).at(0)) && infix_expressionVec.at(j).at(0) != '0') {
					if (j < (int)infix_expressionVec.size() - 1) {
						infix_expressionVec.insert((infix_expressionVec.begin() + (j + 1)), std::string(1, ')'));
					}
					else {
						infix_expressionVec.push_back(std::string(1, ')'));
					}
					break;
				}
			}
		}
	}
	f_pivotString.erase(f_pivotString.begin(), f_pivotString.end());
	for (int i = 0; i < (int)infix_expressionVec.size(); ++i) {
		if (!secondStack.empty()) {
			if (secondStack.top() == ")" && infix_expressionVec.at(i) == "(") {
				secondStack.push(std::string(1, '+'));
			}
		}
		secondStack.push(infix_expressionVec.at(i));
	}
	infix_expressionVec.erase(infix_expressionVec.begin(), infix_expressionVec.end());
	while (!secondStack.empty()) {
		infix_expressionVec.push_back(secondStack.top());
		secondStack.pop();
	}
	std::reverse(infix_expressionVec.begin(), infix_expressionVec.end());
	for (auto& c : infix_expressionVec) {
		f_pivotString += c;
	}
	for (auto& c : infix_expressionVec) {
		std::cout << c;
	}
	std::cout << std::endl;
	return f_pivotString;
}
double calc(std::string expression) {

	std::string s_pivotString;
	std::string f_pivotString;
	std::stringstream streamObj;
	std::stack<std::string> stack;
	std::deque<std::string> postfix_expressionDeque;
	std::unordered_map <char, int> operands_vMap = { {'+', 1},{'-', 1},{'*', 2},{'/', 2} };
	expression = manageNegativeNumbers(expression);
	for (int i = 0; i < (int)expression.size(); ++i) {
		while (std::isdigit(expression.at(i)) || expression.at(i) == '.') {
			f_pivotString += expression.at(i);
			++i;
			if (i == (int)expression.size()) {
				break;
			}
		}
		if (!f_pivotString.empty()) {
			postfix_expressionDeque.push_back(f_pivotString);
			f_pivotString.erase(f_pivotString.begin(), f_pivotString.end());
		}
		if (i == (int)expression.size()) {
			break;//end operand found
		}
		switch (expression.at(i)) {
		case '(':stack.push("("); break;
		case ')':
			while (!stack.empty() && stack.top() != "(") {
				postfix_expressionDeque.push_back(stack.top());
				stack.pop();
			}
			stack.pop();
			break;
		case '+':case '-':case '*':case '/':
			if (stack.empty() || stack.top() == "(") {
				if (expression.at(i) == '+') stack.push("+");
				if (expression.at(i) == '-') stack.push("-");
				if (expression.at(i) == '*') stack.push("*");
				if (expression.at(i) == '/') stack.push("/");
			}
			else {
				while (!stack.empty() && stack.top() != "(" &&
					operands_vMap[expression.at(i)] <= operands_vMap[stack.top().at(0)]) {
					postfix_expressionDeque.push_back(stack.top());
					stack.pop();
				}
				if (expression.at(i) == '+') stack.push("+");
				if (expression.at(i) == '-') stack.push("-");
				if (expression.at(i) == '*') stack.push("*");
				if (expression.at(i) == '/') stack.push("/");
			}
			break;
		default:
			break;
		}
	}
	while (!stack.empty()) {
		postfix_expressionDeque.push_back(stack.top());
		stack.pop();
	}
	long double a, b, c;
	a = b = c = 0;
	while (!postfix_expressionDeque.empty()) {
		if (!std::ispunct(postfix_expressionDeque.front().at(0))) {
			stack.push(postfix_expressionDeque.front());
		}
		else {
			streamObj
				<< std::scientific
				<< std::setprecision(std::numeric_limits<long double>::digits10)
				<< stack.top();
			stack.pop();
			streamObj >> a;
			streamObj.clear();
			streamObj
				<< std::scientific
				<< std::setprecision(std::numeric_limits<long double>::digits10)
				<< stack.top();
			stack.pop();
			streamObj >> b;
			streamObj.clear();
			switch (postfix_expressionDeque.front().at(0)) {
			case '+':c = b + a; break;
			case '-':c = b - a; break;
			case '*':c = b * a; break;
			case '/':c = b / a; break;
			default: break;
			}
			streamObj
				<< std::scientific
				<< std::setprecision(std::numeric_limits<long double>::digits10)
				<< c;
			streamObj >> s_pivotString;
			stack.push(s_pivotString);
			streamObj.clear();
		}
		postfix_expressionDeque.pop_front();
	}
	streamObj
		<< std::scientific
		<< std::setprecision(std::numeric_limits<long double>::max_digits10)
		<< stack.top();
	streamObj >> c;
	return c;
}
int main() {

	std::cout
		<< std::setprecision(std::numeric_limits<double>::max_digits10)
		<< calc("(26) - (-25 / -25 + (35)) - (-30 * -(((-(16 + -39)))) + -54)");
	//std::cout << std::setprecision(std::numeric_limits<double>::max_digits10)<< 
		//calc("(91.62+81.06*67.01)*-80.36*17.78+-29.13/((96.68-47.52+((59.91)/(14.82/(72.29+53.25))-42.83)*--50.08+--27.91*44.31+-38.52-3.1*49.28*-(69.01)-6.78/(75.18/(91.89))/(-(40.03))-(-77.49-62.22))-95.66/(-8.05/(6.47))+44.43*((61.86)--43.97+91.32+34.92+86.83+95.86*87.69+(13.36/(-74.02+77.65))+(60.97)/((42.12)+74.91))*4.4+29.84/(47.06))") << std::endl;
	//std::cout << calc("(91.62+81.06*67.01)*-80.36*17.78+-29.13/((96.68-47.52+((59.91)/(14.82/(72.29+53.25))-42.83)*--50.08+--27.91*44.31+-38.52-3.1*49.28*-(69.01)-6.78/(75.18/(91.89))/(-(40.03))-(-77.49-62.22))-95.66/(-8.05/(6.47))+44.43*((61.86)--43.97+91.32+34.92+86.83+95.86*87.69+(13.36/(-74.02+77.65))+(60.97)/((42.12)+74.91))*4.4+29.84/(47.06))") << std::endl;
	/*
	std::cout << "prima operazione --- scritta correttamente ---> (0-7) * (0-(6 / 3)): " << std::endl;
	std::cout << calc("(0-7) * (0-(6 / 3))") <<std::endl;
	std::cout << "seconda operazione --- equivalente ---> -7 * -(6 / 3)" << std::endl;
	std::cout << calc("-7 * -(6 / 3)") << std::endl;
	std::cout << "must be -7891910.63605 . . . " << std::endl;
	std::cout << calc("(91.62+81.06*67.01)*-80.36*17.78+-29.13/((96.68-47.52+((59.91)/(14.82/(72.29+53.25))-42.83)*--50.08+--27.91*44.31+-38.52-3.1*49.28*-(69.01)-6.78/(75.18/(91.89))/(-(40.03))-(-77.49-62.22))-95.66/(-8.05/(6.47))+44.43*((61.86)--43.97+91.32+34.92+86.83+95.86*87.69+(13.36/(-74.02+77.65))+(60.97)/((42.12)+74.91))*4.4+29.84/(47.06))");
	*/
	std::cin.ignore();
}