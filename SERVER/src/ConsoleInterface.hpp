/* This file is part of Plaine.
 *
 * Plaine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Plaine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Plaine. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <iostream>
#include <string>
#include <list>
#include <exception>

#include "ParseError.hpp"
#include "Lexeme.hpp"

class ConsoleInterface {
    void parse_string(const std::string &str);
    void execute_cmd(const std::string &cmd, const std::list<Lexeme> &args);

public:
    void run();
};