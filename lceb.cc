#include<vector>
#include<stdexcept>
#include<iostream>
#include<array>
#include<cstdlib>
#include<string>
#include<format>

using op = std::variant<long,char> ;
using expression = std::vector<op>;
using stack = std::vector<long> ;
using numbers = std::vector<std::tuple<long,bool>>;

struct Status {
	long target{};
	long best{};
	size_t best_n{};
	bool fast{};
	expression postfix_expression{};
	stack postfix_stack{};
	numbers number_list;

	Status(long pTarget, numbers& n, bool pFast) : target{pTarget}, best{pTarget}, number_list{n}, fast{pFast} {}

	void print(std::ostream& out) const {
		out << std::format("{0:6}", best) << ": " ;
		out << '(' ;
		for(auto const r : postfix_expression) {
			if (const char* v = std::get_if<char>(&r)) {
				out << ' ' << *v ;	
			} else {
				out << ' ' << std::get<long>(r) ;
			}
		}
		out << " )";
	}

	void progress(std::ostream& out) {
		if (auto d = std::labs(postfix_stack.back()-target); d<best || d<=best && postfix_expression.size() < best_n) {
			best = d ;
			best_n = postfix_expression.size() ;
			print(out);
			out << '\n';
			if (d == 0 && fast) {
				exit(0) ;
			}
		}
	}
};

std::ostream& operator<<(std::ostream& out, Status const& status) {
	status.print(out);
	out << std::endl ;
	return out ;
}

void solve(Status& status) {
	if (status.postfix_stack.size() > 1) {
		auto a = status.postfix_stack.back();
		status.postfix_stack.pop_back();
		auto b = status.postfix_stack.back();
		status.postfix_stack.pop_back();

		status.postfix_stack.push_back(a+b);
		status.postfix_expression.push_back('+');
		status.progress(std::cout) ;
		solve(status) ;
		status.postfix_stack.pop_back();
		status.postfix_expression.pop_back();

		status.postfix_stack.push_back(a*b);
		status.postfix_expression.push_back('*');
		status.progress(std::cout) ;
		solve(status) ;
		status.postfix_stack.pop_back();
		status.postfix_expression.pop_back();

		if (a<b) {	
			status.postfix_stack.push_back(b-a);
			status.postfix_expression.push_back('-');
			status.progress(std::cout) ;
			solve(status) ;
			status.postfix_stack.pop_back();
			status.postfix_expression.pop_back();
		}

		if (b % a != 0) {
			status.postfix_stack.push_back(b/a);
			status.postfix_expression.push_back('/');
			status.progress(std::cout) ;
			solve(status) ;
			status.postfix_stack.pop_back();
			status.postfix_expression.pop_back();
		}

		status.postfix_stack.push_back(b);
		status.postfix_stack.push_back(a);
	}

	for(auto r = status.number_list.begin(); r != status.number_list.end(); ++r) {
		if (std::get<bool>(*r)) {
			status.postfix_stack.push_back(std::get<long>(*r)) ;
			status.postfix_expression.push_back(std::get<long>(*r));
			std::get<bool>(*r) = false ;
			solve(status);
			std::get<bool>(*r) = true ;
			status.postfix_stack.pop_back();
			status.postfix_expression.pop_back();
		}
	}
}

int main(int c, char const** v)
{
	std::vector<std::tuple<long,bool>> n ;
	std::string const fast_flag{"-f"};
	bool fast=false;

	auto i = 1;

	if (c < 3) { 
		std::cerr << "usage: " << v[0] << "[-f] <target> <num1> ... <numN>\n\t-f  stop on first exact match" << std::endl;
		return 1;
	}

	if (fast_flag.compare(v[i]) == 0) {
		fast = true ;
		++i;
	}

	auto target = std::atoi(v[i]);

	std::cout << "Obtain " << target << ", using" ;

	while(++i < c) {
		n.push_back({std::atoi(v[i]),true});
		std::cout << " " << std::get<long>(n.back());
	}

 	std::cout << std::endl ;

	Status s{target, n, fast};

	solve(s);
}
