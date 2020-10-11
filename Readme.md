# Onethinx LoRa To LoRa communication example
   
   ![LoRa to LoRa](../assets/LoRa-to-LoRa.gif?raw=true)<br/>
   

1. Prerequisites
    - Onethinx LoRaWAN Core Module (minimum stackversion: 0x000000BC)
    - Visual Studio Code
    - Onethinx Core VS Code dependencies pack (compiler, Cypress tools, Cypress PDL, linkersettings etc.)
      - [VSCode_OnethinxPack_macOS](https://github.com/onethinx/VSCode_OnethinxPack_macOS)
      - [VSCode_OnethinxPack_Windows](https://github.com/onethinx/VSCode_OnethinxPack_Windows)
      - [VSCode_OnethinxPack_Linux](https://github.com/onethinx/VSCode_OnethinxPack_Linux)

2. Building the example
   - Clean Reconfigure All Projects<br/>
   ![Clean Reconfigure](../assets/CleanReconfigure.png?raw=true)<br/>
     *if you get errors, try to delete the contents of the build/ folder*
   - Build<br/>
   ![Build](../assets/Build.png?raw=true)
   - Debug<br/>
   ![Debug](../assets/Debug.png?raw=true)
   
3. Onethinx Core API
   - This command should be used to transmit data over LoRa<br>
   `LoRa_TX(&LoRaParams, &RadioStatus, (uint8_t *) &LoRaTXbuffer, sizeof(LoRaTXbuffer), 3000);`
   - This command should be used to receive data over LoRa<br>
   `LoRa_RX(&LoRaParams, &RadioStatus, (uint8_t *) &LoRaRXbuffer, sizeof(LoRaRXbuffer), 30000);`
   
   The LoRaParams are holding the LoRa parameters (frequency, spreading factor etc).
  
