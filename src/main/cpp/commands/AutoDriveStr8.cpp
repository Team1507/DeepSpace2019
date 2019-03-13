
#include "commands/AutoDriveStr8.h"
#include "commands/CmdDriveManual.h"
#include "commands/CmdPrintAutoText.h"
#include "commands/CmdDriveClearAll.h"
#include "Commands/CmdDriveClearEncoder.h"
#include "commands/CmdDriveFwdGyro.h"
#include "commands/CmdConeControl.h"
#include "Commands/CmdBuzzGamepad.h"
#include "commands/CmdSetHumanDriver.h"


AutoDriveStr8::AutoDriveStr8() {
	AddSequential(new CmdPrintAutoText("AutoDriveStr8")	);
	AddSequential(new CmdDriveClearAll( ) );

	//AddSequential(new CmdDriveClearEncoder() );
	//AddSequential(new CmdDriveManual(0.5, 0.5, 3));  //was 0.4,0.4,2
	//AddSequential(new CmdDriveManual(0.5, 0.5, 3));  //was 0.4,0.4,2
	//                               power,heading, distance, stop, timeout)
	AddSequential(new CmdDriveFwdGyro(0.6,   0.0,      1000,   true, 1.5    ));  

	//Done
	AddSequential( new CmdDriveManual(0.0, 0.0, 0) );	//Do we want to stop at end?
	AddSequential( new CmdConeControl(false) );
    AddSequential( new CmdBuzzGamepad() );	
	AddSequential( new CmdPrintAutoText("Auto Done") );
	AddSequential( new CmdSetHumanDriver() );	
}
