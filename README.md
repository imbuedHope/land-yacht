##Current Project Status

CAD models for the project can be found at https://grabcad.com/library/land-yacht-2/ . Components are mounted, and PWM and encoder control are working. Needs to be done:

1. Get WiFi adapter working. Right now if you `ifup wlan0` with the current `/etc/network/interfaces` and `/etc/wpa_supplicant/wpa_supplicant.conf` settings, you will connect to `planets`, but it is not working straight from boot.
1. Confirm server-client communication is working - fold in Sriram's work.
1. Write some controllers!
1. Get GPS working with the SPI pins on the BeagleBone (I think it's SPI?).
1. Download Exploring BeagleBone book if you want to learn more.

##Connecting to the BeagleBone Black
A script `connect-ssh` has been configured to connect to the BeagleBone using its ethernet adapter MAC address. Run `./connect-ssh <user>` to use the script.

##Setting up the BeagleBone Black

1. Install Debian 7.9 image to SD card.
1. Set device IP to static (currently 192.168.0.5).
1. Set up users as shown in `info` file.
1. `git clone` this repo as hotsail home directory.
1. Edit `~/.profile`, append:

    ```bash
    export SLOTS=/sys/devices/bone_capemgr.9/slots
    export PINS=/sys/kernel/debug/pinctrl/44e10800.pinmux/pins
    ```

1. Run `visudo` as root, under `Defaults	env_reset` add two lines:

    ```bash
    Defaults        env_keep += "SLOTS"
    Defaults        env_keep += "PINS"
    ```

1. Now the Beaglebone is set up to easily add device tree overlays.
1. f you want to auto disable/enable device tree overlays on boot, run these commands as root:

    ```bash
    mkdir /mnt/boot
    mount /dev/mmcblk0p1 /mnt/boot
    vim /mnt/boot/uEnv.txt
    ```

1. Add these lines above the `mmcargs` line:

	```bash
	##Disable HDMI, enable PWM P9_22
	optargs=capemgr.disable_partno=BB-BONELT-HDMI,BB-BONELT-HDMIN capemgr.enable_partno=am33xx_pwm,bone_pwm_P9_22
	```

1. Change the `capemgr.enable/disable_partno` to the overlays you'd like to enable or disable.
1. To use the Exploring BeagleBone library, there are a few steps needed.
	1. You will need to have `cmake` installed, which may or may not be in the repositories. I had to build it from source.
	1. You will need to run `cmake .. && make && sudo make install` in the `exploringBBlibrary/build` folder. If you're running into trouble look at "https://github.com/derekmolloy/exploringBB/tree/master/library".

## Examples

1. To enable PWM P9_22 and P9_16 on boot

    ```bash
    optargs=capemgr.disable_partno=BB-BONELT-HDMI,BB-BONELT-HDMIN capemgr.enable_partno=am33xx_pwm,bone_pwm_P9_22,bone_pwm_P9_16
    ```

1. To enable eQEP2b (encoder support) on boot

    ```bash
    add CAPE=PyBBIO-eqep2b to /etc/default/capemgr
    ```

1. The eQEP2b position can be accessed from

    ```bash
    /sys/devices/ocp.3/48304000.epwmss/48304180.eqep/position
    ```

    and it appears as if the every 360<sup>o</sup> is 4100 counts.