//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ `
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:         BSD License
//                   Kratos default license: kratos/license.txt
//
//  Main authors:    Pooyan Dadvand
//
//

// System includes
#include <ctime>


// External includes


// Project includes
#include "input_output/logger_message.h"
#include "includes/parallel_environment.h"

namespace Kratos
{
  LoggerMessage::MessageSource::MessageSource()
  {
    mRank = ParallelEnvironment::GetDefaultRank();
  }

  std::string LoggerMessage::Info() const
  {
    return "LoggerMessage";
  }

  std::string LoggerMessage::GetTimeStamp() const
  {
    time_t raw_time;
    char buffer[10];

    time (&raw_time);

    strftime(buffer, sizeof(buffer), "%H:%M:%S", localtime(&raw_time));
    return std::string(buffer);
  }

  /// Print information about this object.
  void LoggerMessage::PrintInfo(std::ostream& rOStream) const
  {
    rOStream << Info();
  }

  /// Print object's data.
  void LoggerMessage::PrintData(std::ostream& rOStream) const
  {
    rOStream << mMessage;
  }

  /// char stream function
  LoggerMessage& LoggerMessage::operator << (const char * rString)
  {
    mMessage.append(rString);

    return *this;
  }

  LoggerMessage& LoggerMessage::operator << (std::ostream& (*pf)(std::ostream&))
  {
    std::stringstream buffer;
    pf(buffer);

    mMessage.append(buffer.str());

    return *this;
    }

    LoggerMessage& LoggerMessage::operator << (CodeLocation const& TheLocation)
  {
    mLocation = TheLocation;

    return *this;
  }

  LoggerMessage& LoggerMessage::operator << (Severity const& TheSeverity)
  {
    mSeverity = TheSeverity;

    return *this;
  }

  LoggerMessage& LoggerMessage::operator << (Category const& TheCategory) {
    mCategory = TheCategory;

    return *this;
  }

  LoggerMessage& LoggerMessage::operator << (DistributedFilter const& TheFilter) {
    mDistributedFilter = TheFilter;

    return *this;
  }

  LoggerMessage& LoggerMessage::operator << (DataCommunicator const& TheDataCommunicator) {
    mMessageSource = MessageSource(TheDataCommunicator.Rank());

    return *this;
  }

  /// output stream function
  std::ostream& operator << (std::ostream& rOStream,
    const LoggerMessage& rThis)
  {
    rThis.PrintInfo(rOStream);
    rOStream << std::endl;
    rThis.PrintData(rOStream);

    return rOStream;
  }


}  // namespace Kratos.


