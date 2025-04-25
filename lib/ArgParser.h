#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>

namespace ArgumentParser {

class StringArg {
public:
    std::string long_name;
    char short_name;
    bool use_short_name;
    std::string value;
    bool use_value;
    std::string default_value;
    std::string description;
    bool is_multi;
    bool is_positional;
    std::string* external_value_ptr;

    StringArg()
        : short_name('\0'), use_short_name(false), use_value(false),
          is_multi(false), is_positional(false), external_value_ptr(nullptr) {}
};

class IntArg {
public:
    std::string long_name;
    char short_name;
    bool use_short_name;
    int value;
    bool use_value;
    int default_value;
    std::string description;
    bool is_multi;
    bool is_positional;
    size_t min_count;
    std::vector<int>* values_ptr;
    int* external_value_ptr;

    IntArg()
        : short_name('\0'), use_short_name(false), value(0), use_value(false),
          default_value(0), is_multi(false), is_positional(false),
          min_count(0), values_ptr(nullptr), external_value_ptr(nullptr) {}
};

class FlagArg {
public:
    std::string long_name;
    char short_name;
    bool use_short_name;
    bool value;
    bool default_value;
    std::string description;
    bool* external_value_ptr;

    FlagArg()
        : short_name('\0'), use_short_name(false), value(false),
          default_value(false), external_value_ptr(nullptr) {}
};

class HelpArg {
public:
    std::string long_name;
    char short_name;
    bool use_short_name;
    std::string description;

    HelpArg()
        : short_name('\0'), use_short_name(false) {}
};

class ArgParser {
private:
    std::string description_pars;
    std::vector<StringArg> string_arr;
    std::vector<IntArg> int_arr;
    std::vector<FlagArg> flag_arr;
    std::vector<std::string> position_args;
    HelpArg help_struct;
    bool help_flag;

    bool is_num(std::string num);
    void set_default_value();
    void reset_values();

public:
    ArgParser(std::string pars_name);

    ArgParser& AddStringArgument(std::string name);
    ArgParser& AddStringArgument(char short_name, std::string name, std::string description = "");
    ArgParser& AddIntArgument(std::string name, std::string description = "");
    ArgParser& AddIntArgument(char short_name, std::string name, std::string description = "");
    ArgParser& AddFlag(std::string name);
    ArgParser& AddFlag(char short_name, std::string name, std::string description = "");
    void AddHelp(char short_name, std::string name, std::string description);
    ArgParser& MultiValue(size_t x = 0);
    ArgParser& Default(bool flag);
    ArgParser& Default(std::string s);
    ArgParser& Positional();
    ArgParser& StoreValue(int& x);
    ArgParser& StoreValue(std::string& s);
    ArgParser& StoreValue(bool& flag);
    ArgParser& StoreValues(std::vector<int>& vault);
    bool Parse(std::vector<std::string> args);
    std::string GetStringValue(std::string name);
    int GetIntValue(std::string name, int index = -1);
    bool GetFlag(std::string name);
    bool Help();
    void PrintHelp();
};

} 