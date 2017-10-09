
//  Copyright (c) 2014 - 2017 Maksym V. Bilinets.
//
//  This file is part of Dataflow++.
//
//  Dataflow++ is free software: you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  Dataflow++ is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public License
//  along with Dataflow++. If not, see <http://www.gnu.org/licenses/>.

#include <dataflow/io.h>

#include <iostream>

dataflow::ref<std::string>
dataflow::console::Input(const Time& t0, const ref<std::string>& prompt)
{
  struct policy
  {
    static std::string label()
    {
      return "Input";
    }
    static std::string calculate(const std::string& prompt)
    {
      std::cout << prompt;

      std::string line;

      std::getline(std::cin, line);

      return line;
    };
  };

  return core::LiftPuller<policy>(prompt);
}

dataflow::ref<std::string>
dataflow::console::Error(const Time& t0, const ref<std::string>& message)
{
  struct policy
  {
    static std::string label()
    {
      return "Error";
    }
    static std::string calculate(const std::string& s)
    {
      std::cerr << s << std::endl;
      return s;
    };
  };

  return core::LiftPuller<policy>(message);
}

dataflow::ref<std::string>
dataflow::console::Log(const Time& t0, const ref<std::string>& message)
{
  struct policy
  {
    static std::string label()
    {
      return "Log";
    }
    static std::string calculate(const std::string& s)
    {
      std::clog << s << std::endl;
      return s;
    };
  };

  return core::LiftPuller<policy>(message);
}

dataflow::ref<std::string>
dataflow::console::Output(const Time& t0, const ref<std::string>& message)
{
  struct policy
  {
    static std::string label()
    {
      return "Output";
    }
    static std::string calculate(const std::string& s)
    {
      std::cout << s << std::endl;
      return s;
    };
  };

  return core::LiftPuller<policy>(message);
}
