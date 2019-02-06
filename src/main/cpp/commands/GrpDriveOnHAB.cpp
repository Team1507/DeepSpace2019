

#include "Commands/GrpDriveOnHAB.h"
#include "Commands/CmdPrintAutoText.h"
#include "Commands/CmdDriveFwdEncoder.h"
#include "Commands/CmdDriveRevEncoder.h"
#include "Commands/CmdDriveFwdGyro.h"
#include "Commands/CmdDriveRevGyro.h"

//#include "Commands/CmdLogEnable.h"
//#include "Commands/CmdLogMark.h"
//#include "Commands/CmdTurnPIDTest.h"

#include "Commands/CmdDriveClearAll.h"
#include "Commands/CmdDriveFwdEncoder.h"
#include "Commands/CmdDriveRevEncoder.h"
#include "Commands/CmdDriveTurn2Angle.h"
#include "Commands/CmdDriveTurn2Heading.h"
#include "Commands/CmdDriveRevTurn2Heading.h"
#include "Commands/CmdDeployLineSensors.h"
#include "Commands/CmdDriveLineDriftLeft.h"
#include "Commands/CmdElevatorGo2Bot.h"
#include "Commands/CmdElevatorGo2Top.h"
#include "Commands/CmdElevatorGo2RocketMid.h"
#include "Commands/CmdElevatorGo2CargoMid.h"
#include "Commands/CmdSetCage.h"
#include "Commands/CmdSetStilts.h"
#include "subsystems/Collector.h"
#include "Commands/CmdDriveRevGyroPitchDown.h"
#include "Commands/CmdDriveRevGyroPhotoeye.h"


#define FWD_PWR 0.6
#define TRN_PWR 0.5

GrpDriveOnHAB::GrpDriveOnHAB() 
{
    AddSequential(new CmdPrintAutoText("GrpDriveOnHAB Begin"));
    AddSequential(new CmdDriveClearAll());
    AddSequential(new WaitCommand(0.05));
    //***************************************************
    //Stage 1- Raise up rear of robot
    AddSequential(new CmdSetCage(Collector::DEPLOY_CAGE));
    AddSequential(new WaitCommand(1.5));

    //Stage 2- Get rear wheels up on level 2
    AddSequential(new CmdDriveRevGyro(0.4, 0, 75, true, 0.0)); 
    AddSequential(new CmdDriveRevGyroPitchDown(0.4, 0, 75, true, 0.0)); 

    //Stage 3- Get center wheels on level 2
    AddSequential(new CmdSetStilts(Drivetrain::DEPLOY_STILTS));
    AddSequential(new CmdDriveRevGyroPhotoeye(0.4, 0, 75, true, 0.0)); 

    //Stage 4- Drive backwards and retract stilts
    AddSequential(new CmdDriveRevGyro(0.4, 0, 75, true, 0.0)); 
    AddSequential(new CmdSetStilts(Drivetrain::RETRACT_STILTS));

    //***************************************************
    AddSequential(new WaitCommand(1.0));        //Let it finish whatever it's doing
    AddSequential(new CmdPrintAutoText("GrpDriveOnHAB Done!"));
}
