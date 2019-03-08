
#include "commands/AutoDoNothing.h"
#include "commands/CmdPrintAutoText.h"
#include "commands/CmdDriveClearAll.h"

#include "commands/CmdConeControl.h"

AutoDoNothing::AutoDoNothing() {
	AddSequential(new CmdPrintAutoText("AutoDoNothing")	);

	AddSequential( new CmdDriveClearAll() );
	AddSequential( new CmdConeControl(false) );	
}
