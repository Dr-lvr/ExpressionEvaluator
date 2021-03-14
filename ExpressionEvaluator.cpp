#include <stack>
#include <string>
#include <iostream>
#include <unordered_map>

std::string manageNegativeNumbers(std::string expression) {
	std::string exp;
	std::stack<char> stack;
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
		if (c != ' ') stack.push(c);
	}
	while (!stack.empty()) {
		exp.push_back(stack.top());
		stack.pop();
	}
	std::reverse(exp.begin(), exp.end());
	std::string stringSt;
	std::vector<std::string> postFix;
	for (size_t i = 0; i < exp.size(); ++i) {
		while (std::isdigit(exp.at(i)) || exp.at(i) == '.') {
			stringSt += exp.at(i);
			++i;
			if (i == exp.size()) break;
		}
		if (!stringSt.empty()) {
			postFix.push_back(stringSt);
			stringSt.erase(stringSt.begin(), stringSt.end());
		}
		if (i == exp.size()) break;
		postFix.push_back(std::string(1, exp.at(i)));
	}
	for (size_t i = 0; i < postFix.size(); ++i) {
		if (postFix.at(i) == " ") {
			postFix.at(i) = "(";
			for (size_t j = i + 1; j < postFix.size(); ++j) {
				if (postFix.at(j).at(0) == '(') {
					while (postFix.at(j).at(0) != ')') {
						++j;
					}
					if (j < postFix.size() - 1) postFix.insert(postFix.begin() + (j + 1), std::string(1, ')'));
					else postFix.push_back(std::string(1, ')'));
					break;
				}
				if (std::isdigit(postFix.at(j).at(0)) && postFix.at(j).at(0) != '0') {
					if (j < postFix.size() - 1) postFix.insert(postFix.begin() + (j + 1), std::string(1, ')'));
					else postFix.push_back(std::string(1, ')'));
					break;
				}
			}
		}
	}
	stringSt.erase(stringSt.begin(), stringSt.end());
	//------------------------------
	std::cout << " qui ------>";
	for (auto& c : postFix) {
		stringSt += c;
	}
	std::cout << "|";
	std::cout << std::endl;
	//---------------------------------
	return stringSt;
}
double calc(std::string expression) {

	std::string stringSt;
	std::stack<std::string> stack;
	std::deque<std::string> postFix;
	std::unordered_map <char, int> opMap = { {'+', 1},{'-', 1},{'*', 2},{'/', 2} };
	expression = manageNegativeNumbers(expression);
	for (size_t i = 0; i < expression.size(); ++i) {
		while (std::isdigit(expression.at(i)) || expression.at(i) == '.') {
			stringSt += expression.at(i);
			++i;
			if (i == expression.size()) break;
		}
		if (!stringSt.empty()) {
			postFix.push_back(stringSt);
			stringSt.erase(stringSt.begin(), stringSt.end());
		}
		if (i == expression.size()) break;//end operand found
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
		postFix.push_back(stack.top());//<-------
		stack.pop();
	}//end conversion into postfix
	///---------------------------------------------
	for (auto& c : postFix) {
		std::cout << c;
	}
	std::cout << std::endl;
	///---------------------------------------------
	double a = 0; double b = 0; double c = 0;
	while (!postFix.empty()) {
		if (!std::ispunct(postFix.front().at(0))) {
			stack.push(postFix.front());
		}
		else {
			a = std::stod(stack.top());
			stack.pop();
			b = std::stod(stack.top());
			stack.pop();
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
			stack.push(std::to_string(c));
		}
		postFix.pop_front();
	}
	return std::stod(stack.top());
}
int main() {
	std::cout << "prima operazione --- scritta correttamente ---> (0-7) * (0-(6 / 3)): " << std::endl;
	std::cout << calc("(0-7) * (0-(6 / 3))") <<std::endl;
	std::cout << "seconda operazione --- scritta correttamente ---> -7 * -(6 / 3)" << std::endl;
	std::cout << calc("-7 * -(6 / 3)");
	std::cin.ignore();
}