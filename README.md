# Robust-Remote-Process-Control

# SystemMgr
The system manager will start the Child programs listed in the Configuration file. It will create a simple process table that stores the program name and PID of each process as it is started. When the Child process dies, the SystemMgr will restart that program. As Child programs are terminated, SystemMgr will print clear output indicating which program terminated (by name and PID). It will print clear output indicating which program (by name and PID) was started.

# ChildN
The Child can be a simple program. The Child will read the Configuration file to determine which UDP port it will listen on for requests to terminate. Upon request, it will simply exit. Each Child program will print clear output indicating its name and PID when it is asked to terminate, before it terminates itself.

# ProcKiller
This program will provide the user with option as to which Child to terminate. When selected, ProcKiller will simply send a UDP message to the appropriate Child asking it to terminate. It reads the available programs (and ports) to terminate from the Configuration file.

# SysConfigReader
This program will be responsible for reading the configuration file, similar to what `CProgs/common/socketsetup` currently does. Note that the tags in the Configuration are shown in **BOLD BLUE** below. Also note that the proc tags have multiple values, which is a difference from the current socketsetup program. You must define the location of the Configuration file using an environment variable that is used by SysConfigReader, just as the current socketsetup program does.

The required directory structure for this solution is as follows. Ensure that the makefiles in each folder support this structure.

|-FinalProject 
 | |-ProcKiller - contains the ProcKiller application 
 | |-Child - contains the Child applications 
 | |-common – contains the Configuration File reader application used by ProcKiller, SysManager and the Child programs
 | |-SysManager – contains the SysManager application 
 | |-config – contains the Configuration file read by ProcKiller, SysManager and the Child programs 
 | |-bin
