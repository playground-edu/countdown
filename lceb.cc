#include<vector>
#include<stdexcept>
#include<iostream>
#include<array>
#include<cstdlib>
#include<string>
#include<format>

using op = std::variant<long,char> ;

const std::array<char,4> ops{'+','-','*','/'} ;

struct Stack {
	using stack = std::vector<long> ;
	using equtn = std::vector<op>;

	equtn equtn_{};
	stack stack_{};

	long target_{};
	long best_{};
	size_t best_n_{};
	bool fast_{};

	Stack(long target, bool fast) : target_{target}, best_{target}, fast_{fast} {}

	std::tuple<long,bool> update(const op& v_op) {
		if (const char* v = std::get_if<char>(&v_op)) {
			if (size() < 2) {
				print(std::cout);
				std::cout << std::endl;
				throw std::runtime_error("stack has too few elements");
			}
			long a = stack_.back();
			stack_.pop_back();
			long b = stack_.back();
			stack_.pop_back();
			switch(*v) {
				case '+':
					
					stack_.push_back(b+a);
					return {b+a,true} ;
				case '-':
					stack_.push_back(b-a);
					return {b-a,true} ;
				case '*':
					stack_.push_back(b*a);
					return {b*a,true} ;
				case '/':
					{
						long c = b / a ;
						if (b % a != 0) {
							stack_.push_back(b);
							stack_.push_back(a);
							return {0,false};
						}
						stack_.push_back(c);
						return {c,true};
					}
					break;
				default:
					throw std::runtime_error("invalid operator");
			};
		} else {
			stack_.push_back(std::get<long>(v_op));
			return {0,true};
		}
	}

	auto push(const op& v_op) {
		// push new value or operator and if operator execute it if it can.
		auto v = update(v_op);
		if (std::get<bool>(v)) {
			equtn_.push_back(v_op);
		}
		return v ;
	}

	void pop() {
		// pop last value or operator and recalculate the stack from scratch
		equtn_.pop_back();
		stack_.clear();
		for(auto r = equtn_.begin(); r != equtn_.end(); ++r) {
			update(*r);
		}
	}

	void print(std::ostream& out) const {
		out << '(' ;
		for(auto r = equtn_.begin(); r != equtn_.end(); ++r) {
			if (const char* v = std::get_if<char>(&(*r))) {
				out << ' ' << *v ;	
			} else {
				out << ' ' << std::get<long>(*r) ;
			}
		}
		out << " )";
	}

	size_t size() const {
		return stack_.size();
	}

	size_t num() const {
		return equtn_.size();
	}
};

std::ostream& operator << (std::ostream& os, const Stack& s) {
	s.print(os) ;
	return os ;
}

void solve(Stack& s, std::vector<std::tuple<long,bool>>& n) {
	if (s.size() > 1) {
		for(auto r = ops.begin(); r != ops.end(); r++) {
			auto q = s.push(*r) ;
			if (std::get<bool>(q)) {
				if (auto d = std::labs(std::get<long>(q)-s.target_); d<s.best_ || d<=s.best_ && s.num() < s.best_n_) {
					s.best_ = d ;
					s.best_n_ = s.num() ;
					//if (d < 100) {
					std::cout << std::format("{0:6}", d) << ": " << s << std::endl;
					//}
					if (d == 0 && s.fast_) {
						exit(0) ;
					}
				}
				solve(s, n) ;
				s.pop();
			}
		}
	}
	for(auto r = n.begin(); r != n.end(); r++) {
		if (std::get<bool>(*r)) {
			s.push(std::get<long>(*r)) ;
			std::get<bool>(*r) = false ;
			solve(s, n);
			std::get<bool>(*r) = true ;
			s.pop();
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

	Stack s{std::atoi(v[i]),fast};

	std::cout << "Obtain " << s.target_ << ", using" ;

	while(++i < c) {
		n.push_back({std::atoi(v[i]),true});
		std::cout << " " << std::get<long>(n.back());
	}

 	std::cout << std::endl ;

	solve(s, n);
}
