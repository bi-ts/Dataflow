
#include <dataflow/qt/qml_data.h>

namespace dataflow
{
namespace qt
{
qml_data::qml_data() = default;

qml_data::operator const QVariant&() const
{
  return value_;
}

const std::shared_ptr<QObject> qml_data::to_qobject() const
{
  return p_qobject_;
}

bool qml_data::operator==(const qml_data& other) const
{
  return value_ == other.value_;
}

bool qml_data::operator!=(const qml_data& other) const
{
  return !(*this == other);
}
}

std::ostream& qt::operator<<(std::ostream& out, const qml_data& v)
{
  // TODO: implement
  return out;
}
}
