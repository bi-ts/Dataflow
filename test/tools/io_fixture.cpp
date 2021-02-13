
//  Copyright (c) 2014 - 2021 Maksym V. Bilinets.
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

#include "io_fixture.h"

#include <algorithm> // std::replace

namespace dataflow_test
{
io_fixture::io_fixture()
: p_original_cin_buffer_(std::cin.rdbuf())
, p_original_cerr_buffer_(std::cerr.rdbuf())
, p_original_clog_buffer_(std::clog.rdbuf())
, p_original_cout_buffer_(std::cout.rdbuf())
, cin_string_stream()
, cerr_string_stream()
, clog_string_stream()
, cout_string_stream()
{
}

io_fixture::~io_fixture()
{
  reset_input();
  reset_output();
}

void io_fixture::capture_output(bool clear_buffers)
{
  if (clear_buffers)
  {
    cerr_string_stream.str("");
    clog_string_stream.str("");
    cout_string_stream.str("");
  }

  std::cerr.rdbuf(cerr_string_stream.rdbuf());
  std::clog.rdbuf(clog_string_stream.rdbuf());
  std::cout.rdbuf(cout_string_stream.rdbuf());
}

void io_fixture::reset_output()
{
  std::cerr.rdbuf(p_original_cerr_buffer_);
  std::clog.rdbuf(p_original_clog_buffer_);
  std::cout.rdbuf(p_original_cout_buffer_);
}

void io_fixture::set_input(const std::string& str)
{
  cin_string_stream.str(str);

  std::cin.rdbuf(cin_string_stream.rdbuf());
}

void io_fixture::reset_input()
{
  std::cin.rdbuf(p_original_cin_buffer_);
}

void io_fixture::reset()
{
  reset_input();
  reset_output();
}

std::string io_fixture::err_string()
{
  return concat_lines_(cerr_string_stream.str());
}

std::string io_fixture::log_string()
{
  return concat_lines_(clog_string_stream.str());
}

std::string io_fixture::out_string()
{
  return concat_lines_(cout_string_stream.str());
}

std::string io_fixture::concat_lines_(std::string str)
{
  std::replace(str.begin(), str.end(), '\n', ';');

  return str;
}
}
