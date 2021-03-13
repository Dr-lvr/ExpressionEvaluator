#include <stack>
#include <string>
#include <iostream>
#include <unordered_map>

std::string manageNegativeNumbers(std::string expression) {

	std::string exp;
	std::stack<char> stack;
	for (auto& c : expression) {
		if (c == '-') {
			if (stack.empty() || std::ispunct(stack.top())) {
				stack.push('0');
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
	return exp;
}
double calc(std::string expression) {

	expression = manageNegativeNumbers(expression);
	std::string stringSt;
	std::stack<std::string> stack;
	std::deque<std::string> postFix;
	std::unordered_map <char, int> opMap = { {'+', 1},{'-', 1},{'*', 2},{'/', 2} };
	for (size_t i = 0; i < expression.size(); ++i) {
		while (std::isdigit(expression.at(i))) {
			stringSt += expression.at(i);
			++i;
			if (i == expression.size()) {
				break;
			}
		}
		if (!stringSt.empty()) {
			postFix.push_back(stringSt);
			stringSt.erase(stringSt.begin(), stringSt.end());
		}
		if (i == expression.size()) {
			break;
		}//end operand found
		switch (expression.at(i)) {
		case '(':
			stack.push("(");
			break;
		case ')':
			while (!stack.empty() && stack.top() != "(") {
				postFix.push_back(stack.top());//<-------
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
					postFix.push_back(stack.top()); //<-------
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
	std::cout << calc("3 -(-1)");
	std::cin.ignore();
}