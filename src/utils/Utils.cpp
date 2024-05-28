#include "Utils.h"

namespace
{

static const std::array<std::pair<ReturnCode, std::string>, 5> returnCodeToStringMap {
  {{ReturnCode::Success, "Success"},
   {ReturnCode::ReadError, "ReadError"},
   {ReturnCode::WriteError, "WriteError"},
   {ReturnCode::CopyError, "CopyError"},
   {ReturnCode::InvalidArguments, "InvalidArguments"}}};
   
}    // namespace

const std::string returnCodeToString(const ReturnCode &returnCode)
{
    spdlog::trace("returnCodeToString()");

    const auto it = std::find_if(
      returnCodeToStringMap.begin(), returnCodeToStringMap.end(),
      [returnCode](const std::pair<ReturnCode, std::string> &pair)
      { return pair.first == returnCode; });
    return it == returnCodeToStringMap.end() ? "Unknown" : it->second;
}