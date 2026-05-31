#include <command/base/CommandError.h>

#ifndef COMMANDRESULT_H
#define COMMANDRESULT_H

struct CommandResult {
  bool complete;
  CommandError error;
};

#endif