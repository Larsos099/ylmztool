#include <stdio.h>
#include <cstring>
#include <nsysnet/netconfig.h>
#include <coreinit/screen.h>
#include <vpad/input.h>
#include <nn/ac.h>
#include <whb/proc.h>
#include <unistd.h>
#include <wut.h>
#include <whb/log.h>
#include <whb/log_console.h>
#include <nn/ac.h>

int main(int argc, char **argv) {
    // Initialize everything
    WHBProcInit();
    VPADInit();
    OSScreenInit();
    WHBLogConsoleInit();
    nn::ac::Initialize();

    bool isBlueScreen = false;  // Flag to check if the screen should remain blue
    char ssid[256];  // Declare ssid once
    char passwd[256]; // Declare password buffer

    while (WHBProcIsRunning()) {
        VPADStatus vpad;
        VPADRead(VPAD_CHAN_0, &vpad, 1, NULL);

        if (vpad.trigger & VPAD_BUTTON_A) {
            // Read network config
            NetConfCfg cfg;
            nn::ac::ReadConfig(1, &cfg);
            NetConfWifiConfig wcfg;
            wcfg = cfg.wifi;
            NetConfWifiConfigData dwcfg = wcfg.config;

            // Copy the SSID
            strcpy(ssid, (const char*)dwcfg.ssid);

            
            size_t aes_key_size = sizeof(dwcfg.privacy.aes_key);
            memcpy(passwd, dwcfg.privacy.aes_key, aes_key_size);
            
        }

        
        OSScreenPutFontEx(SCREEN_DRC, 0, 1, ssid);
        OSScreenPutFontEx(SCREEN_TV, 0, 1, ssid);
        
       
        OSScreenPutFontEx(SCREEN_DRC, 0, 3, passwd);
        OSScreenPutFontEx(SCREEN_TV, 0, 3, passwd);

        OSScreenFlipBuffersEx(SCREEN_DRC);
        OSScreenFlipBuffersEx(SCREEN_TV);

        
        usleep(100000); 
    }

    WHBLogConsoleFree();
    WHBProcShutdown();
    OSScreenShutdown();
    VPADShutdown();
    nn::ac::Finalize();
    return 0;
}
