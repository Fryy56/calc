#include <iostream>
#include <iomanip>
#include <string>
#include "calc.hpp"

int main() {
	std::string h;
	std::cin >> h;
	if (auto res = Calc::calc_once(h))
		std::cout << std::fixed << std::setprecision(6) << res.value();
	else
		std::cout << "Error";

	return 0;
}