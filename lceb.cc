#include<vector>
#include<stdexcept>
#include<iostream>
#include<array>
#include<cstdlib>
#include<string>
#include<format>
#include<ranges>

using number = long long ;
using numbers = std::vector<number> ;
using marked_numbers = std::vector<std::tuple<number,bool>>;
using op = std::variant<number,char> ;
using expression = std::vector<op>;

struct State {
        number target{};
        number best{};
        size_t best_n{};
        bool fast{};
        expression postfix_expression{};
        numbers postfix_stack{};
        marked_numbers number_list;
        long long num_iter{0};
        std::ostream& out;

        State(number pTarget, numbers const& n, bool pFast, std::ostream& out=std::cout) : target{pTarget}, best{pTarget}, fast{pFast}, out{out} {
            for(auto num : n) {
                number_list.push_back({num,true});
            }
            std::ranges::sort(number_list, [](auto a, auto b) { return std::get<number>(a) < std::get<number>(b); });
        }

        void print() const {
                out << '(' ;
                for(auto const r : postfix_expression) {
                        if (const char* v = std::get_if<char>(&r)) {
                                out << ' ' << *v ;        
                        } else {
                                out << ' ' << std::get<number>(r) ;
                        }
                }
                out << " )";
        }

        void check() {
                if (auto d = std::llabs(postfix_stack.back()-target); d<best || d<=best && postfix_expression.size() < best_n) {
                        best = d ;
                        best_n = postfix_expression.size() ;
                        out << std::format("{0:<14}", num_iter) << ' ' << std::format("{0:12}", best) << ": " ;
                        print();
                        out << '\n';
                        if (d == 0 && fast) {
                                exit(0) ;
                        }
                }
        }
};

std::ostream& operator<<(std::ostream& out, State const& state) {
        state.print() ;
        out << std::endl ;
        return out ;
}

void solve(State& state) {
        auto const do_op=[&state](char op, number res) {
             state.postfix_expression.push_back(op);
             state.postfix_stack.push_back(res);
             state.check() ;
             solve(state) ;
             state.postfix_stack.pop_back();
             state.postfix_expression.pop_back();
        } ;

        ++state.num_iter ;

        if (state.postfix_stack.size() > 1) {
                auto const a = state.postfix_stack.back();
                state.postfix_stack.pop_back();
                auto const b = state.postfix_stack.back();
                state.postfix_stack.pop_back();

                do_op('+', a+b);

                if (a > 1 && b > 1 && a+b != a*b) {
                        do_op('*', a*b);
                }

                if (a < b && b-a != a) {
                        do_op('-', b-a);
                }

                if (a > 1 && (b % a == 0) && b/a != a) {
                        do_op('/', b/a);
                }

                state.postfix_stack.push_back(b);
                state.postfix_stack.push_back(a);
        }

        number num = 0 ;
        for(auto r = state.number_list.begin(); r != state.number_list.end(); ++r) {
                if (std::get<bool>(*r) && std::get<number>(*r) != num) {
                        num = std::get<number>(*r);
                        state.postfix_stack.push_back(num) ;
                        state.postfix_expression.push_back(num);
                        std::get<bool>(*r) = false ;
                        solve(state);
                        std::get<bool>(*r) = true ;
                        state.postfix_stack.pop_back();
                        state.postfix_expression.pop_back();
                }
        }
}

int main(int c, char const** v)
{
        numbers n ;
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

        auto target = std::atoll(v[i]);

        while(++i < c) {
                if (auto num = std::atoll(v[i]); num > 0) {
                    n.push_back(num);
                }
        }

        std::cout << "Obtain " << target << ", using " ;
        std::ranges::copy(n, std::ostream_iterator<long>(std::cout, " "));
        std::cout << std::endl ;

        State s{target, n, fast};

        solve(s);
        std::cout << "Number of iterations: " << s.num_iter << std::endl;
}
