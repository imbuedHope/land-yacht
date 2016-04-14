#Setting up the BeagleBone Black

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