#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
// #include <iomanip>

// Global Error variable
int ERROR = 0;

// Error codes
const int IS_NOT_NUM              = 1;
const int IS_NOT_STRING           = 2;
const int IS_NOT_LIST             = 3;
const int IS_NOT_TREE             = 4;
const int IS_NOT_FUNCTION         = 5;
const int MISMATCHED_TYPES        = 6;
const int NO_MATCHING_OPERATOR    = 7;
const int STACK_EMPTY             = 8;
const int NON_EXISTANT_FUNCTION   = 9;
const int INDEX_OUT_OF_BOUNDS     = 10;
const int VARIABLE_DOES_NOT_EXIST = 11;

// Types
const int NIL_TYPE      = 0;
const int NUM_TYPE      = 1;
const int STRING_TYPE   = 2;
const int LIST_TYPE     = 3;
const int TREE_TYPE     = 4;
const int FUNCTION_TYPE = 5;


std::string Error() {
    if (ERROR == IS_NOT_NUM) {
        return "Not a Num";
    } else if (ERROR == IS_NOT_STRING) {
        return "Not a String";
    } else if (ERROR == IS_NOT_LIST) {
        return "Not a List";
    } else if (ERROR == IS_NOT_FUNCTION) {
        return "Tried to call a non-Function";
    } else if (ERROR == MISMATCHED_TYPES) {
        return "Mismatched types";
    } else if (ERROR == NO_MATCHING_OPERATOR) {
        return "No matching operator";
    } else if (ERROR == IS_NOT_TREE) {
        return "Not a Tree";
    } else if (ERROR == STACK_EMPTY) {
        return "The stack is empty";
    } else if (ERROR == NON_EXISTANT_FUNCTION) {
        return "Tried to call a Function that does not exist";
    } else if (ERROR == INDEX_OUT_OF_BOUNDS) {
        return "Requested index of List is out of bounds";
    } else {
        return "";
    }
}

// Is number?
bool is_number(const std::string &str) {    
  return !str.empty() && str.find_first_not_of("-.0123456789") == std::string::npos;
}

// Map garbage
template <typename Map>
bool map_compare (Map const &lhs, Map const &rhs) {
    return lhs.size() == rhs.size()
        && std::equal(lhs.begin(), lhs.end(),
                      rhs.begin());
}

template <typename Map>
bool map_less (Map const &lhs, Map const &rhs) {
    return lhs.size() < rhs.size()
        && std::equal(lhs.begin(), lhs.end(),
                      rhs.begin());
}

template<typename Char, typename Traits, typename Allocator>
std::basic_string<Char, Traits, Allocator> operator *
(const std::basic_string<Char, Traits, Allocator> s, size_t n)
{
   std::basic_string<Char, Traits, Allocator> tmp = s;
   for (size_t i = 0; i < n; ++i)
   {
      tmp += s;
   }
   return tmp;
}

template<typename Char, typename Traits, typename Allocator>
std::basic_string<Char, Traits, Allocator> operator *
(size_t n, const std::basic_string<Char, Traits, Allocator>& s)
{
   return s * n;
}

// Function
class Function {
private:
    std::string instructions;

public:
    Function() {
        this->instructions = "";
    }

    Function(std::string s) {
        this->instructions = s;
    }

    void Set(std::string s) {
        this->instructions = s;
    }

    std::string Get() {
        return this->instructions;
    }

    friend Function operator+(Function lhs, Function rhs)
    {
        lhs.Set(lhs.Get() + rhs.Get());
        return lhs;
    }

    friend bool operator==(Function lhs, Function rhs)
    {
        return lhs.instructions == rhs.instructions;
    }

    friend bool operator<(Function lhs, Function rhs)
    {
        return lhs.instructions < rhs.instructions;
    }
};

// Object
class Object {
public:
    int type;
    double num;
    std::string string;
    std::vector<Object> list;
    std::map<Object, Object> tree;

    Function function;

    Object() {
        this->type = 0;

        this->num = 0;
        this->string = "";
        this->list = std::vector<Object>();
        this->function = Function();
    }

