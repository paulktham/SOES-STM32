## Introduction

This repository aims to help beginners to EtherCAT like myself to be able to implement a EtherCAT slave using Simple Open Source EtherCAT Slave (SOES). The EtherCAT Slave Controller (ESC) used in this example is a LAN9252 by MicroChip, the microcontroller used is a STM32F446RET6. This guide assumes basic knowledge of working with a microcontroller.

You can clone this repository and open the project with STM32CubeIDE to view this project better.

Please just use my code as a reference as our PDOs are very likely to be different and will cause differing results. The code is just to give a general idea of how things are meant to flow.

## How to implement a EtherCAT slave (SOES)

1. Go to [EEPROM Generator](https://kubabuda.github.io/EEPROM_generator/)
   1. Input the necessary information, key things to be accurate:
      1. ESC chip type
      2. SPI mode
   2. Download all the files generated
2. Update the EEPROM connected to the ESC using EtherCAT Technology Group’s Slave Stack Code (SSC)

   1. Connect the ESC port to the computer with an ethernet cable (RJ45)
   2. Use the EEPROM Programmer tool provided by SSC to scan for the slave
   3. Click on File → Open then select the .xml file created by the EEPROM Generator
   4. click on Slaves → Program Selected to upload the esi to the eeprom

   > **Alternative Option:**
   > If you do not have access to the EtherCAT Technology Group’s **Slave Stack Code (SSC)**, you can also use **Beckhoff TwinCAT** as an alternative tool for generating and programming the ESI/EEPROM files. While the workflow and interface may differ slightly, many developers have reported success using TwinCAT for configuring and flashing EtherCAT slaves.

3. Create a STM32 Project configured for the chip at hand (SPI comms)
4. Write helper functions for reading the ESC chip registers, directly and indirectly (read datasheet to understand the requirements for each chip)

   **Follow naming conventions for these function:**

   ```jsx
   // Hardware initialization
   void ESC_init(const esc_cfg_t *config);

   // Basic read/write functions
   void ESC_read(uint16_t address, void *buf, uint16_t len);
   void ESC_write(uint16_t address, void *buf, uint16_t len);

   // Interrupt handling
   void ESC_interrupt_enable(uint32_t mask);
   void ESC_interrupt_disable(uint32_t mask);
   ```

   > **For my implementation for reading registers it was kind of weird, because I needed to rearrange the bytes received in a weird manner for it to read data properly**

   ```jsx
   uint32_t LAN9252_ReadReg32(uint16_t address)
   {
       uint8_t cmd[4];
       uint8_t recv[4];

       cmd[0] = 0x03;
       cmd[1] = (address >> 8) & 0xFF;
       cmd[2] = address & 0xFF;
       cmd[3] = 0x00;  // Dummy byte

       LAN9252_CS_LOW();
       HAL_SPI_Transmit(&hspi1, cmd, 4, HAL_MAX_DELAY);
       HAL_SPI_Receive(&hspi1, recv, 4, HAL_MAX_DELAY);
       LAN9252_CS_HIGH();

       // LAN9252 returns data in WEIRD FORMAT
       return (recv[2] << 24) | (recv[1] << 16) | (recv[0] << 8) | recv[3];
   }
   ```

5. Add the necessary SOES files into the STM project

   1. Clone the repository
      ```jsx
      git clone https://github.com/OpenEtherCATsociety/SOES.git
      ```
   2. Copy the header files into inc and c files into src
   3. Add these into the main.c

      ```jsx
      static esc_cfg_t config = {
          .user_arg = NULL,
          .use_interrupt = 0,  // Set to 1 if you want to use interrupts
          .watchdog_cnt = 150, // Watchdog counter
          .set_defaults_hook = NULL,
          .pre_state_change_hook = NULL,
          .post_state_change_hook = NULL,
          .application_hook = NULL,
          .safeoutput_override = NULL,
          .pre_object_download_hook = NULL,
          .post_object_download_hook = NULL,
          .rxpdo_override = NULL,
          .txpdo_override = NULL,
          .esc_hw_interrupt_enable = NULL,
          .esc_hw_interrupt_disable = NULL,
          .esc_hw_eep_handler = NULL,
          .esc_check_dc_handler = NULL,
      }; // Depends on your needs

      void cb_set_outputs (void){
      // TODO: Implementation
      }

      void cb_set_inputs (void){
      // TODO: Implementation
      }

      int main(void)
      {
      ...
      (Setup)
      ...

        ecat_slv_init(&config);
        while (1)
        {
      	  ecat_slv();
        }
      }
      ```

   4. **_IMPORTANT INCLUSION_** in ecat_slv.c

      ```jsx
      void ecat_slv_poll (void)
      {
         /* Read local time from ESC*/
         ESC_read (ESCREG_LOCALTIME, (void *) &ESCvar.Time, sizeof (ESCvar.Time));
         ESCvar.Time = etohl (ESCvar.Time);

         /* ADD THE CODE BELOW */

         /* READ AL EVENT REGISTER - THIS IS CRITICAL! */
         CC_ATOMIC_SET(ESCvar.ALevent, ESC_ALeventread());

         /* Clear the events we're about to process */
         if (ESCvar.ALevent)
         {
            ESC_ALeventwrite(ESCvar.ALevent);
         }

         /* UNTIL HERE */

         /* Check the state machine */
         ESC_state();
         /* Check the SM activation event */
         ESC_sm_act_event();
         .
         ...
         .
         }
      ```

      > This was just how I solved the error I was facing where the slave was unable to change its operating state. I am sure there are many other methods to solve the same issue

6. Build the STM project and flash in onto the chip, make sure that the chip is wired correctly to the ESC board

## Contributions

Feel free to contribute by creating PRs :)

## Acknowledgements & Disclaimer

This project makes use of the **Simple Open Source EtherCAT Slave (SOES)** library, which is an open-source implementation maintained by the [OpenEtherCAT Society](https://github.com/OpenEtherCATsociety/SOES).

EtherCAT® is a registered trademark and patented technology, licensed by **Beckhoff Automation GmbH & Co. KG**, Germany. This repository and its contents are **not affiliated with, endorsed by, or certified by Beckhoff Automation or the EtherCAT Technology Group (ETG)** in any way.

All source code, documentation, and examples provided here are for **educational and experimental purposes only**. Users are responsible for ensuring compliance with any relevant licensing, safety, or certification requirements when deploying EtherCAT-based systems in commercial or production environments.

Special thanks to the developers and maintainers of the SOES project and to the wider open-source EtherCAT community for their contributions and resources that made this learning project possible.


