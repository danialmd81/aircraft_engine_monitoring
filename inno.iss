[Setup]
AppName=Aircraft Engine Monitoring
AppVersion=1.1.0
DefaultDirName={pf}\AircraftEngineMonitoring
DefaultGroupName=Aircraft Engine Monitoring
OutputDir=./windows_installer
OutputBaseFilename=aircraft_engine_monitoring_setup

[Files]
Source: ".\release\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs

[Icons]
Name: "{group}\Aircraft Engine Monitor"; Filename: "{app}\aircraft_engine_monitoring.exe"