    Object(double n) {
        this->type = NUM_TYPE;

        this->num = n;
        this->string = "";
        this->list = std::vector<Object>();
        this->function = Function();
    }

    Object(std::string s) {
        this->type = STRING_TYPE;

        this->num = 0;
        this->string = s;
        this->list = std::vector<Object>();
        this->function = Function();
    }

    Object(std::vector<Object> l) {
        this->type = LIST_TYPE;

        this->num = 0;
        this->string = "";
        this->list = l;
        this->function = Function();
    }
    
    Object(std::map<Object, Object> t) {
        this->type = TREE_TYPE;

        this->num = 0;
        this->string = "";
        this->list = std::vector<Object>();
        this->function = Function();
        this->tree = t;
    }

    Object(Function f) {
        this->type = FUNCTION_TYPE;

        this->num = 0;
        this->string = "";
        this->list = std::vector<Object>();
        this->function = f;
    }

    int Type() {
        return this->type;
    }

    double Num() {
        if (this->type != NUM_TYPE) {
            ERROR = IS_NOT_NUM;
        }
        return this->num;
    }

    std::string String() {
        if (this->type != STRING_TYPE) {
            ERROR = IS_NOT_STRING;
        }
        return this->string;
    }

    std::vector<Object> List() {
        if (this->type != LIST_TYPE) {
            ERROR = IS_NOT_LIST;
        }
        return this->list;
    }

    Object GetIndex(int n) {
        if (this->type != LIST_TYPE) {
            ERROR = IS_NOT_LIST;
        }
        if (this->list.size() > n) {
            return this->list[n];
        } else {
            ERROR = INDEX_OUT_OF_BOUNDS;
            return Object();
        }
        // return this->list[n];
    }

    std::map<Object, Object> Tree() {
        if (this->type != TREE_TYPE) {
            ERROR = IS_NOT_TREE;
        }
        return this->tree;
    }
    
    Object GetMember(Object a) {
        if (this->type != TREE_TYPE) {
            ERROR = IS_NOT_TREE;
        }
        return this->tree[a];
    }

    Function Func() {
        if (this->type != FUNCTION_TYPE) {
            ERROR = IS_NOT_FUNCTION;
        }
        return this->function;
    }

    std::string Instruction() {
        return this->function.Get();
    }

    void Set(double n) {
        this->num = n;
        this->type = NUM_TYPE;
    }

    void Set(std::string s) {
        this->string = s;
        this->type = STRING_TYPE;
    }

    void Set(std::vector<Object> l) {
        this->list = l;
        this->type = LIST_TYPE;
    }

    void Append(Object a) {
        this->list.push_back(a);
        this->type = LIST_TYPE;
    }

    void Pop() {
        if (this->type != LIST_TYPE) {
            ERROR = IS_NOT_LIST;
        }
        this->list.pop_back();
        // this->type = LIST_TYPE;
    }

    void SetMember(Object a, Object b) {
        this->tree[a] = b;
        this->type = TREE_TYPE;
    }

    void Set(std::map<Object, Object> t) {
        this->tree = t;
        this->type = TREE_TYPE;
    }

    void Set(Function f) {
        this->function;
        this->type = FUNCTION_TYPE;
    }

    void Set(Object a) {
        *this = a;
    }

    void SetType(int type) {
        this->type = type;
    }

    friend Object operator+(Object lhs, Object rhs)
    {
        if (lhs.Type() == rhs.Type()) {
            switch (lhs.Type()) {
                case NUM_TYPE:
                    lhs.Set(lhs.Num() + rhs.Num());
                    break;
                case STRING_TYPE:
                    lhs.Set(lhs.String() + rhs.String());
                    break;
                case LIST_TYPE:
                    {
                        std::vector<Object> vector1 = lhs.List();
                        std::vector<Object> vector2 = rhs.List();
                        vector1.insert(vector1.end(), vector2.begin(), vector2.end());
                        lhs.Set(vector1);
                    }
                    break;
                case FUNCTION_TYPE:
                    lhs.Set(lhs + rhs);
                    break;
                default:
                    ERROR = NO_MATCHING_OPERATOR;
                    break;
            }
        } else {
            ERROR = MISMATCHED_TYPES;
        }
        return lhs;
    }

