

#include "Commands/GrpDriveOffHAB.h"
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
#include "Commands/CmdDriveFwdGyroPitchDown.h"
#include "Commands/CmdDriveFwdGyroPitchUp.h"


// #define FWD_PWR 0.6
// #define TRN_PWR 0.5

GrpDriveOffHAB::GrpDriveOffHAB() 
{
    AddSequential(new CmdPrintAutoText("GrpDriveOffHAB Begin"));
    AddSequential(new CmdDriveClearAll());
    AddSequential(new WaitCommand(0.05));
    //***************************************************
    //Stage 1- Cross Level 2
    AddSequential(new CmdDriveFwdGyro(0.4, 0, 75, true, 0.0)); 
    AddSequential(new CmdSetCage(Collector::DEPLOY_CAGE));
    AddSequential(new CmdSetStilts(Drivetrain::DEPLOY_STILTS));

    //Stage 2- Lower Front
    AddSequential(new CmdDriveFwdGyro(0.4, 0, 75, true, 0.0)); 
    AddSequential(new CmdSetStilts(Drivetrain::RETRACT_STILTS));

    //Stage 3- Lower Rear
    AddSequential(new CmdDriveFwdGyro(0.4, 0, 75, true, 0.0)); 
    AddSequential(new CmdSetCage(Collector::RETRACT_CAGE));

    //Stage 4- Cross Level 1
    AddSequential(new CmdDriveFwdGyroPitchDown(0.4, 0, 75, true, 0.0)); 
   

    //Stage 5- Stop when we level out
    AddSequential(new CmdDriveFwdGyroPitchUp(0.4, 0, 75, true, 0.0)); 
  
    //***************************************************
    AddSequential(new WaitCommand(1.0));        //Let it finish whatever it's doing
    AddSequential(new CmdPrintAutoText("GrpDriveOffHAB Done!"));
}
