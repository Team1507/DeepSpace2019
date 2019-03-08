
#include "commands/AutoDriveStr8.h"
#include "commands/CmdDriveManual.h"
#include "commands/CmdPrintAutoText.h"
#include "commands/CmdDriveClearAll.h"
#include "Commands/CmdDriveClearEncoder.h"
#include "commands/CmdDriveFwdGyro.h"
#include "commands/CmdConeControl.h"


AutoDriveStr8::AutoDriveStr8() {
	AddSequential(new CmdPrintAutoText("AutoDriveStr8")	);
	AddSequential(new CmdDriveClearAll( ) );

	//AddSequential(new CmdDriveClearEncoder() );
	//AddSequential(new CmdDriveManual(0.5, 0.5, 3));  //was 0.4,0.4,2
	//AddSequential(new CmdDriveManual(0.5, 0.5, 3));  //was 0.4,0.4,2
	//                               power,heading, distance, stop, timeout)
	AddSequential(new CmdDriveFwdGyro(0.6,   0.0,      1000,   true, 1.5    ));  

	//testing backup on AutoSwitchRightHighGear
	//AddSequential(new CmdDriveManual(-0.3, -0.4, 0.5));  //was .3,.3,.5;
	//AddSequential(new CmdDriveManual(-0.3, -0.4, 1.5));  //was -.4,-.5,1.0;
	//AddSequential(new CmdDriveManual(-0.5, -0.0, 0.5));  //was -.7, .0, 0.65  //Spin left side to point at pile


	//testing backup on AutoSwitchLeftHighGear
	//AddSequential(new CmdDriveManual(-0.4, -0.3, 0.5));  //was .3,.3,.5;
	//AddSequential(new CmdDriveManual(-0.4, -0.3, 1.5));  //was -.4,-.3,1.0;
	//AddSequential(new CmdDriveManual(-0.0, -0.5, 0.5));  //Spin right side to point at pile


	AddSequential( new CmdDriveManual(0.0, 0.0, 0) );	//Do we want to stop at end?
	AddSequential( new CmdConeControl(false) );
	AddSequential( new CmdPrintAutoText("Auto Done") );
}