    friend Object operator-(Object lhs, Object rhs)
    {
        if (lhs.Type() == rhs.Type()) {
            switch (lhs.Type()) {
                case NUM_TYPE:
                    lhs.Set(lhs.Num() - rhs.Num());
                    break;
                default:
                    ERROR = NO_MATCHING_OPERATOR;
                    break;
            }
        } else {
            ERROR = MISMATCHED_TYPES;
        }
        return lhs;
    }

    friend Object operator*(Object lhs, Object rhs)
    {
        if (lhs.Type() == rhs.Type()) {
            switch (lhs.Type()) {
                case NUM_TYPE:
                    lhs.Set(lhs.Num() * rhs.Num());
                    break;
                default:
                    ERROR = NO_MATCHING_OPERATOR;
                    break;
            }
        } else if ((lhs.Type() == STRING_TYPE && rhs.Type() == NUM_TYPE ) || (lhs.Type() == NUM_TYPE && rhs.Type() == STRING_TYPE)) {
            lhs.Set(lhs * rhs);
        } else {
            ERROR = MISMATCHED_TYPES;
        }
        return lhs;
    }

    friend Object operator/(Object lhs, Object rhs)
    {
        if (lhs.Type() == rhs.Type()) {
            switch (lhs.Type()) {
                case NUM_TYPE:
                    lhs.Set(lhs.Num() / rhs.Num());
                    break;
                default:
                    ERROR = NO_MATCHING_OPERATOR;
                    break;
            }
        } else {
            ERROR = MISMATCHED_TYPES;
        }
        return lhs;
    }

    friend bool operator==(Object lhs, Object rhs)
    {
        return lhs.type == rhs.type && lhs.num == rhs.num && lhs.string == rhs.string && lhs.list == rhs.list && map_compare(lhs.tree, rhs.tree) && lhs.function == rhs.function;
    }

    friend bool operator!=(Object lhs, Object rhs)
    {
        return ! (lhs == rhs);
    }

    friend bool operator<(Object lhs, Object rhs)
    {
        return lhs.type < rhs.type || lhs.num < rhs.num || lhs.string < rhs.string || lhs.list < rhs.list || map_less(lhs.tree, rhs.tree) || lhs.function < rhs.function;
    }
};


template<typename T>
std::vector<T> slice(std::vector<T> v, int m, int n)
{
    auto first = v.cbegin() + m;
    auto last = v.cbegin() + n + 1;

    std::vector<T> vec(first, last);
    return vec;
}


std::string StringList(std::vector<Object> list);

std::string StringTree(std::map<Object, Object> tree) {
    std::string s = "{";
    if (tree.size() > 0) {
        for(auto value: tree) {
            s += value.first.string + ": ";
            switch (value.second.Type()) {
                case NUM_TYPE:
                    {
                        std::ostringstream strs;
                        strs << value.second.Num();
                        std::string str = strs.str();
                        s += str + ", ";
                    }
                    break;
                case STRING_TYPE:
                    s += value.second.String() + ", ";
                    break;
                case LIST_TYPE:
                    s += StringList(value.second.List()) + ", ";
                    break;
                case FUNCTION_TYPE:
                    s += value.second.Func().Get() + ", ";
                    break;
                case TREE_TYPE:
                    s += StringTree(value.second.Tree()) + ", ";
                    break;
            }
        }
        s.pop_back();
        s.pop_back();
    }
    s += "}";
    return s;
}

std::string StringList(std::vector<Object> list) {
    std::string s = "[";
    if (list.size() > 0) {
        for(auto value: list) {
            switch (value.Type()) {
                case NUM_TYPE:
                    {
                        std::ostringstream strs;
                        strs << value.Num();
                        std::string str = strs.str();
                        s += str + ", ";
                    }
                    break;
                case STRING_TYPE:
                    s += "\"" + value.String() + "\", ";
                    break;
                case LIST_TYPE:
                    s += StringList(value.List()) + ", ";
                    break;
                case FUNCTION_TYPE:
                    s += value.Func().Get() + ", ";
                    break;
                case TREE_TYPE:
                    s += StringTree(value.Tree()) + ", ";
                    break;
            }
        }
        s.pop_back();
        s.pop_back();
    }
    s += "]";
    return s;
}

