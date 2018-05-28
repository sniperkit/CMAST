# Level 1 AST

In the `cmast::level1` namespace:

```c++
struct file {
    std::vector<file_element> elements;
};

struct command_invocation {
    identifier name;
    std::vector<argument> arguments;
};

struct identifier { std::string value; };

struct argument {
    std::variant<bracket_argument, quoted_argument, unquoted_argument> value;
};

struct bracket_argument { std::string value; };

struct quoted_argument { std::string value; };

struct unquoted_argument { std::string value; }

struct comment { std::string value; }
```

Dev note: TODO: source location information
