#include <iostream>
#include <iomanip>
#include <vector>

using std::cout, std::cin;

// Custom structures declarations
struct Stack {
    double contents;
    Stack* next;
};

struct Conv_Stack {
    char contents;
    Conv_Stack* next;
};

// Global data structures declarations
Conv_Stack* conv_st = nullptr;
Stack* st = nullptr;
std::vector <std::string> postfix;

// Stack related functions
void conv_push(char a) { // No dependencies | Calls to malloc
    Conv_Stack* new_st = (Conv_Stack*)malloc(sizeof(Conv_Stack));
    new_st -> contents = a;
    new_st -> next = conv_st;
    conv_st = new_st;
}

char conv_pop() { // No dependencies
    if (conv_st == nullptr) { 
        printf("Error.");
        exit(0);
    }
    char output = conv_st -> contents;
    Conv_Stack* temp_ptr = conv_st -> next;
    free(conv_st);
    conv_st = temp_ptr;
    return output;
}

void push(double a) { // No dependencies | Calls to malloc
    Stack* new_st = (Stack*)malloc(sizeof(Stack));
    new_st -> contents = a;
    new_st -> next = st;
    st = new_st;
}

double pop() { // No dependencies
    if (st == nullptr) { 
        printf("Error.");
        exit(0);
    }
    double output = st -> contents;
    Stack* temp_ptr = st -> next;
    free(st);
    st = temp_ptr;
    return output;
}

// Extra functions
int prec(char a) { // No dependencies
    int precedence;
    switch (a) {
        case '+':
        case '-':
            precedence = 0;
            break;
        case '*':
        case '/':
            precedence = 1;
            break;
        default:
            printf("Unknown operator: %c", a);
            exit(0);
    }

    return precedence;
}

std::string parse(std::string &a) { // No dependencies
    std::string parsed_input = "";
    // Check for unary minus
    for (int i = 0; i < a.size() - 1; i++) {
        if (a[i] == '(' && a[i + 1] == '-') {
            parsed_input += "(0";
            continue;
        } else if (i == 0 && a[i] == '-')
            parsed_input += "0";
        parsed_input += a[i];
    }
    parsed_input += a[a.size() - 1];

    // Check for parenthesis correctness
    int paren = 0;
    for (char i : parsed_input) {
        if (i == '(')
            paren++;
        else if (i == ')')
            paren--;
        
        if (paren < 0) {
            cout << "Error.";
            exit(0);
        }
    }
    if (paren != 0) {
        cout << "Error.";
        exit(0);
    }

    return parsed_input;
}

// Conversion
void conversion(std::string &input) { // 1 dependency - prec
    std::string cur_num = "";
    for (char cur_char : input)
        if ('0' <= cur_char && cur_char <= '9') // If it's an operand
            cur_num += cur_char;
        else { // If it's an operator
            // Add what number we have
            if (cur_num != "") {
                postfix.push_back(cur_num);
                cur_num = "";
            }

            // Handle parenthesis
            if (cur_char == '(') {
                conv_push(cur_char);
                continue;
            }
            
            if (cur_char == ')') {
                while (conv_st -> contents != '(')
                    postfix.push_back(std::string(1, conv_pop()));
                conv_pop();
                continue;
            }

            // Handle other operands
            if (conv_st == nullptr || conv_st -> contents == '(' || prec(cur_char) > prec(conv_st -> contents))
                conv_push(cur_char);
            else {
                while (conv_st != nullptr && conv_st -> contents != '(' && prec(cur_char) <= prec(conv_st -> contents))
                    postfix.push_back(std::string(1, conv_pop()));
                conv_push(cur_char);
            }
        }

    // Add what we have (again)
    if (cur_num != "") 
        postfix.push_back(cur_num);
    // Clear the conversion stack (finalize the conversion)
    while (conv_st != nullptr)
        postfix.push_back(std::string(1, conv_pop()));;
}

// Main

int main() {
    std::string input;
    cin >> input;
    input = parse(input);
    conversion(input);

    // Calculating
    for (std::string i : postfix) { //! UPDATE OPERATORS HERE WHEN ADDING NEW ONES TO prec()
        double num1;
        double num2;

        switch (i[0]) {
            case '+':
                num2 = pop();
                num1 = pop();
                push(num1 + num2);
                break;
            case '-':
                num2 = pop();
                num1 = pop();
                push(num1 - num2);
                break;
            case '*':
                num2 = pop();
                num1 = pop();
                push(num1 * num2);
                break;
            case '/':
                num2 = pop();
                num1 = pop();
                if (num2 != 0) // Handle division by 0
                    push(num1 / num2);
                else {
                    cout << "Error.";
                    exit(0);
                }
                break;
            default:
                push(double(stoi(i)));
        }
    }

    cout << std::fixed << std::setprecision(6) << pop();
    return 0;
}