std::string StringParams(std::vector<Object> list) {
    std::string s = "(";
    if (list.size() > 0) {
        for(auto value: list) {
            switch (value.Type()) {
                case NUM_TYPE:
                    {
                        std::ostringstream strs;
                        strs << value.Num();
                        std::string str = strs.str();
                        s += str + ", ";
                    }
                    break;
                case STRING_TYPE:
                    s += "\"" + value.String() + "\", ";
                    break;
                case LIST_TYPE:
                    s += StringList(value.List()) + ", ";
                    break;
                case FUNCTION_TYPE:
                    s += value.Func().Get() + ", ";
                    break;
                case TREE_TYPE:
                    s += StringTree(value.Tree()) + ", ";
                    break;
                default:
                    ERROR = MISMATCHED_TYPES;
                    break;
            }
        }
        s.pop_back();
        s.pop_back();
    }
    s += ")";
    return s;
}

void PrintObject(Object a) {
    switch (a.Type()) {
        case NUM_TYPE:
            std::cout << a.Num();
            break;
        case STRING_TYPE:
            std::cout << a.String();
            break;
        case LIST_TYPE:
            std::cout << StringList(a.List());
            break;
        case FUNCTION_TYPE:
            std::cout << a.Func().Get();
            break;
        case TREE_TYPE:
            std::cout << StringTree(a.Tree());
            break;
        default:
            ERROR = MISMATCHED_TYPES;
            break;
    }
}

// Operators
const Function Add        = Function("+");
const Function Sub        = Function("-");
const Function Mul        = Function("*");
const Function Div        = Function("/");
const Function Print      = Function("|");
const Function Input      = Function("$");
const Function Store      = Function("<");
const Function Load       = Function(">");
const Function Write      = Function(",");
const Function Read       = Function(".");
const Function Loop       = Function("&");
const Function Create     = Function("@");
const Function Call       = Function("!");
const Function Until      = Function("^");
const Function PopList    = Function("[");
const Function AppendList = Function("]");
const Function Index      = Function(":");
const Function Equals     = Function("=");
const Function Greater    = Function(">>");
const Function Less       = Function("<<");
const Function Exit       = Function(";");


