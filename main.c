/*--------------------------------------------------------------------------------------------------------------------*/
//
// Driver Detector
//
#include <windows.h>
#include <stdio.h>
#include <psapi.h>
#include <string.h>
#define ARRAY_SIZE 1024 * 2
/*--------------------------------------------------------------------------------------------------------------------*/
//
// Driver Hunting Function
//
VOID DriverDetector(){
    //
    // Local var will be used
    //
    LPWSTR DriverPath[MAX_PATH * 2] = { 0 };
    PVOID Drivers[ARRAY_SIZE] = { 0 };
    DWORD CbNeeded = { 0 };
    LPWSTR DriverName[MAX_PATH] = { 0 };
    DWORD CountedDrivers = { 0 };

    //
    // Array of drivers to check "examples"
    //
    char* driversToSearch[] = {"raspptp.sys", "condrv.sys", "mrxsmb.sys", "MsMpEng.sys", "tcpip.sys", "ntfs.sys", "mimicatz.sys"};
    DWORD numDriversToSearch = sizeof(driversToSearch) / sizeof(driversToSearch[0]);

    //
    // Retrieves the load address for each device driver in the system
    //
    if(EnumDeviceDrivers(Drivers, sizeof(Drivers), &CbNeeded) && CbNeeded < sizeof(Drivers)){
        //
        // Getting how many drivers has been found
        //
        CountedDrivers = CbNeeded / sizeof(Drivers[0]);
        printf("\n\x1b[31m\tDrivers Found : (%d)\n\x1b[0m", CountedDrivers);
        printf("====================================\n");
        printf("\033[0;34m[i] Searching For Interesting Drivers...\n\033[0m");
        printf("====================================\n");

        for(int i = 0; i < CountedDrivers; i++){

            if(GetDeviceDriverBaseNameA(Drivers[i], DriverName, sizeof(DriverName) / sizeof(DriverName[0]))){
                //
                // Get the whole path
                //
                if(GetDeviceDriverFileNameA(Drivers[i], DriverPath, MAX_PATH)){
                    if(strstr(DriverPath, ".sys")){
                        //
                        // Uncomment this to list all the drivers
                        //
                        printf("%s \x1b[32mFound\x1b[0m !\n", DriverPath);

                    }
                }else{
                    printf("[!] GetDeviceDriverFileNameA Failed, ERROR->(%d) \n", GetLastError());
                }
            }else{
                printf("[!] GetDeviceDriverBaseNameA Failed, ERROR->(%d) \n", GetLastError());
            }
        }
        printf("====================================\n");
        printf("\033[0;34m[*] Interested Drivers Detected !\n\033[0m");
        printf("====================================\n");
        for(int j = 0; j < numDriversToSearch; j++){
            DWORD DriverFound = { 0 };
            for(int i = 0; i < CountedDrivers; i++){
                if(GetDeviceDriverBaseNameA(Drivers[i], DriverName, sizeof(DriverName) / sizeof(DriverName[0]))){
                    //
                    // Get the whole path
                    //
                    if(GetDeviceDriverFileNameA(Drivers[i], DriverPath, MAX_PATH)) {
                        if (strstr(DriverPath, ".sys")) {
                            //
                            // Check if the current driver matches any of the drivers in the search list
                            //
                            if (_strcmpi(DriverName, driversToSearch[j]) == 0) {
                                printf("\033[0;32m[+] Driver Found: \033[0m");
                                printf("\033[0;33m%s\033[0m\n", DriverPath);
                                DriverFound = 1;
                            }
                        }
                    }else{
                        printf("[!] GetDeviceDriverFileNameA Failed, ERROR->(%d) \n", GetLastError());
                    }
                }else{
                    printf("[!] GetDeviceDriverBaseNameA Failed, ERROR->(%d) \n", GetLastError());
                }
            } if (!DriverFound) {
                printf("\x1b[31m[-] Driver Not Found: \x1b[0m\x1b[37m%s\x1b[0m\n", driversToSearch[j]);
            }
        }
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
//
// Main Function
//
int main(){
    printf("====================================\n");
    printf("[i] Driver Detector Started...");
    DriverDetector();

    return 0;
}

