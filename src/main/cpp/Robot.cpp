#include "Robot.h"
#include <frc/commands/Scheduler.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include "GamepadMap.h"
#include <cameraserver/CameraServer.h>
//#include <CameraServer.h> //new

OI* Robot::m_oi;

Drivetrain* Robot::m_drivetrain;
Collector* Robot::m_collector;
Carriage* Robot::m_carriage;
Elevator *Robot::m_elevator;
DriverFeedback* Robot::m_driverfeedback;

void Write2Dashboard(void);

void Robot::RobotInit() {
    //*************************** INIT ******************************
    std::cout<<"RobotInit"<<std::endl;
	std::cout<<"Buzz 2019"<<std::endl;
	std::cout<<"Version: " << __DATE__ <<"  "<<__TIME__<<std::endl<<std::endl; //Display Version info to Log file
    
    //******Subsystem Inits******
    m_drivetrain      = new Drivetrain();
    m_collector       = new Collector();
    m_carriage        = new Carriage();
    m_elevator        = new Elevator();
    m_driverfeedback  = new DriverFeedback();
    
    //OI **MUST** be after all subsystem Inits
    m_oi = new OI();


    ahrs = new AHRS(SPI::Port::kMXP);
    
    frc::SmartDashboard::PutData("Auto Modes", &m_chooser);

    //Subsystem Init
    m_drivetrain->Init();
    m_drivetrain->LineSensorsRetract();
    m_elevator->TalonSRXinit();
    m_carriage->VictorSPXInit();
    m_collector->VictorSPXInit();

    m_drivetrain->SetHighGear();
    m_collector->RetractCage();
    m_carriage->TiltUp();
    m_collector->CloseBridge();
    m_carriage->CloseLatch();
    m_drivetrain->ResetEncoders();

    //CAMERA  
    cs::UsbCamera camera = frc::CameraServer::GetInstance()->StartAutomaticCapture();
//     camera.SetResolution(320, 240); //was 160,120
//         //camera.SetVideoMode(cs::VideoMode::kMJPEG, 320, 240, 8); //kGray, kRGB565
//     camera.SetFPS(15);
//     camera.SetBrightness(0);       //100-most bright //0- most dark //was 20
//         //camera.SetVideoMode() //stinky line don't worry about it
//         //camera.SetWhiteBalanceManual(cs::VideoCamera::kFixedFluorescent1);
//     camera.SetExposureManual(38);   //0- Mbps 0.73, darker //100- Mbps .31, lighter 
}

void Robot::RobotPeriodic() 
{
    m_drivetrain->DrivetrainPeriodic();
    m_elevator->ElevatorPeriodic();
    m_collector->CollectorPeriodic();
    m_carriage->CarriagePeriodic();
    m_driverfeedback->DriverFeedbackPeriodic();
     Write2Dashboard();

    //m_drivetrain->DriveWithGamepad(); 
    //HEY try putting this line back in if something doesn't work JM
}

void Robot::DisabledInit() 
{
    std::cout<<"Disabled Init"<<std::endl;
    m_driverfeedback->RumbleOff();
}

void Robot::DisabledPeriodic() { frc::Scheduler::GetInstance()->Run(); }

void Robot::AutonomousInit() {
    std::cout<<"Auto Init"<<std::endl;
  // std::string autoSelected = frc::SmartDashboard::GetString(
  //     "Auto Selector", "Default");
  // if (autoSelected == "My Auto") {
  //   m_autonomousCommand = &m_myAuto;
  // } else {
  //   m_autonomousCommand = &m_defaultAuto;
  // }

  m_autonomousCommand = m_chooser.GetSelected();

  if (m_autonomousCommand != nullptr) {
    m_autonomousCommand->Start();
  }
}

void Robot::AutonomousPeriodic() { frc::Scheduler::GetInstance()->Run(); }

void Robot::TeleopInit() {
  std::cout<<"Teleop Init"<<std::endl;
  if (m_autonomousCommand != nullptr) {
    m_autonomousCommand->Cancel();
    m_autonomousCommand = nullptr;
  }
}

void Robot::TeleopPeriodic() { frc::Scheduler::GetInstance()->Run(); }

void Robot::TestPeriodic() {}

#ifndef RUNNING_FRC_TESTS
int main() { return frc::StartRobot<Robot>(); }
#endif

void Write2Dashboard(void)
{
    SmartDashboard::PutNumber("L_Motor",  Robot::m_drivetrain->GetLeftMotor()  );
    SmartDashboard::PutNumber("R_Motor",  Robot::m_drivetrain->GetRightMotor()  );

    SmartDashboard::PutNumber("D_L_Y_axis",  Robot::m_oi->DriverGamepad()->GetRawAxis(GAMEPADMAP_AXIS_L_Y)  );
    SmartDashboard::PutNumber("D_R_Y_axis",  Robot::m_oi->DriverGamepad()->GetRawAxis(GAMEPADMAP_AXIS_R_Y)  );
    SmartDashboard::PutNumber("D_L_X_axis",  Robot::m_oi->DriverGamepad()->GetRawAxis(GAMEPADMAP_AXIS_L_X)  );
    SmartDashboard::PutNumber("D_R_X_axis",  Robot::m_oi->DriverGamepad()->GetRawAxis(GAMEPADMAP_AXIS_R_X)  );

    SmartDashboard::PutNumber("D_L_Trig",    Robot::m_oi->DriverGamepad()->GetRawAxis(GAMEPADMAP_AXIS_L_TRIG)  );
    SmartDashboard::PutNumber("D_R_Trig",    Robot::m_oi->DriverGamepad()->GetRawAxis(GAMEPADMAP_AXIS_R_TRIG)  );

	SmartDashboard::PutNumber("LeftEnc",    Robot::m_drivetrain->GetLeftEncoder());
	SmartDashboard::PutNumber("RightEnc",   Robot::m_drivetrain->GetRightEncoder());  

    SmartDashboard::PutBoolean("Front_Photoeye", Robot::m_carriage->IsFrontPhotoeyeDetected());
    SmartDashboard::PutBoolean("Rear_Photoeye", Robot::m_carriage->IsRearPhotoeyeDetected());

    SmartDashboard::PutBoolean("Collector_Photoeye", Robot::m_collector->IsCollectorPhotoeyeDetected());

}