std::vector<Object> Parse(std::string func) {
    std::vector<Object> tokens;

	int depth = 0;
	bool is_string = false;
	std::string token = "";

	for (int i = 0; i < func.length(); i++) {
		if (func[i] == '\'' && depth < 1) {
			if (is_string) {
				// fmt.Println(token)
				is_string = false;
				token += "\'";
				if (token == "''") {
					tokens.push_back(Object(token));
				} else {
					tokens.push_back(token.substr(1, token.length()-2));
				}
				token = "";
				continue;
			} else {
				is_string = true;
				tokens.push_back(Object(token));
				token = "";
			}
		}
		if (is_string) {
			token += func[i];
			if (i == func.length()-1) {
				tokens.push_back(Object(token));
			}
		} else {
			if (func[i] == ' ') {
				if (depth < 1) {
                    if (is_number(token)) {
                        tokens.push_back(Object(stod(token)));
                    } else {
                        tokens.push_back(Object(token));                        
                    }
					token = "";
				} else {
					token += ' ';
				}
			} else if (func[i] == '{') {
				if (depth < 1) {
                    if (is_number(token)) {
                        tokens.push_back(Object(stod(token)));
                    } else {
                        tokens.push_back(Object(token));                        
                    }
					token = "{";
				} else {
					token += '{';
				}
				depth += 1;
			} else if (func[i] == '}') {
				depth -= 1;
				token += '}';
				if (depth < 1) {
					tokens.push_back(Object(Function(token.substr(1, token.length()-2))));
					token = "";
				}
			} else {
				if (func[i] != '\n') {
					token += func[i];
				} else {
                    if (is_number(token)) {
                        tokens.push_back(Object(stod(token)));
                    } else {
                        tokens.push_back(Object(token));                        
                    }
					token = "";
				}
				if (i == func.length()-1) {
                    if (is_number(token)) {
                        tokens.push_back(Object(stod(token)));
                    } else {
                        tokens.push_back(Object(token));                        
                    }
				}
			}
		}
	}

    std::vector<Object> returned_tokens;
    for(auto elem: tokens) {
		if (elem == Object("")) {

		} else if (elem == Object("''")) {
			returned_tokens.push_back(elem);
		} else if (elem == Object("|")) {
			returned_tokens.push_back(Print);
		} else if (elem == Object("$")) {
			returned_tokens.push_back(Input);
		} else if (elem == Object("<")) {
			returned_tokens.push_back(Store);
		} else if (elem == Object(">")) {
			returned_tokens.push_back(Load);
		} else if (elem == Object("+")) {
			returned_tokens.push_back(Add);
		} else if (elem == Object("-")) {
			returned_tokens.push_back(Sub);
		} else if (elem == Object("*")) {
			returned_tokens.push_back(Mul);
		} else if (elem == Object("/")) {
			returned_tokens.push_back(Div);
		} else if (elem == Object(".")) {
			returned_tokens.push_back(Read);
		} else if (elem == Object(",")) {
			returned_tokens.push_back(Write);
		} else if (elem == Object("&")) {
			returned_tokens.push_back(Loop);
		} else if (elem == Object("@")) {
			returned_tokens.push_back(Create);
		} else if (elem == Object("!")) {
			returned_tokens.push_back(Call);
		} else if (elem == Object("^")) {
			returned_tokens.push_back(Until);
		} else if (elem == Object("[")) {
			returned_tokens.push_back(PopList);
		} else if (elem == Object("]")) {
			returned_tokens.push_back(AppendList);
		} else if (elem == Object(":")) {
			returned_tokens.push_back(Index);
		} else if (elem == Object("=")) {
			returned_tokens.push_back(Equals);
		} else if (elem == Object(">>")) {
			returned_tokens.push_back(Greater);
		} else if (elem == Object("<<")) {
			returned_tokens.push_back(Less);
		} else if (elem == Object(";")) {
			returned_tokens.push_back(Exit);
		} else {
			returned_tokens.push_back(elem);
		}
	}
    // std::cout << StringList(returned_tokens) << std::endl;
    return returned_tokens;
}


class Dynamite {
private:
    int pointer;
    std::vector<Object> stack;
    std::map<Object, Object> registers;
    std::vector<Object> instructions;
    std::vector<void*> library;

public:
    Dynamite() {
        this->stack = {};
        this->library = {};
        std::reverse(instructions.begin(), instructions.end());
        this->instructions = instructions;
    }

    Dynamite(std::vector<Object> instructions) {
        this->stack = {};
        this->library = {};
        std::reverse(instructions.begin(), instructions.end());
        this->instructions = instructions;
    }

    Dynamite(std::vector<Object> stack, std::vector<Object> instructions) {
        this->stack = stack;
        this->library = library;
        std::reverse(instructions.begin(), instructions.end());
        this->instructions = instructions;
    }

    Dynamite(std::vector<Object> stack, std::vector<Object> instructions, std::map<Object, Object> registers) {
        this->stack = stack;
        this->library = library;
        std::reverse(instructions.begin(), instructions.end());
        this->instructions = instructions;
        this->registers = registers;
    }

    std::vector<Object> GetStack() {
        return this->stack;
    }

    std::map<Object, Object> GetRegisters() {
        return this->registers;
    }

