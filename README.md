# LuJim Passenger Driver

**Author:** LuJim Mods  
**Platform:** GTA San Andreas Android 2.00 + AML 1.3.0  
**ABI:** armeabi-v7a

## Idea

LuJim Passenger Driver allows the player to use a special passenger-driver mode:

- Hold the enter vehicle button for 3 seconds to enable/disable the mod.
- Tap normally to keep the original GTA behavior.
- When enabled, the player enters through the passenger side.
- A recruited gang member enters through the driver side and acts as the visual driver.
- The player controls the vehicle while seated as passenger.

## Config path

```text
Android_unprotected/data/com.rockstargames.gtasa/mods/configs/LuJimPassengerDriver.ini
```

If the file does not exist, the plugin creates it automatically.

## Default config

```ini
[LuJimPassengerDriver]
Enabled=0
HoldTime=3000
RequireRecruit=1
UseFirstRecruit=1
UseNearestRecruit=0
PatrolMode=1
RealisticEntry=1
ShowProgressBar=1
ShowNotifications=1
EnableLogs=1
WaitOutsideVehicle=1

[AllowedVehicles]
Cars=1
Police=1
Bikes=0
Boats=0
Planes=0
Helicopters=0
Custom19001_19200=1
```

## GitHub Actions build

Go to **Actions** -> **Build LuJim Passenger Driver** -> **Run workflow**.

The output artifact will contain:

```text
mods/libLuJimPassengerDriver.so
mods/configs/LuJimPassengerDriver.ini
```

## Important status

This is the stable project base with configuration, GitHub Actions, module structure, toggle logic, and placeholders for GTA SA Android hooks.

The next step is connecting the real GTA SA Android 2.00 symbols/offsets for:

- enter vehicle input
- player ped
- recruited ped group
- vehicle seat tasks
- passenger seat control redirection
- driver animation sync
