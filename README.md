#Setting up the BeagleBone Black

1. Install Debian 7.9 image to SD card.
2. Set device IP to static (currently 192.168.0.5).
3. Set up users as shown in `info` file.
4. `git clone` this repo as hotsail home directory.
5. Edit `~/.profile`, append:

```bash
export SLOTS=/sys/devices/bone_capemgr.9/slots
export PINS=/sys/kernel/debug/pinctrl/44e10800.pinmux/pins
```

6. Run `visudo` as root, under `Defaults	env_reset` add two lines:

```bash
Defaults        env_keep += "SLOTS"
Defaults        env_keep += "PINS"
```

7. Now the Beaglebone is set up to easily add device tree overlays.