    void Eval() {
        Object a;
        std::vector<Object> save_stack;
        int n = 0;
        while (true) {
            n++;
            save_stack = stack;
            a = this->NextInstruction();
            if (a.Instruction() == "+") {
                this->Add();
            } else if (a.Instruction() == "-") {
                this->Sub();
            } else if (a.Instruction() == "*") {
                this->Mul();
            } else if (a.Instruction() == "/") {
                this->Div();
            } else if (a.Instruction() == "|") {
                this->Print();
            } else if (a.Instruction() == "$") {
                this->Input();
            } else if (a.Instruction() == "<") {
                this->Store();
            } else if (a.Instruction() == ">") {
                this->Load();
            } else if (a.Instruction() == ".") {
                this->Read();
            } else if (a.Instruction() == ",") {
                this->Write();
            } else if (a.Instruction() == "@") {
                this->Create();
            } else if (a.Instruction() == "!") {
                this->Call();
            } else if (a.Instruction() == "&") {
                this->Loop();
            } else if (a.Instruction() == "[") {
                this->PopList();
            } else if (a.Instruction() == "]") {
                this->AppendList();
            } else if (a.Instruction() == ":") {
                this->Index();
            } else if (a.Instruction() == "^") {
                this->Until();
            } else if (a.Instruction() == "=") {
                this->Equals();
            } else if (a.Instruction() == ">>") {
                this->Greater();
            } else if (a.Instruction() == "<<") {
                this->Less();
            } else if (a.Instruction() == ";") {
                break;
            } else {
                this->Push(a);
            }

            // std::cout << "(INSTRUCTION: " << a.Instruction() << ")" << std::endl;

            if (ERROR != 0) {
                if (a.Instruction() == "|") {
                    std::cout << std::endl << "  ===> Error: " << Error() << " in print with the arguments " << StringParams(save_stack) << " at instruction #" << n << std::endl;
                } else if (a.Instruction() == "$") {
                    std::cout << std::endl << "  ===> Error: " << Error() << " in input with the arguments " << StringParams(save_stack) << " at instruction #" << n << std::endl;
                } else if (a.Instruction() == "<") {
                    std::cout << std::endl << "  ===> Error: " << Error() << " error storing to variable with the arguments " << StringParams(save_stack) << " at instruction #" << n << std::endl;
                } else if (a.Instruction() == ">") {
                    std::cout << std::endl << "  ===> Error: " << Error() << " error loading from variable with the arguments " << StringParams(save_stack) << " at instruction #" << n << std::endl;
                } else if (a.Instruction() == "+") {
                    std::cout << std::endl << "  ===> Error: " << Error() << " in add with the arguments " << StringParams(save_stack) << " at instruction #" << n << std::endl;
                } else if (a.Instruction() == "-") {
                    std::cout << std::endl << "  ===> Error: " << Error() << " in subtract with the arguments " << StringParams(save_stack) << " at instruction #" << n << std::endl;
                } else if (a.Instruction() == "*") {
                    std::cout << std::endl << "  ===> Error: " << Error() << " in multiply with the arguments " << StringParams(save_stack) << " at instruction #" << n << std::endl;
                } else if (a.Instruction() == "/") {
                    std::cout << std::endl << "  ===> Error: " << Error() << " in divide with the arguments " << StringParams(save_stack) << " at instruction #" << n << std::endl;
                } else if (a.Instruction() == ">>") {
                    std::cout << std::endl << "  ===> Error: " << Error() << " in greater with the arguments " << StringParams(save_stack) << " at instruction #" << n << std::endl;
                } else if (a.Instruction() == "<<") {
                    std::cout << std::endl << "  ===> Error: " << Error() << " in less with the arguments " << StringParams(save_stack) << " at instruction #" << n << std::endl;
                } else if (a.Instruction() == "!") {
                    if (save_stack.size() > 0) {
                        std::cout << std::endl << "  ===> Error: " << Error() << " named '" << save_stack[0].String() << "' with the arguments " << StringParams(slice<Object>(save_stack, 1, save_stack.size()-1)) << " at instruction #" << n << std::endl;
                    } else {
                        std::cout << std::endl << "  ===> Error: " << Error() << " with the arguments " << StringParams(save_stack) << " at instruction #" << n << std::endl;
                    }
                } else {
                    std::cout << std::endl << "  ===> Error: " << Error() << " in " << a.Instruction() << " with the arguments " << StringParams(save_stack) << " at instruction #" << n << std::endl;
                }
                break;
            }
        }
    }

