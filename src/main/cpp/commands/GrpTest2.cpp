

#include "Commands/GrpTest2.h"
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


#define FWD_PWR 0.6
#define TRN_PWR 0.5

GrpTest2::GrpTest2() 
{
    AddSequential(new CmdPrintAutoText("GrpTest2 Begin"));
    AddSequential(new CmdDriveClearAll());
    AddSequential(new WaitCommand(0.05));
    //***************************************************
    //Drive and turn to line
    AddSequential(new CmdElevatorGo2Bot());
    AddSequential(new CmdElevatorGo2CargoMid());
    AddSequential(new CmdDriveFwdGyro(0.7, 0, 94, true, 0.0)); //was 0.6,-70,93
    AddSequential(new CmdDriveTurn2Heading(0.6, -60)); //was 0.45,-75
    AddSequential(new CmdDriveTurn2Heading(0.35, -90)); //was 0.25,-90

    //Approach and follow line
    //AddSequential(new CmdDeployLineSensors(true));                      //Deploy Line Sensors 
    AddSequential(new CmdDriveFwdGyro(0.4, -90, 42, true, 0.0)); //was 0.35,-70,36
    AddSequential(new CmdDriveFwdGyro(0.35, -90, 12, true, 0.0)); //was 0.25, ,
    AddSequential(new CmdDriveLineDriftLeft(0.3, -90, 12, true, 0.0)); //was 0.35,-90,12
    AddSequential(new WaitCommand(0.15));
    //AddSequential(new CmdDeployLineSensors(false));                     //Retract Line Sensors

    //Back up and turn to human station
    AddSequential(new CmdDriveRevGyro(0.7, -88, 31.5, true, 0.0)); //was 0.5,-90,30
    AddSequential(new CmdDriveRevTurn2Heading(0.6, -180)); //was 0.6,360
    //AddSequential(new WaitCommand(3.0)); //was 0.5

    //Approach and follow line
    AddSequential(new CmdDriveFwdGyro(0.7, -180, 124, true, 0.0)); //was 0.6,180,120
    AddSequential(new CmdDriveFwdGyro(0.35, -180, 23, true, 0.0)); //was .25, -180, 25
    AddSequential(new CmdDriveLineDriftLeft(0.3, -180, 12, true, 0.0));

     
    //Back up and place 2nd hatch 
    AddSequential(new CmdDriveRevGyro(0.4, -180, 3, true, 0.0)); //was 0.3,-180,102
    AddSequential(new CmdDriveRevGyro(0.7, -180, 92, true, 0.0)); //was 0.6,-180,90
    AddSequential(new WaitCommand(0.25));
    AddSequential(new CmdDriveTurn2Heading(0.6, -90)); //was 0.45,-90
    AddSequential(new CmdDriveFwdGyro(0.6, -90, 24, true, 0.0)); //was 0.35,-90,40
    AddSequential(new CmdDriveFwdGyro(0.35, -90, 16, true, 0.0)); //was 0.35,-90,40
    AddSequential(new CmdDriveLineDriftLeft(0.3, -90, 14, true, 0.0)); //was 0.35,-90,16

    //AddSequential(new CmdDriveFwdEncoder(0.4, 120, true, 0.0)); //was 0.4,120
    //AddSequential(new CmdDriveFwdEncoder(0.1, 5, true, 0.0)); //was 0.4,120
    //AddSequential(new CmdDriveRevEncoder(0.5, 60, true, 0.0));  //was 0.5, 60
   
  


    //***************************************************
    AddSequential(new WaitCommand(1.0));        //Let it finish whatever it's doing
    AddSequential(new CmdPrintAutoText("GrpTest2 Done!"));
}
