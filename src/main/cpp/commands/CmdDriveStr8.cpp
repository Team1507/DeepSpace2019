

#include "commands/CmdDriveStr8.h"
#include "commands/CmdDriveManual.h"

CmdDriveStr8::CmdDriveStr8() {
  AddSequential(new CmdDriveManual(0.4, 0.4, 3));  //was ,,
}
