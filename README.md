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