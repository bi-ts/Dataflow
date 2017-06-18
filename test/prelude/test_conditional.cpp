
#include <dataflow/prelude/conditional.h>

#include <dataflow/introspect.h>

#include <boost/test/unit_test.hpp>

using namespace dataflow;

namespace dataflow_test
{

namespace
{
class test_fixture
{
protected:
  test_fixture()
  {
  }

private:
  Engine engine_;
};
}

BOOST_AUTO_TEST_SUITE(test_conditional)

BOOST_FIXTURE_TEST_CASE(test_Switch_if_int, test_fixture)
{
  auto x = Var("ten");

  auto f = *Switch(Case(x == "one", 1),
                   Case(x == "three", 3),
                   Case(x == "five", 5),
                   Case(x == "ten", 10),
                   Default(0));

  BOOST_CHECK_EQUAL(f(), 10);

  x = "one";

  BOOST_CHECK_EQUAL(f(), 1);

  x = "five";

  BOOST_CHECK_EQUAL(f(), 5);

  x = "three";

  BOOST_CHECK_EQUAL(f(), 3);
}

BOOST_FIXTURE_TEST_CASE(test_Switch_string_int, test_fixture)
{
  auto x = Var("ten");
  auto y1 = Var(1);
  auto y3 = Var(3);
  auto y5 = Var(5);

  auto f = *Switch(x,
                   Case(Const("one"), y1),
                   Case(Const("two"), 2),
                   Case(std::string("three"), y3),
                   Case(std::string("four"), 4),
                   Case("five", y5),
                   Case("ten", 10),
                   Default(0));

  BOOST_CHECK_EQUAL(f(), 10);

  x = "one";

  BOOST_CHECK_EQUAL(f(), 1);

  x = "five";

  BOOST_CHECK_EQUAL(f(), 5);

  x = "two";

  BOOST_CHECK_EQUAL(f(), 2);

  x = "four";

  BOOST_CHECK_EQUAL(f(), 4);

  x = "three";

  BOOST_CHECK_EQUAL(f(), 3);
}

BOOST_FIXTURE_TEST_CASE(test_Switch_string_string, test_fixture)
{
  auto x = Var("yes");

  auto f = *Switch(x,
                   Case(Const("yes"), "ja"),
                   Case(std::string("no"), "nee"),
                   Case("maybe", "misschien"),
                   Default(""));

  BOOST_CHECK_EQUAL(f(), "ja");

  x = "maybe";

  BOOST_CHECK_EQUAL(f(), "misschien");

  x = "no";

  BOOST_CHECK_EQUAL(f(), "nee");
}

BOOST_FIXTURE_TEST_CASE(test_Switch_if_string, test_fixture)
{
  auto x = Var<int>(1);
  auto y = Var("one");

  auto f = *Switch(Case(x == 1, y),
                   Case(x == 2, Const("two")),
                   Case(x == 3, std::string("three")),
                   Case(x == 4, "four"),
                   Default("error"));

  BOOST_CHECK_EQUAL(f(), "one");

  x = 2;

  BOOST_CHECK_EQUAL(f(), "two");

  x = 3;

  BOOST_CHECK_EQUAL(f(), "three");

  x = 4;

  BOOST_CHECK_EQUAL(f(), "four");

  x = 5;

  BOOST_CHECK_EQUAL(f(), "error");
}

BOOST_FIXTURE_TEST_CASE(test_Switch_if_operator, test_fixture)
{
  auto x = Var<int>(1);
  auto y = Var("three");

  auto f = *Switch(x == 1 >>= "one",
                   x == 2 >>= std::string("two"),
                   x == 3 >>= y,
                   x == 4 >>= Const("four"),
                   Default("error"));

  BOOST_CHECK_EQUAL(f(), "one");

  x = 2;

  BOOST_CHECK_EQUAL(f(), "two");

  x = 3;

  BOOST_CHECK_EQUAL(f(), "three");

  x = 4;

  BOOST_CHECK_EQUAL(f(), "four");

  x = 5;

  BOOST_CHECK_EQUAL(f(), "error");
}

BOOST_AUTO_TEST_SUITE_END()

} // dataflow_test
