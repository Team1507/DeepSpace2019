#include "Robot.h"
#include <frc/commands/Scheduler.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include "GamepadMap.h"
#include <cameraserver/CameraServer.h>
#include <frc/wpilib.h>
#include "frc/Compressor.h"
#include "frc/PowerDistributionPanel.h"

//#include <CameraServer.h> //new


//Autonomous Includes
#include "Commands/AutoDoNothing.h"
#include "Commands/AutoDriveStr8.h"
#include "Commands/AutoLeftHatch.h"
#include "Commands/AutoRightHatch.h"



OI* Robot::m_oi;

Drivetrain* Robot::m_drivetrain;
Collector* Robot::m_collector;
Carriage* Robot::m_carriage;
Elevator *Robot::m_elevator;
DriverFeedback* Robot::m_driverfeedback;
Compressor* Robot::m_compressor;
PowerDistributionPanel *Robot::m_pdp;

void Write2Dashboard(void);

bool Robot::human_driver;   //Global Variable

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
    m_pdp             = new PowerDistributionPanel(12); //*** CHECK CAN BUS!!!!!

    //OI **MUST** be after all subsystem Inits
    m_oi = new OI();

    ahrs = new AHRS(SPI::Port::kMXP);
    
    //**** AUTONOMOUS CHOOSER ********
    m_chooser.SetDefaultOption("AutoDoNothing",  new AutoDoNothing()  );
    m_chooser.AddOption("AutoDriveStr8",  new AutoDriveStr8()  );
    m_chooser.AddOption("AutoLeftHatch",  new AutoLeftHatch()  );
    m_chooser.AddOption("AutoRightHatch", new AutoRightHatch() );

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
    human_driver = false;

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


    //****  COMPRESSOR FAULT CHECKS ****
    static int comp_count = 0;
    bool comp_fault = false;
    if( ++comp_count >= 50 )
    {
        comp_count = 0;
        //** NOTE ** If compressor problems, MOVE RADIO AWAY FROM PCM MODULE!!!!!
        if(m_compressor->GetCompressorCurrentTooHighFault() )
        {
            std::cout<<"Compressor Current Fault"<<std::endl;
            comp_fault = true;
        }
        if(m_compressor->GetCompressorCurrentTooHighStickyFault())
        {
            std::cout<<"PCM sticky faults cleared"<<std::endl;
            comp_fault = true;
            m_compressor->ClearAllPCMStickyFaults();
        }
        
        //Turn RED when fault.  Green = OK
        SmartDashboard::PutBoolean("Compressor Status", !comp_fault );
    }

}

void Robot::DisabledInit() 
{
    std::cout<<"Disabled Init"<<std::endl;
    m_driverfeedback->RumbleOff();
    m_elevator->ClearReqElevatorValue();
    human_driver = false;
}

void Robot::DisabledPeriodic() { frc::Scheduler::GetInstance()->Run(); }

void Robot::AutonomousInit() {
    std::cout<<"Auto Init"<<std::endl;
    m_carriage->TiltDown();    //NO Not here!  Must be Auto Command
    human_driver = false;   //In Auto, Human driver false until routiene completed


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
        human_driver = true;
    }

}

void Robot::TeleopInit() 
{
    std::cout<<"Teleop Init"<<std::endl;
    m_carriage->TiltDown();    
    human_driver = true;

    if (m_autonomousCommand != nullptr) {
        m_autonomousCommand->Cancel();
        m_autonomousCommand = nullptr;
    }
}

void Robot::TeleopPeriodic() 
{ 
    frc::Scheduler::GetInstance()->Run(); 

    //Check for driver Abort push and ignore in teleop
    //This only would occur in back-to-back auto testing
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

    // SmartDashboard::PutNumber("D_L_Y_axis",  Robot::m_oi->DriverGamepad()->GetRawAxis(GAMEPADMAP_AXIS_L_Y)  );
    // SmartDashboard::PutNumber("D_R_Y_axis",  Robot::m_oi->DriverGamepad()->GetRawAxis(GAMEPADMAP_AXIS_R_Y)  );
    // SmartDashboard::PutNumber("D_L_X_axis",  Robot::m_oi->DriverGamepad()->GetRawAxis(GAMEPADMAP_AXIS_L_X)  );
    // SmartDashboard::PutNumber("D_R_X_axis",  Robot::m_oi->DriverGamepad()->GetRawAxis(GAMEPADMAP_AXIS_R_X)  );

    // SmartDashboard::PutNumber("D_L_Trig",    Robot::m_oi->DriverGamepad()->GetRawAxis(GAMEPADMAP_AXIS_L_TRIG)  );
    // SmartDashboard::PutNumber("D_R_Trig",    Robot::m_oi->DriverGamepad()->GetRawAxis(GAMEPADMAP_AXIS_R_TRIG)  );

	SmartDashboard::PutNumber("LeftEnc",    Robot::m_drivetrain->GetLeftEncoder());
	SmartDashboard::PutNumber("RightEnc",   Robot::m_drivetrain->GetRightEncoder());  

    SmartDashboard::PutBoolean("Front_Photoeye", Robot::m_carriage->IsFrontPhotoeyeDetected());
    SmartDashboard::PutBoolean("Rear_Photoeye", Robot::m_carriage->IsRearPhotoeyeDetected());

    SmartDashboard::PutBoolean("Collector_Photoeye", Robot::m_collector->IsCollectorPhotoeyeDetected());

    SmartDashboard::PutBoolean("Human Drive Enable", Robot::human_driver );

    //Compressor Debug
    SmartDashboard::PutNumber("Comp Current",   Robot::m_compressor->GetCompressorCurrent() );  
    SmartDashboard::PutNumber("Voltage",        Robot::m_pdp->GetVoltage() );  
}