    Object NextInstruction() {
        if (int(this->instructions.size()) > 0) {
            Object a = this->instructions.back();
            this->instructions.pop_back();
            return a;
        } else {
            return Object(Function(";"));
        }
    }

    void Print() {
        Object a = this->Pop();
        PrintObject(a);
    } 

    void Input() {
        std::string s;
        std::getline(std::cin, s);
        this->Push(Object(s));
    }

    void Push(Object a) {
        this->stack.push_back(a);
    }

    Object Pop() {
        if (int(this->stack.size()) > 0) {
            Object a = this->stack.back();
            this->stack.pop_back();
            return a;
        } else {
            ERROR = STACK_EMPTY;
            return Object();
        }
    }

    void PopList() {
        Object list = this->Pop();
        list.Pop();
        this->Push(list);
    }

    void AppendList() {
        Object list = this->Pop();
        Object object = this->Pop();
        // PrintObject(object);
        list.Append(object);
        this->Push(list);
    }

    void Index() {
        Object list = this->Pop();
        Object index = this->Pop();
        this->Push(list.GetIndex(index.Num()));
    }

    void Write() {
        Object index = this->Pop();
        Object self = this->Pop();
        Object value = this->Pop();
        self.SetMember(index, value);
        this->Push(self);
    }

    void Read() {
        Object index = this->Pop();
        Object self = this->Pop();
        this->Push(self.GetMember(index));
    }
    
    void Create() {
        Object a = Object();
        a.SetType(TREE_TYPE);
        this->Push(a);
    }
    
    void Top() {
        Object dup = this->Pop();
        this->Push(dup);
        this->Push(dup);
    }

    void Store() {
        Object id = this->Pop();
        Object data = this->Pop();
        this->registers[id] = data;
    }

    void Load() {
        Object id = this->Pop();
        if (this->registers.count(id)) {
        } else {
            ERROR = VARIABLE_DOES_NOT_EXIST;
        }
        this->Push(this->registers[id]);
    }

    void Call() {
        if (this->GetStack().size() > 0) {
            Object function = this->Pop();
            Dynamite dynamite = Dynamite(this->GetStack(), Parse(function.Func().Get()), this->GetRegisters());
            dynamite.Eval();

            for(auto elem: dynamite.GetStack()) {
                this->Push(elem);
            }
        } else {
            ERROR = NON_EXISTANT_FUNCTION;
        }
    }

    void Call(bool share_registers) {
        if (this->GetStack().size() > 0) {
            Object function = this->Pop();

            Dynamite dynamite = Dynamite(this->GetStack(), Parse(function.Func().Get()), this->GetRegisters());
            dynamite.Eval();

            if (share_registers) {
                for(auto pair: dynamite.GetRegisters()) {
                    // this->Store(elem);
                    this->registers[pair.first] = pair.second;
                }
            }

            for(auto elem: dynamite.GetStack()) {
                this->Push(elem);
            }
        } else {
            ERROR = NON_EXISTANT_FUNCTION;
        }
    }

    void Loop() {
        Object condition = this->Pop();
        Object function = this->Pop();
        // std::cout << "Calling Condition";
        this->Push(condition);
        this->Call(true);
        while (this->Pop().Num() >= 1) {
            this->Push(function);
            //share the registers!
            this->Call(true);
            this->Push(condition);
            //share the registers!
            this->Call(true);
        }
    }

    void Until() {
        Object list = this->Pop();
        Object function = this->Pop();
        // std::cout << "Calling Condition";
        this->Push(list);
        while (this->Pop().List().size() >= 1) {
            this->Push(function);
            //share the registers!
            this->Call(true);

            this->Push(list);
            this->PopList();
            list = this->Pop();
            this->Push(list);
        }
    }

    void Add() {
        Object operand_a = this->Pop();
        Object operand_b = this->Pop();
        this->Push(operand_a + operand_b);
    }

    void Sub() {
        Object operand_a = this->Pop();
        Object operand_b = this->Pop();
        this->Push(operand_a - operand_b);
    }

    void Mul() {
        Object operand_a = this->Pop();
        Object operand_b = this->Pop();
        this->Push(operand_a * operand_b);
    }

