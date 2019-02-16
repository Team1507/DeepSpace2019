#include "commands/GrpStartLeftPlaceHatch.h"

//*****Cmd Includes*****
#include "commands/CmdConeControl.h"
#include "commands/CmdDriveFwdGyro.h"
#include "commands/CmdTurn2Line.h"
#include "commands/CmdDriveLineDriftLeft.h"
#include "commands/CmdHatchEject.h"
#include "commands/CmdDriveFwdGyro.h"
#include "commands/CmdDriveRevGyro.h"
#include "commands/CmdDriveTurn2Heading.h"

//*****Constants******
#define DRIVE_SPEED         .6   //Subject To change
#define DRIVE_STR8_DISTANCE  235 //The first Drive Foward
#define TURN_2_LINE_SPEED   .3
#define DRIVE_CARGOSHIP      6   //after turn towards cargo ship
#define LINE_FOLLOW_SPEED   .3
#define LINE_FOLLOW_DISTANCE 6
#define BACKUP_DISTANCE      85  //May want to change
#define LAST_TURN_SPEED     .6
#define LAST_TURN_ANGLE      -175 //will change with the booking angle
#define BOOKING_SPEED        1
#define BOOKING_ANGLE        -175 //may be totally wrong
#define BOOKING_DISTANCE     225 //its whatever

//******Auto Group******
GrpStartLeftPlaceHatch::GrpStartLeftPlaceHatch() 
{
  //Drop Cone
  AddSequential(new CmdConeControl(false));
  //Drive Foward towards line
  AddSequential(new CmdDriveFwdGyro(DRIVE_SPEED, 0, DRIVE_STR8_DISTANCE, true, 0.0));
  //Turn right until line is found
  AddSequential(new CmdTurn2Line(false, TURN_2_LINE_SPEED));
  //Follow line fowards
  AddSequential(new CmdDriveLineDriftLeft(LINE_FOLLOW_SPEED, 90, LINE_FOLLOW_DISTANCE, true, 0.0));
  //Eject Hatch
  AddSequential(new CmdHatchEject());
  //Backup
  AddSequential(new CmdDriveRevGyro(DRIVE_SPEED, 90, BACKUP_DISTANCE, true, 0.0));
  //Turn to human station (roughly)
  AddSequential(new CmdDriveTurn2Heading(LAST_TURN_SPEED, LAST_TURN_ANGLE)); 
  //start booking
  AddSequential(new CmdDriveFwdGyro(BOOKING_SPEED, BOOKING_ANGLE, BOOKING_DISTANCE, true, 0.0));//will want to fiddle with timeout
}
