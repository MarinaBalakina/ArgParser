#include "ArgParser.h"

namespace ArgumentParser {

bool ArgParser::is_num(std::string num) {
    if (num.empty()) return false;
    size_t length = num.length();
    for (size_t i = 0; i < length; ++i) {
        if (!isdigit(num[i])) {
            return false;
        }
    }
    return true;
}

void ArgParser::set_default_value() {
    for (size_t i = 0; i < string_arr.size(); i++) {
        StringArg& arg = string_arr[i];
        if (!arg.default_value.empty()) {
            arg.value = arg.default_value;
            arg.use_value = true;
            if (arg.external_value_ptr != nullptr) {
                *(arg.external_value_ptr) = arg.default_value;
            }
        }
        else {
            arg.use_value = false;
        }
    }

    for (size_t i = 0; i < int_arr.size(); i++) {
        IntArg& arg = int_arr[i];
        if (arg.default_value != 0) {
            arg.value = arg.default_value;
            arg.use_value = true;
            if (arg.values_ptr != nullptr) {
                arg.values_ptr->push_back(arg.default_value);
            }
            if (arg.external_value_ptr != nullptr) {
                *(arg.external_value_ptr) = arg.default_value;
            }
        }
        else {
            arg.use_value = false;
        }
    }

    for (size_t i = 0; i < flag_arr.size(); i++) {
        FlagArg& arg = flag_arr[i];
        arg.value = arg.default_value;
        if (arg.external_value_ptr != nullptr) {
            *(arg.external_value_ptr) = arg.default_value;
        }
    }
}

void ArgParser::reset_values() {
    for (size_t i = 0; i < string_arr.size(); i++) {
        StringArg& arg = string_arr[i];
        arg.value = arg.default_value;
        arg.use_value = !(arg.default_value.empty());
        if (arg.external_value_ptr != nullptr) {
            *(arg.external_value_ptr) = arg.default_value;
        }
    }

    for (size_t i = 0; i < int_arr.size(); i++) {
        IntArg& arg = int_arr[i];
        arg.value = arg.default_value;
        arg.use_value = (arg.default_value != 0);
        if (arg.values_ptr != nullptr) {
            arg.values_ptr->clear();
            if (arg.default_value != 0) {
                arg.values_ptr->push_back(arg.default_value);
            }
        }
        if (arg.external_value_ptr != nullptr) {
            *(arg.external_value_ptr) = arg.default_value;
        }
    }

    for (size_t i = 0; i < flag_arr.size(); i++) {
        FlagArg& arg = flag_arr[i];
        arg.value = arg.default_value;
        if (arg.external_value_ptr != nullptr) {
            *(arg.external_value_ptr) = arg.default_value;
        }
    }

    help_flag = false;
    position_args.clear();
}

ArgParser::ArgParser(std::string pars_name) {
    description_pars = pars_name;
    help_flag = false;
}

ArgParser& ArgParser::AddStringArgument(std::string name) {
    StringArg arg;
    arg.long_name = name;
    arg.short_name = '\0';
    arg.use_short_name = false;
    arg.value = "";
    arg.use_value = false;
    arg.default_value = "";
    arg.description = "";
    arg.is_multi = false;
    arg.is_positional = false;
    arg.external_value_ptr = nullptr;
    string_arr.push_back(arg);
    return *this;
}

ArgParser& ArgParser::AddStringArgument(char short_name, std::string name, std::string description) {
    StringArg arg;
    arg.long_name = name;
    arg.short_name = short_name;
    arg.use_short_name = true;
    arg.value = "";
    arg.use_value = false;
    arg.default_value = "";
    arg.description = description;
    arg.is_multi = false;
    arg.is_positional = false;
    arg.external_value_ptr = nullptr;
    string_arr.push_back(arg);
    return *this;
}

ArgParser& ArgParser::AddIntArgument(std::string name, std::string description) {
    IntArg arg;
    arg.long_name = name;
    arg.short_name = '\0';
    arg.use_short_name = false;
    arg.value = 0;
    arg.use_value = false;
    arg.default_value = 0;
    arg.description = description;
    arg.is_multi = false;
    arg.is_positional = false;
    arg.min_count = 0;
    arg.values_ptr = nullptr;
    arg.external_value_ptr = nullptr;
    int_arr.push_back(arg);
    return *this;
}

ArgParser& ArgParser::AddIntArgument(char short_name, std::string name, std::string description) {
    IntArg arg;
    arg.long_name = name;
    arg.short_name = short_name;
    arg.use_short_name = true;
    arg.value = 0;
    arg.use_value = false;
    arg.default_value = 0;
    arg.description = description;
    arg.is_multi = false;
    arg.is_positional = false;
    arg.min_count = 0;
    arg.values_ptr = nullptr;
    arg.external_value_ptr = nullptr;
    int_arr.push_back(arg);
    return *this;
}

ArgParser& ArgParser::AddFlag(std::string name) {
    FlagArg arg;
    arg.long_name = name;
    arg.short_name = '\0';
    arg.use_short_name = false;
    arg.value = false;
    arg.default_value = false;
    arg.description = "";
    arg.external_value_ptr = nullptr;
    flag_arr.push_back(arg);
    return *this;
}

ArgParser& ArgParser::AddFlag(char short_name, std::string name, std::string description) {
    FlagArg arg;
    arg.long_name = name;
    arg.short_name = short_name;
    arg.use_short_name = true;
    arg.value = false;
    arg.default_value = false;
    arg.description = description;
    arg.external_value_ptr = nullptr;
    flag_arr.push_back(arg);
    return *this;
}

void ArgParser::AddHelp(char short_name, std::string name, std::string description) {
    help_struct.long_name = name;
    help_struct.short_name = short_name;
    help_struct.use_short_name = true;
    help_struct.description = description;
}

ArgParser& ArgParser::MultiValue(size_t x) {
    if (int_arr.size() > 0) {
        IntArg& arg = int_arr[int_arr.size() - 1];
        arg.is_multi = true;
        arg.min_count = x;
    }
    return *this;
}

ArgParser& ArgParser::Default(bool flag) {
    if (flag_arr.size() > 0) {
        FlagArg& arg = flag_arr[flag_arr.size() - 1];
        arg.default_value = flag;
        if (arg.external_value_ptr != nullptr) {
            *(arg.external_value_ptr) = flag;
        }
    }
    return *this;
}

ArgParser& ArgParser::Default(std::string s) {
    if (string_arr.size() > 0) {
        StringArg& arg = string_arr[string_arr.size() - 1];
        arg.default_value = s;
        if (arg.external_value_ptr != nullptr) {
            *(arg.external_value_ptr) = s;
        }
    }
    return *this;
}

ArgParser& ArgParser::Positional() {
    if (int_arr.size() > 0) {
        IntArg& arg = int_arr[int_arr.size() - 1];
        arg.is_positional = true;
    }
    return *this;
}

ArgParser& ArgParser::StoreValue(int& x) {
    if (int_arr.size() > 0) {
        IntArg& arg = int_arr[int_arr.size() - 1];
        arg.external_value_ptr = &x;
    }
    return *this;
}

ArgParser& ArgParser::StoreValue(std::string& s) {
    if (string_arr.size() > 0) {
        StringArg& arg = string_arr[string_arr.size() - 1];
        arg.external_value_ptr = &s;
    }
    return *this;
}

ArgParser& ArgParser::StoreValue(bool& flag) {
    if (flag_arr.size() > 0) {
        FlagArg& arg = flag_arr[flag_arr.size() - 1];
        arg.external_value_ptr = &flag;
    }
    return *this;
}

ArgParser& ArgParser::StoreValues(std::vector<int>& vault) {
    if (int_arr.size() > 0) {
        IntArg& arg = int_arr[int_arr.size() - 1];
        arg.values_ptr = &vault;
    }
    return *this;
}

bool ArgParser::Parse(std::vector<std::string> args) {
    reset_values();
    set_default_value();
    help_flag = false;

    for (size_t i = 1; i < args.size(); i++) {
        std::string current_arg = args[i];
        std::string name = "";
        std::string value = "";

        if (current_arg.length() >= 2 && current_arg.substr(0, 2) == "--") {
            size_t equal_pos = current_arg.find('=');
            if (equal_pos != std::string::npos) {
                name = current_arg.substr(2, equal_pos - 2);
                value = current_arg.substr(equal_pos + 1);
            }
            else {
                name = current_arg.substr(2);
            }

            if (name == help_struct.long_name) {
                PrintHelp();
                help_flag = true;
                return true;
            }

            bool find_flag = false;
            for (size_t j = 0; j < flag_arr.size(); j++) {
                FlagArg& arg = flag_arr[j];
                if (arg.long_name == name) {
                    arg.value = true;
                    if (arg.external_value_ptr != nullptr) {
                        *(arg.external_value_ptr) = true;
                    }
                    find_flag = true;
                    break;
                }
            }
            if (find_flag) continue;

            bool find_string = false;
            for (size_t j = 0; j < string_arr.size(); j++) {
                StringArg& arg = string_arr[j];
                if (arg.long_name == name) {
                    if (equal_pos == std::string::npos) {
                        if (i + 1 < args.size()) {
                            value = args[++i];
                        }
                        else {
                            std::cerr << "Error: Missing value for argument --" << name << "\n";
                            return false;
                        }
                    }
                    arg.value = value;
                    arg.use_value = true;
                    if (arg.external_value_ptr != nullptr) {
                        *(arg.external_value_ptr) = value;
                    }
                    find_string = true;
                    break;
                }
            }
            if (find_string) continue;

            bool find_int = false;
            for (size_t j = 0; j < int_arr.size(); j++) {
                IntArg& arg = int_arr[j];
                if (arg.long_name == name) {
                    if (equal_pos == std::string::npos) {
                        if (i + 1 < args.size()) {
                            value = args[++i];
                        }
                        else {
                            std::cerr << "Error: Missing value for argument --" << name << "\n";
                            return false;
                        }
                    }
                    if (!is_num(value)) {
                        std::cerr << "Error: Value for argument --" << name << " must be an integer.\n";
                        return false;
                    }
                    int int_value = std::atoi(value.c_str());
                    if (arg.is_multi) {
                        if (arg.values_ptr != nullptr) {
                            arg.values_ptr->push_back(int_value);
                            arg.use_value = true;
                        }
                        else {
                            std::cerr << "Error: No storage provided for multi-value argument --" << name << "\n";
                            return false;
                        }
                    }
                    else {
                        arg.value = int_value;
                        arg.use_value = true;
                        if (arg.external_value_ptr != nullptr) {
                            *(arg.external_value_ptr) = int_value;
                        }
                    }
                    find_int = true;
                    break;
                }
            }
            if (find_int) continue;

            std::cerr << "Error: Unknown argument --" << name << "\n";
            return false;
        }
        else if (current_arg.length() >= 1 && current_arg.substr(0, 1) == "-") {
            char short_name = '\0';
            if (current_arg.length() >= 2) {
                short_name = current_arg[1];
            }
            else {
                std::cerr << "Error: Invalid short argument " << current_arg << "\n";
                return false;
            }

            size_t equal_pos = current_arg.find('=');
            if (equal_pos != std::string::npos) {
                value = current_arg.substr(equal_pos + 1);
            }

            if (help_struct.use_short_name && short_name == help_struct.short_name) {
                PrintHelp();
                help_flag = true;
                return true;
            }

            bool find_flag = false;
            for (size_t j = 0; j < flag_arr.size(); j++) {
                FlagArg& arg = flag_arr[j];
                if (arg.use_short_name && arg.short_name == short_name) {
                    arg.value = true;
                    if (arg.external_value_ptr != nullptr) {
                        *(arg.external_value_ptr) = true;
                    }
                    find_flag = true;
                    break;
                }
            }
            if (find_flag) continue;

            bool find_string = false;
            for (size_t j = 0; j < string_arr.size(); j++) {
                StringArg& arg = string_arr[j];
                if (arg.use_short_name && arg.short_name == short_name) {
                    if (equal_pos == std::string::npos) {
                        if (i + 1 < args.size()) {
                            value = args[++i];
                        }
                        else {
                            std::cerr << "Error: Missing value for argument -" << short_name << "\n";
                            return false;
                        }
                    }
                    else {
                        value = current_arg.substr(equal_pos + 1);
                    }
                    arg.value = value;
                    arg.use_value = true;
                    if (arg.external_value_ptr != nullptr) {
                        *(arg.external_value_ptr) = value;
                    }
                    find_string = true;
                    break;
                }
            }
            if (find_string) continue;

            bool find_int = false;
            for (size_t j = 0; j < int_arr.size(); j++) {
                IntArg& arg = int_arr[j];
                if (arg.use_short_name && arg.short_name == short_name) {
                    if (equal_pos == std::string::npos) {
                        if (i + 1 < args.size()) {
                            value = args[++i];
                        }
                        else {
                            std::cerr << "Error: Missing value for argument -" << short_name << "\n";
                            return false;
                        }
                    }
                    if (!is_num(value)) {
                        std::cerr << "Error: Value for argument -" << short_name << " must be an integer.\n";
                        return false;
                    }
                    int int_value = std::atoi(value.c_str());
                    if (arg.is_multi) {
                        if (arg.values_ptr != nullptr) {
                            arg.values_ptr->push_back(int_value);
                            arg.use_value = true;
                        }
                        else {
                            std::cerr << "Error: No storage provided for multi-value argument -" << short_name << "\n";
                            return false;
                        }
                    }
                    else {
                        arg.value = int_value;
                        arg.use_value = true;
                        if (arg.external_value_ptr != nullptr) {
                            *(arg.external_value_ptr) = int_value;
                        }
                    }
                    find_int = true;
                    break;
                }
            }
            if (find_int) continue;

            std::cerr << "Error: Unknown argument -" << short_name << "\n";
            return false;
        }
        else {
            bool handled = false;
            for (size_t j = 0; j < int_arr.size(); j++) {
                IntArg& arg = int_arr[j];
                if (arg.is_positional) {
                    if (!is_num(current_arg)) {
                        std::cerr << "Error: Positional argument must be an integer.\n";
                        return false;
                    }
                    int int_value = std::atoi(current_arg.c_str());
                    if (arg.values_ptr != nullptr) {
                        arg.values_ptr->push_back(int_value);
                        arg.use_value = true;
                        if (arg.external_value_ptr != nullptr) {
                            *(arg.external_value_ptr) = int_value;
                        }
                        handled = true;
                    }
                    else {
                        std::cerr << "Error: No storage provided for positional argument.\n";
                        return false;
                    }
                    break;
                }
            }
            if (!handled) {
                position_args.push_back(current_arg);
            }
        }
    }

    for (size_t i = 0; i < string_arr.size(); i++) {
        StringArg& arg = string_arr[i];
        if (arg.default_value.empty() && !arg.use_value) {
            std::cerr << "Error: Missing required argument --" << arg.long_name << "\n";
            return false;
        }
    }

    for (size_t i = 0; i < int_arr.size(); i++) {
        IntArg& arg = int_arr[i];
        if (arg.default_value == 0 && !arg.use_value && !arg.is_positional) {
            std::cerr << "Error: Missing required argument --" << arg.long_name << "\n";
            return false;
        }
        if (arg.is_multi && arg.values_ptr != nullptr) {
            if (arg.values_ptr->size() < arg.min_count) {
                std::cerr << "Error: Argument --" << arg.long_name << " requires at least " << arg.min_count << " values.\n";
                return false;
            }
        }
    }

    return true;
}

std::string ArgParser::GetStringValue(std::string name) {
    for (size_t i = 0; i < string_arr.size(); i++) {
        StringArg& arg = string_arr[i];
        if (arg.long_name == name) {
            return arg.value;
        }
    }
    return "";
}

int ArgParser::GetIntValue(std::string name, int index) {
    for (size_t i = 0; i < int_arr.size(); i++) {
        IntArg& arg = int_arr[i];
        if (arg.long_name == name && arg.use_value) {
            if (arg.is_multi && arg.values_ptr != nullptr) {
                if (index >= 0 && static_cast<size_t>(index) < arg.values_ptr->size()) {
                    return (*arg.values_ptr)[index];
                }
                else {
                    return 0;
                }
            }
            else {
                return arg.value;
            }
        }
    }
    return 0;
}

bool ArgParser::GetFlag(std::string name) {
    for (size_t i = 0; i < flag_arr.size(); i++) {
        FlagArg& arg = flag_arr[i];
        if (arg.long_name == name) {
            return arg.value;
        }
    }
    return false;
}

bool ArgParser::Help() {
    return help_flag;
}

void ArgParser::PrintHelp() {
    std::cout << description_pars << "\n";
    std::cout << "Available arguments:\n";

    if (!help_struct.long_name.empty()) {
        if (help_struct.use_short_name) {
            std::cout << "  -" << help_struct.short_name << ", --" << help_struct.long_name << " : " << help_struct.description << "\n";
        }
        else {
            std::cout << "      --" << help_struct.long_name << " : " << help_struct.description << "\n";
        }
    }

    for (size_t i = 0; i < flag_arr.size(); i++) {
        FlagArg& arg = flag_arr[i];
        if (arg.use_short_name) {
            std::cout << "  -" << arg.short_name << ", --" << arg.long_name << " : " << arg.description;
            if (arg.default_value) {
                std::cout << " [default = true]";
            }
            std::cout << "\n";
        }
        else {
            std::cout << "      --" << arg.long_name << " : " << arg.description;
            if (arg.default_value) {
                std::cout << " [default = true]";
            }
            std::cout << "\n";
        }
    }

    for (size_t i = 0; i < string_arr.size(); i++) {
        StringArg& arg = string_arr[i];
        if (arg.use_short_name) {
            std::cout << "  -" << arg.short_name << ", --" << arg.long_name << " <string> : " << arg.description;
            if (arg.is_multi) {
                std::cout << " [repeated]";
            }
            if (!arg.default_value.empty()) {
                std::cout << " [default = " << arg.default_value << "]";
            }
            std::cout << "\n";
        }
        else {
            std::cout << "      --" << arg.long_name << " <string> : " << arg.description;
            if (arg.is_multi) {
                std::cout << " [repeated]";
            }
            if (!arg.default_value.empty()) {
                std::cout << " [default = " << arg.default_value << "]";
            }
            std::cout << "\n";
        }
    }

    for (size_t i = 0; i < int_arr.size(); i++) {
        IntArg& arg = int_arr[i];
        if (arg.use_short_name) {
            std::cout << "  -" << arg.short_name << ", --" << arg.long_name << " <int> : " << arg.description;
            if (arg.is_multi) {
                std::cout << " [repeated";
                if (arg.min_count > 0) {
                    std::cout << ", min args = " << arg.min_count;
                }
                std::cout << "]";
            }
            if (arg.default_value != 0) {
                std::cout << " [default = " << arg.default_value << "]";
            }
            std::cout << "\n";
        }
        else {
            std::cout << "      --" << arg.long_name << " <int> : " << arg.description;
            if (arg.is_multi) {
                std::cout << " [repeated";
                if (arg.min_count > 0) {
                    std::cout << ", min args = " << arg.min_count;
                }
                std::cout << "]";
            }
            if (arg.default_value != 0) {
                std::cout << " [default = " << arg.default_value << "]";
            }
            std::cout << "\n";
        }
    }
}

} 