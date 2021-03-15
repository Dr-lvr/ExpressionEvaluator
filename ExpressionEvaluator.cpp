#include <stack>
#include <string>
#include <sstream>
#include <ostream>
#include <iostream>
#include <iomanip>
#include <unordered_map>

std::string manageNegativeNumbers(std::string expression) {
	std::string exp;
	std::string stringSt;
	std::stack<char> stack;
	std::vector<std::string> inFix;
	for (auto& c : expression) {
		if (c == '-') {
			if (stack.empty()) {
				stack.push(' ');
				stack.push('0');
			}
			else {
				if (std::ispunct(stack.top())) {
					stack.push(' ');
					stack.push('0');
				}
			}
		}
		if (c != ' ') {
			stack.push(c);
		}
	}
	while (!stack.empty()) {
		exp.push_back(stack.top());
		stack.pop();
	}
	std::reverse(exp.begin(), exp.end());
	for (int i = 0; i < (int)exp.size(); ++i) {
		while (std::isdigit(exp.at(i)) || exp.at(i) == '.') {
			stringSt += exp.at(i);
			++i;
			if (i == (int)exp.size()) {
				break;
			}
		}
		if (!stringSt.empty()) {
			inFix.push_back(stringSt);
			stringSt.erase(stringSt.begin(), stringSt.end());
		}
		if (i == (int)exp.size()) {
			break;
		}
		inFix.push_back(std::string(1, exp.at(i)));
	}
	for (int i = 0; i < (int)inFix.size(); ++i) {
		if (inFix.at(i) == " ") {
			inFix.at(i) = "(";
			for (int j = i + 1; j < (int)inFix.size(); ++j) {
				if (inFix.at(j).at(0) == '(') {
					while (inFix.at(j).at(0) != ')') {
						++j;
					}
					if (j < (int)inFix.size() - 1) {
						inFix.insert((inFix.begin() + (j + 1)), std::string(1, ')'));
					}
					else {
						inFix.push_back(std::string(1, ')'));
					}
					break;
				}
				if (std::isdigit(inFix.at(j).at(0)) && inFix.at(j).at(0) != '0') {
					if (j < (int)inFix.size() - 1) {
						inFix.insert((inFix.begin() + (j + 1)), std::string(1, ')'));
					}
					else {
						inFix.push_back(std::string(1, ')'));
					}
					break;
				}
			}
		}
	}
	stringSt.erase(stringSt.begin(), stringSt.end());
	std::stack<std::string> ok;
	for (int i = 0; i < (int)inFix.size(); ++i) {
		if (!ok.empty()) {
			if (ok.top() == ")" && inFix.at(i) == "(") {
				ok.push(std::string(1, '+'));
			}
			if (std::isdigit(ok.top().at(0)) && std::isdigit(inFix.at(i).at(0))) {
				ok.push(std::string(1, '+'));
			}
		}
		ok.push(inFix.at(i));
	}
	inFix.erase(inFix.begin(), inFix.end());
	while (!ok.empty()) {
		inFix.push_back(ok.top());
		ok.pop();
	}
	std::reverse(inFix.begin(), inFix.end());
	for (auto& c : inFix) {
		stringSt += c;
	}
	return stringSt;
}
double calc(std::string expression) {

	std::string st;
	std::string stringSt;
	std::stringstream streamObj;
	std::stack<std::string> stack;
	std::deque<std::string> postFix;
	std::unordered_map <char, int> opMap = { {'+', 1},{'-', 1},{'*', 2},{'/', 2} };
	expression = manageNegativeNumbers(expression);
	for (int i = 0; i < (int)expression.size(); ++i) {
		while (std::isdigit(expression.at(i)) || expression.at(i) == '.') {
			stringSt += expression.at(i);
			++i;
			if (i == (int)expression.size()) {
				break;
			}
		}
		if (!stringSt.empty()) {
			postFix.push_back(stringSt);
			stringSt.erase(stringSt.begin(), stringSt.end());
		}
		if (i == (int)expression.size()) {
			break;//end operand found
		}
		switch (expression.at(i)) {
		case '(':
			stack.push("(");
			break;
		case ')':
			while (!stack.empty() && stack.top() != "(") {
				postFix.push_back(stack.top());
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
					opMap[expression.at(i)] <= opMap[stack.top().at(0)]) {
					postFix.push_back(stack.top());
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
		postFix.push_back(stack.top());
		stack.pop();
	}
	long double a, b, c;
	a = b = c = 0;
	while (!postFix.empty()) {
		if (!std::ispunct(postFix.front().at(0))) {
			stack.push(postFix.front());
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
			switch (postFix.front().at(0)) {
			case '+':
				c = b + a;
				break;
			case '-':
				c = b - a;
				break;
			case '*':
				c = b * a;
				break;
			case '/':
				c = b / a;
				break;
			default:
				break;
			}
			streamObj
				<< std::scientific
				<< std::setprecision(std::numeric_limits<long double>::digits10)
				<< c;
			streamObj >> st;
			stack.push(st);
			streamObj.clear();
		}
		postFix.pop_front();
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
		<<  std::setprecision(std::numeric_limits<double>::max_digits10) 
		<< calc("25 * 12 + 93 - 22 - 36 + -40 - -57 * 52 - 66 - 26 / -80 + 75 + -83 - -76 - 18 * 98 - 93 * -50 - 53 / 14 * -65 / -97 + -41 + -75 4 / -21 - 71 / -73 / 42 + 90 / -16 + -7") << std::endl;
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