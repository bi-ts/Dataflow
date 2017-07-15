
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

#pragma once

#include <iostream>
#include <sstream>

namespace dataflow_test
{

class io_fixture
{
public:
  io_fixture();

  ~io_fixture();

  void capture_output();

  void reset_output();

  void set_input(const std::string& str);

  void reset_input();

  void reset();

  std::string err_string();
  std::string log_string();
  std::string out_string();

private:
  std::string concat_lines_(std::string str);

private:
  std::streambuf* const p_original_cin_buffer_;
  std::streambuf* const p_original_cerr_buffer_;
  std::streambuf* const p_original_clog_buffer_;
  std::streambuf* const p_original_cout_buffer_;

  std::stringstream cin_string_stream;
  std::stringstream cerr_string_stream;
  std::stringstream clog_string_stream;
  std::stringstream cout_string_stream;
};
}
