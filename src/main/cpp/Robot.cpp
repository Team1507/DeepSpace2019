#include "Robot.h"
#include <frc/commands/Scheduler.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include "GamepadMap.h"
#include <cameraserver/CameraServer.h>
#include <frc/wpilib.h>
#include "frc/Compressor.h"

//#include <CameraServer.h> //new


//Autonomous Includes
#include "Commands/AutoDoNothing.h"
#include "Commands/AutoDriveStr8.h"



OI* Robot::m_oi;

Drivetrain* Robot::m_drivetrain;
Collector* Robot::m_collector;
Carriage* Robot::m_carriage;
Elevator *Robot::m_elevator;
DriverFeedback* Robot::m_driverfeedback;
Compressor* Robot::m_compressor;
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
    m_compressor      = new Compressor(1);
    //OI **MUST** be after all subsystem Inits
    m_oi = new OI();

    ahrs = new AHRS(SPI::Port::kMXP);
    
    //**** AUTONOMOUS CHOOSER ********
    m_chooser.SetDefaultOption("AutoDoNothing",  new AutoDoNothing()  );
    m_chooser.AddOption("AutoDriveStr8", new AutoDriveStr8() );

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
    m_carriage->OpenLatch();
    m_drivetrain->ResetEncoders();

    //CAMERA ~~~Replaced with rasberry pi 3/1/19~~~  
    // cs::UsbCamera camera = frc::CameraServer::GetInstance()->StartAutomaticCapture(0);
    // camera.SetConnectionStrategy(cs::VideoSource::kConnectionKeepOpen);
    // camera.SetResolution(160, 120); //was 320,240
    //     //camera.SetVideoMode(cs::VideoMode::kMJPEG, 320, 240, 8); //kGray, kRGB565
    // camera.SetFPS(15); //was 10
    // camera.SetBrightness(30);       //100-most bright //0- most dark //was 20
    //     //camera.SetVideoMode() //stinky line don't worry about it
    //     //camera.SetWhiteBalanceManual(cs::VideoCamera::kFixedFluorescent1);
    // camera.SetExposureManual(38);   //0- Mbps 0.73, darker //100- Mbps .31, lighter 
}

void Robot::RobotPeriodic() 
{
    m_drivetrain->DrivetrainPeriodic();
    m_elevator->ElevatorPeriodic();
    m_collector->CollectorPeriodic();
    m_carriage->CarriagePeriodic();
    m_driverfeedback->DriverFeedbackPeriodic();
    Write2Dashboard();


}

void Robot::DisabledInit() 
{
    std::cout<<"Disabled Init"<<std::endl;
    m_driverfeedback->RumbleOff();
    m_elevator->ClearReqElevatorValue();
}

void Robot::DisabledPeriodic() { frc::Scheduler::GetInstance()->Run(); }

void Robot::AutonomousInit() {
    std::cout<<"Auto Init"<<std::endl;
    //m_carriage->TiltDown();    NO Not here!  Must be Auto Command


    m_autonomousCommand = m_chooser.GetSelected();

    if (m_autonomousCommand != nullptr) {
        m_autonomousCommand->Start();
    }
}

void Robot::AutonomousPeriodic() 
{ 
    frc::Scheduler::GetInstance()->Run(); 

    //Check for Abort
    if( m_oi->DriverGamepad()->GetRawButtonPressed(GAMEPADMAP_BUTTON_Y) && (m_autonomousCommand != nullptr))
    {
        m_autonomousCommand->Cancel();
        m_autonomousCommand = nullptr;
        std::cout<<"Auto Aborted!!!!"<<std::endl; 
    }

}

void Robot::TeleopInit() 
{
    std::cout<<"Teleop Init"<<std::endl;
    m_carriage->TiltDown();    

    if (m_autonomousCommand != nullptr) {
        m_autonomousCommand->Cancel();
        m_autonomousCommand = nullptr;
    }
}

void Robot::TeleopPeriodic() 
{ 
    frc::Scheduler::GetInstance()->Run(); 

    //Check for driver Abort push and ignore in teleop
    //This only occurs in back-to-back auto testing
    if( m_oi->DriverGamepad()->GetRawButtonPressed(GAMEPADMAP_BUTTON_Y) )
    {
        //Do nothing.  Just ignore 
    }
}

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