    void Div() {
        Object operand_a = this->Pop();
        Object operand_b = this->Pop();
        this->Push(operand_a / operand_b);
    }

    void Equals() {
        Object operand_a = this->Pop();
        Object operand_b = this->Pop();
        this->Push(double(operand_a == operand_b));
    }

    void Greater() {
        Object operand_a = this->Pop();
        Object operand_b = this->Pop();
        this->Push(double(operand_a.Num() > operand_b.Num()));
    }

    void Less() {
        Object operand_a = this->Pop();
        Object operand_b = this->Pop();
        this->Push(double(operand_a.Num() < operand_b.Num()));
    }
};


int main() {
    // Dynamite dynamite = Dynamite(Parse("'Starting loop' | 1 n < {0 n <} {n >} & 'Finished!' |"));
    // Dynamite dynamite = Dynamite(Parse("'\n' endl < {| endl > |} println < 'Starting loop' println > ! 1 n < {0 n < 'This is one iteration!' println > !} {n >} & 'Finished!' println > !"));
    // Dynamite dynamite = Dynamite(Parse("@ list < 3 2 1 list > ] ] ] [ |"));
    // Dynamite dynamite = Dynamite(Parse("@ tree < @ list < 1 'string' list > ] ] tree > x , tree < tree > |"));
    // Dynamite dynamite = Dynamite(Parse("5 5.3 * |"));
    // Dynamite dynamite = Dynamite(Parse("'\n' endl < {| endl > |} println < 'Hello world!' println > !"));
    // Dynamite dynamite = Dynamite(Parse("2 1 << | 3 2 1 a !"));
    Dynamite dynamite = Dynamite(Parse(" { ] } push < { [ } pop < 1 @ push > ! list < 0 list > : | "));

    // Dynamite dynamite = Dynamite(Parse("'Hello world!\n' |"));
    // Dynamite dynamite = Dynamite({
    //                             //   Object("\n"),
    //                             //   Object("endl"),
    //                             //   Object(Function("<")),
    //                             //   Object("printing works!"),
    //                             //   Object("a"),
    //                             //   Object(Function("<")),

    //                             //   Object("a"),
    //                             //   Object(Function(">")),
    //                             //   Object(Function("|")),
    //                             //   Object("endl"),
    //                             //   Object(Function(">")),
    //                             //   Object(Function("|")),

    //                             //   Object(9),
    //                             //   Object("a"),
    //                             //   Object(Function("<")),
                                  
    //                             //   Object("a"),
    //                             //   Object(Function(">")),
    //                             //   Object(Function("|")),

    //                               Object(),
    //                               Object("var"),
    //                               Object(Store),

    //                               Object("Hello members!"),
    //                               Object("var"),
    //                               Object(Load),
    //                               Object("a"),
    //                               Object(Write),
    //                               Object("var"),
    //                               Object(Store),

    //                               Object("9"),
    //                               Object("var"),
    //                               Object(Load),
    //                               Object("a"),
    //                               Object(Read),
    //                               Object(Add),
    //                               Object(Print),
    //                             //   Object(Function("$")),
    //                             //   Object(Function("|")),
    //                             //   Object(Function("|")),

    //                               Object(Exit)
    //                               });
    dynamite.Eval();
    // Object a = Object(std::vector<Object>{std::vector<Object>{Object("Inside"), Object("List")}, Object("There")});
    // a.SetMember(Object("method"), Object(Function("|")));
    // PrintObject(a.GetMember(Object("method")))
    // PrintObject(a);
    // a.Set("Hello world!");
    // PrintObject(a);
    // a = a + a;
    // std::cout << a.String() << std::endl;

    // a.Set(2);
    // std::cout << a.Num() << std::endl;
    // a = a / a;
    // std::cout << a.Num() << std::endl;
    // a = Object(9) * Object(8);
    // std::cout << a.Num() << std::endl;

    // a.Set(std::vector<Object>({Object(9), Object("Hey!")}));

    // a.Set(Function("hey |"));
        
    // std::cout << Error() << std::endl;

    return 0;
}

