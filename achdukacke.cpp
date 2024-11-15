#include <stdio.h> //ylmztool, Lars Lindner, 15/11/2024
#include <cstring> // C++, devkitPro Wii U Toolchain (wut)
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
    char ssid[256]; 
    char pwd[256];
    OSScreenClearBufferEx(SCREEN_DRC, 0x0000FF);
    OSScreenClearBufferEx(SCREEN_TV, 0x0000FF);
    OSScreenPutFontEx(SCREEN_DRC, 0, 1, "ylmztool");
    OSScreenPutFontEx(SCREEN_TV, 0, 1, "ylmztool");
    OSScreenPutFontEx(SCREEN_TV, 0, 2, "A fuer Internet ansehen");
    OSScreenPutFontEx(SCREEN_TV, 0, 3, "Y fuer Internet config aendern");
    OSScreenPutFontEx(SCREEN_TV, 0, 4, "DPAD links fuer ethernet");
    OSScreenFlipBuffersEx(SCREEN_DRC);
    OSScreenFlipBuffersEx(SCREEN_TV);
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
            memcpy(pwd, dwcfg.privacy.aes_key, aes_key_size);
            // Display SSID text
            OSScreenClearBufferEx(SCREEN_DRC, 0x0000FF);
            OSScreenPutFontEx(SCREEN_DRC, 0, 2, ssid);
            OSScreenPutFontEx(SCREEN_TV, 0, 2, ssid);
            OSScreenPutFontEx(SCREEN_DRC, 0, 3, pwd);
            OSScreenPutFontEx(SCREEN_TV, 0, 3, pwd);
            // Flip buffers to display the updated screen
            OSScreenFlipBuffersEx(SCREEN_DRC);
            OSScreenFlipBuffersEx(SCREEN_TV);
            // Add a small delay for responsiveness, to avoid a too fast loop
            usleep(1000000);  // Sleep for 100ms (adjust as needed)
        }
        else if(vpad.trigger & VPAD_BUTTON_Y){
            NetConfCfg cfg;
            NetConfCfg altered;
            netconf_get_running(&cfg);
            strcpy(altered.wifi.config.ssid, "Emrerrrrr");
            strcpy((char*)altered.wifi.config.privacy.aes_key, "Emre2016.()?z");
            if(netconf_set_running(&altered)){
                OSScreenPutFontEx(SCREEN_TV, 1, 10, "WiFi altered and set,");
                OSScreenPutFontEx(SCREEN_TV, 1, 11, "SSID: "); OSScreenPutFontEx(SCREEN_TV, 2, 11, altered.wifi.config.ssid);
                OSScreenPutFontEx(SCREEN_TV, 1, 12, "Password: "); OSScreenPutFontEx(SCREEN_TV, 2, 12 , (char*)altered.wifi.config.privacy.aes_key);
                OSScreenClearBufferEx(SCREEN_DRC, 0x00FF00);
                OSScreenFlipBuffersEx(SCREEN_DRC);
                OSScreenFlipBuffersEx(SCREEN_TV);
            }
            else{
                OSScreenClearBufferEx(SCREEN_DRC, 0xFF0000);
                OSScreenFlipBuffersEx(SCREEN_DRC);
            }
        }
        else if(vpad.trigger & VPAD_BUTTON_LEFT){
            NetConfEthCfg cfg;
            cfg.duplex = NET_CONF_ETH_CFG_DUPLEX_HALF;
            cfg.negotiation = NET_CONF_ETH_CFG_NEGOTIATION_AUTO;
            cfg.speed = NET_CONF_ETH_CFG_SPEED_100M;
            if(netconf_set_eth_cfg(&cfg)){
                OSScreenClearBufferEx(SCREEN_DRC, 0x00FF00);
                OSScreenFlipBuffersEx(SCREEN_DRC);
            }
            else{
                OSScreenClearBufferEx(SCREEN_DRC, 0xFF0000);
                OSScreenFlipBuffersEx(SCREEN_DRC);
            }
        }

        

            
    }

    WHBLogConsoleFree();
    WHBProcShutdown();
    OSScreenShutdown();
    VPADShutdown();
    nn::ac::Finalize();
    return 0;
}
