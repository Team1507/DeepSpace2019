#include "commands/GrpStartRightPlaceHatch.h"

//*****Cmd Includes*****
#include "commands/CmdConeControl.h"
#include "commands/CmdDriveFwdGyro.h"
#include "commands/CmdTurn2Line.h"
#include "commands/CmdDriveLineDriftLeft.h"
#include "commands/CmdHatchEject.h"
#include "commands/CmdDriveFwdGyro.h"
#include "commands/CmdDriveRevGyro.h"
#include "commands/CmdDriveTurn2Heading.h"
#include "commands/CmdDriveManual.h"
#include "commands/CmdDriveRevTurn2Heading.h"
#include "commands/CmdPrintAutoText.h"
#include "commands/CmdDriveClearAll.h"

//*****Constants******
#define DRIVE_SPEED         .6   //Subject To change
#define DRIVE_STR8_DISTANCE  194 //The first Drive Foward //was 235
#define TURN_2_LINE_SPEED   .5 //was 0.3
#define DRIVE_CARGO_SHIP      6   //after turn towards cargo ship
#define LINE_FOLLOW_SPEED   .4
#define LINE_FOLLOW_DISTANCE 6
#define BACKUP_DISTANCE      36  //May want to change //was 85
#define LAST_TURN_SPEED     .8
#define LAST_TURN_ANGLE      -185 //will change with the booking angle
#define BOOKING_SPEED        0.5 //was 0.7
#define BOOKING_ANGLE        -185 //may be totally wrong
#define BOOKING_DISTANCE     48 //its whatever //was 225

//******Auto Group******
GrpStartRightPlaceHatch::GrpStartRightPlaceHatch() 
{
    AddSequential(new CmdPrintAutoText("GrpStartRightPlaceHatch Begin"));
    AddSequential(new CmdDriveClearAll());
    //***************************************************

  //Drop Cone
  AddSequential(new CmdConeControl(false));
  //Drive Foward towards line
  AddSequential(new CmdDriveFwdGyro(DRIVE_SPEED, 0, DRIVE_STR8_DISTANCE, true, 0.0));
  //Slow down
  AddSequential(new CmdDriveManual(-0.2, -0.2, 0.5));
  //Turn left until line is found
  AddSequential(new CmdTurn2Line(true, TURN_2_LINE_SPEED));
  //Follow line fowards
  AddSequential(new CmdDriveLineDriftLeft(LINE_FOLLOW_SPEED, -90, LINE_FOLLOW_DISTANCE, true, 4.0)); //timeout was 0.0
  //Eject Hatch
  AddSequential(new CmdHatchEject());

  //Backup and turn
  AddSequential(new CmdDriveRevTurn2Heading(DRIVE_SPEED, -135  ));

  //Turn to human station (roughly)
  AddSequential(new CmdDriveTurn2Heading( -0.05, LAST_TURN_SPEED,  LAST_TURN_ANGLE,0.0)); 
  
  //start booking
  AddSequential(new CmdDriveFwdGyro(BOOKING_SPEED, BOOKING_ANGLE, BOOKING_DISTANCE, true, 0.0));//will want to fiddle with timeout
}
