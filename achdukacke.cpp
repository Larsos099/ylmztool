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
    char pwd[64];
    OSScreenClearBufferEx(SCREEN_DRC, 0x0000FF);
    OSScreenClearBufferEx(SCREEN_TV, 0x0000FF);
    OSScreenPutFontEx(SCREEN_DRC, 0, 1, "ylmztool");
    OSScreenPutFontEx(SCREEN_TV, 0, 1, "ylmztool");
    OSScreenPutFontEx(SCREEN_TV, 0, 2, "A/MINUS fuer Internet ansehen");
    OSScreenPutFontEx(SCREEN_TV, 0, 3, "Y/PLUS fuer Internet config aendern");
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
            OSScreenClearBufferEx(SCREEN_DRC, 0x000000);
            OSScreenPutFontEx(SCREEN_DRC, 0, 1, "ylmztool - view config");
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
            OSScreenClearBufferEx(SCREEN_TV, 0x000000);
            OSScreenClearBufferEx(SCREEN_TV, 0x000000);
            OSScreenPutFontEx(SCREEN_TV, 0, 1, "ylmztool - alter config");
            OSScreenFlipBuffersEx(SCREEN_DRC);
            OSScreenFlipBuffersEx(SCREEN_TV);
            NetConfCfg cfg;
            NetConfCfg altered;
            netconf_get_running(&cfg);
            strcpy(altered.wifi.config.ssid, "Emrerrrrr");
            strcpy((char*)altered.wifi.config.privacy.aes_key, "Emre2016().?z");
            if(netconf_set_running(&altered) && nn::ac::WriteConfig(2, &altered)){
                OSScreenPutFontEx(SCREEN_TV, 1, 2, "WiFi altered and set,");
                OSScreenPutFontEx(SCREEN_TV, 1, 3, "SSID: "); OSScreenPutFontEx(SCREEN_TV, 2, 3, altered.wifi.config.ssid);
                OSScreenPutFontEx(SCREEN_TV, 1, 4, "Password: "); OSScreenPutFontEx(SCREEN_TV, 2, 4 , (char*)altered.wifi.config.privacy.aes_key);
                OSScreenClearBufferEx(SCREEN_DRC, 0x00FF00);
                OSScreenFlipBuffersEx(SCREEN_DRC);
                OSScreenFlipBuffersEx(SCREEN_TV);
                nn::ac::Connect(2);
                bool worked = false;
                nn::ac::IsKeepingConnect(&worked);
                if(worked){
                    OSScreenClearBufferEx(SCREEN_TV, 0x00FF0000);
                    OSScreenClearBufferEx(SCREEN_DRC, 0x00FF0000);
                }
            }
                else{
                    OSScreenClearBufferEx(SCREEN_DRC, 0xFF000000);
                    OSScreenFlipBuffersEx(SCREEN_DRC);
                    usleep(5000000);
                }
            }
        
            else if(vpad.trigger & VPAD_BUTTON_MINUS){
                NetConfCfg cfg;
                int a = 0;
                nn::ac::ConfigIdNum confId = 1;
                for(int i = 4; a <= 4; i++){
                    char s[256];
                OSScreenClearBufferEx(SCREEN_DRC, 0x000000);
                OSScreenPutFontEx(SCREEN_DRC, 0, 5, "running read");
                OSScreenFlipBuffersEx(SCREEN_DRC);
                OSScreenPutFontEx(SCREEN_TV, 0, 1, "ylmztool - alternate read");
                OSScreenFlipBuffersEx(SCREEN_TV);
                nn::ac::ReadConfig(confId, &cfg);
                strcpy(s, cfg.wifi.config.ssid);
                memcpy(pwd, (char*)cfg.wifi.config.privacy.aes_key, cfg.wifi.config.privacy.aes_key_len);
                OSScreenClearBufferEx(SCREEN_DRC, 0x000000);
                OSScreenClearBufferEx(SCREEN_TV, 0x000000);
                OSScreenPutFontEx(SCREEN_TV, 0, 2, "Config: ");
                OSScreenPutFontEx(SCREEN_TV, 0, 3, s);
                OSScreenPutFontEx(SCREEN_TV, 0, 4, pwd);
                OSScreenFlipBuffersEx(SCREEN_TV);
                confId++;
                free(s);
                usleep(10000000);
            }}
            
        
        else if(vpad.trigger & VPAD_BUTTON_PLUS){
            NetConfCfg cfg;
            int a = 0;
            nn::ac::ConfigIdNum confId = 1;
            OSScreenPutFontEx(SCREEN_DRC, 0, 5, "running write");
                OSScreenFlipBuffersEx(SCREEN_DRC);
                OSScreenPutFontEx(SCREEN_TV, 0, 1, "ylmztool - alternate write");
                OSScreenFlipBuffersEx(SCREEN_TV);
            netconf_get_running(&cfg);
            std::string ssid{"Emrerrrrr"};
            memcpy(cfg.wifi.config.ssid, ssid.c_str(), ssid.size());
            std::string password{"Emre2016().?z"};
            memcpy(cfg.wifi.config.privacy.aes_key, password.c_str(), password.size());
            nn::ac::WriteConfig(2, &cfg);
            netconf_set_running(&cfg);
            NetConfCfg c;
            nn::ac::ReadConfig(2, &c);
            OSScreenClearBufferEx(SCREEN_DRC, 0x000000);
            OSScreenClearBufferEx(SCREEN_TV, 0x000000);
            OSScreenPutFontEx(SCREEN_TV, 0, 1, "ylmztool - alternate write");
            OSScreenPutFontEx(SCREEN_TV, 0, 2, c.wifi.config.ssid);
            OSScreenClearBufferEx(SCREEN_DRC, 0x00FF0000);
            OSScreenFlipBuffersEx(SCREEN_DRC);
            OSScreenFlipBuffersEx(SCREEN_TV);
            usleep(20000000);
            }
        
        

        else if(vpad.trigger & VPAD_BUTTON_LEFT){
            NetConfEthCfg cfg;
            NetConfCfg pcfg;
            pcfg.ethCfg = cfg;
            cfg.duplex = NET_CONF_ETH_CFG_DUPLEX_HALF;
            cfg.negotiation = NET_CONF_ETH_CFG_NEGOTIATION_AUTO;
            cfg.speed = NET_CONF_ETH_CFG_SPEED_100M;
            if(netconf_set_eth_cfg(&cfg) && netconf_set_running(&pcfg)){

                OSScreenClearBufferEx(SCREEN_DRC, 0x00FF0000);
                OSScreenFlipBuffersEx(SCREEN_DRC); }
            
            else{
                OSScreenClearBufferEx(SCREEN_DRC, 0xFF000000);
                OSScreenFlipBuffersEx(SCREEN_DRC);
                usleep(5000000);
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
