#pragma once

#include <string>
#include <map>
#include <functional>

namespace dye {
    // Map for ANSI color codes
    inline std::map<std::string, std::string> ColorMap = {
        {"reset", "\033[0m"},
        {"bold", "\033[1m"},
        {"underline", "\033[4m"},
        {"black", "\033[30m"},
        {"red", "\033[31m"},
        {"green", "\033[32m"},
        {"yellow", "\033[33m"},
        {"blue", "\033[34m"},
        {"magenta", "\033[35m"},
        {"cyan", "\033[36m"},
        {"white", "\033[37m"},
        {"light_black", "\033[90m"},
        {"light_red", "\033[91m"},
        {"light_green", "\033[92m"},
        {"light_yellow", "\033[93m"},
        {"light_blue", "\033[94m"},
        {"light_magenta", "\033[95m"},
        {"light_cyan", "\033[96m"},
        {"light_white", "\033[97m"}
    };

    // Function to apply ANSI color codes
    inline std::string colorize(const std::string& style, const std::string& input) {
        auto style_iter = ColorMap.find(style);
        if (style_iter != ColorMap.end()) {
            return style_iter->second + input + ColorMap["reset"];
        } else {
            return input; // Return input unchanged if style is not found
        }
    }

    // Factory function to create color/style functions dynamically
    inline std::function<std::string(const std::string&)> make_color_function(const std::string& style) {
        return [style](const std::string& input) {
            return colorize(style, input);
        };
    }

    // Aliases for commonly used colors and styles
    const auto reset = make_color_function("reset");
    const auto bold = make_color_function("bold");
    const auto underline = make_color_function("underline");
    const auto black = make_color_function("black");
    const auto red = make_color_function("red");
    const auto green = make_color_function("green");
    const auto yellow = make_color_function("yellow");
    const auto blue = make_color_function("blue");
    const auto magenta = make_color_function("magenta");
    const auto cyan = make_color_function("cyan");
    const auto white = make_color_function("white");
    const auto light_black = make_color_function("light_black");
    const auto light_red = make_color_function("light_red");
    const auto light_green = make_color_function("light_green");
    const auto light_yellow = make_color_function("light_yellow");
    const auto light_blue = make_color_function("light_blue");
    const auto light_magenta = make_color_function("light_magenta");
    const auto light_cyan = make_color_function("light_cyan");
    const auto light_white = make_color_function("light_white");
}
