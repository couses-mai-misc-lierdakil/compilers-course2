#include <iostream>
#include <list>
#include <string>

int dtable(int s, char c) {
    if (s == 0 && (c == '+' || c == '-' || c == '*' || c == '/')) return 1;
    else if (
        s == 0 && ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'))
        return 2;
    else if (s == 0 && (c >= '0' && c <= '9')) return 3;
    else if (s == 2 && (
                 (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' ||
                 (c >= '0' && c <= '9'))) return 2;
    else if (s == 3 && (c >= '0' && c <= '9')) return 3;
    else if (s == 3 && c == '.') return 4;
    else if (s == 4 && (c >= '0' && c <= '9')) return 4;
    else if (s == 0 && (c == ' ' || c == '\t' || c == '\n')) return 0;
    else return -1;
}

int main()
{
    int state = 0;
    int curch_;
    std::list<char> buf;
    std::list<char> acc_buf;
    std::list<char> ret_buf;
    int la_state = -1;
    auto getchar = [&ret_buf]() {
        if (ret_buf.empty()) return std::getchar();
        else {
          char ch = ret_buf.front();
          ret_buf.pop_front();
          return static_cast<int>(ch);
        }
    };
    while ((curch_ = getchar()) != EOF) {
        char curch = static_cast<char>(curch_);
        buf.push_back(curch);
        state = dtable(state, curch);
        if (state > 0) {
            la_state = state;
            acc_buf.splice(acc_buf.end(), buf);
        } else if (state == -1) {
            if (la_state == 1) {
                std::string op(acc_buf.begin(), acc_buf.end());
                std::cout << "Operator " << op << std::endl;
            } else if (la_state == 2) {
                std::string var(acc_buf.begin(), acc_buf.end());
                std::cout << "Variable " << var << std::endl;
            } else if (la_state == 3 || la_state == 4) {
                std::string num(acc_buf.begin(), acc_buf.end());
                std::cout << "Number " << num << std::endl;
            }
            state = 0;
            acc_buf.clear();
            ret_buf.splice(ret_buf.begin(), buf);
        } else if (state == 0) {
            buf.clear();
        }
    }
    return 0;
}
