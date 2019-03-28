

#include "Commands/AutoRightHatch.h"
#include "Commands/CmdPrintAutoText.h"
#include "Commands/CmdDriveClearAll.h"

#include "commands/CmdDriveManual.h"
#include "Commands/CmdDriveClearEncoder.h"
#include "commands/CmdDriveFwdGyro.h"
#include "Commands/CmdDriveTurn2Heading.h"
#include "Commands/CmdBuzzGamepad.h"
#include "Commands/CmdDriveTurn2Angle.h"
#include "commands/CmdSetHumanDriver.h"
#include "Commands\CmdDriveClearEncoderV2.h"


AutoRightHatch::AutoRightHatch() 
{
    //** SETUP **
    AddSequential(new CmdPrintAutoText("AutoRightHatch Begin"));
    AddSequential(new CmdDriveClearAll());
    //**************************************************

    //ADD DELAY>....

    //From Drive Staright
	AddSequential(new CmdDriveFwdGyro(0.6,   0.0,      1000,   true, 1.5    ));  

    //Drive to Cargo
	//AddSequential(new CmdDriveClearEncoder() );
	AddSequential(new CmdDriveClearEncoderV2() );
	AddSequential(new CmdDriveFwdGyro(0.6,   21.5,      108,   true, 0.0    ));  //was 20.0, 102

    //Hard Left turn
    //AddSequential(new CmdDriveTurn2Heading(0.6, -90));
    AddSequential(new CmdDriveTurn2Angle(0.8, -90));


    //Done and release to Teleop
    AddSequential(new CmdDriveManual(0.0, 0.0, 0)); 
    AddSequential(new CmdBuzzGamepad() );
    AddSequential( new CmdSetHumanDriver() );	




    //Bank of commands ready to use
	//AddSequential(new CmdDriveFwdGyro(0.6,   0.0,      1000,   true, 1.5    ));  
	//AddSequential(new CmdDriveClearEncoder() );
    //AddSequential(new CmdDriveTurn2Heading(0.6, -60)); //was 0.45,-75
    //AddSequential(new CmdDriveManual(0.5, 0.5, 2)); 
    //AddSequential(new WaitCommand(1.0));  
    //AddSequential(new CmdDriveManual(test3, test3, 2));   

    //***************************************************
    AddSequential(new CmdPrintAutoText("AutoRightHatch Done!"));
}
