# Bodycam Internal Cheat
![image](https://github.com/user-attachments/assets/71223b8d-946c-4aa2-9c67-06a9268138b2)

## How to update
- Download [Dumper-7](https://github.com/Encryqed/Dumper-7) source and build it
- Inject the Dumper-7.dll into the game process "Bodycam-Win64-Shipping.exe"
- Go to the dumper output directory, it should be "C:\Dumper-7" and open the dump folder
- Drop the entire CppSDK folder content into the "Bodycam Internal" project folder replacing all files
- Open the project and build it on Release, remove all lines that cause compilation errors, and rebuild.
> [!NOTE]  
> You must have Microsoft DirectX SDK, download it [here](https://www.microsoft.com/download/details.aspx?id=6812)

> [!WARNING]  
> Go to the project settings -> C/C++ -> All Options and add `/bigobj` under Additional Options

## Features
### Aimbot
- Hitboxes (Head/Body)
- Show Fov
- Fov Slider
- Aim Line
- Instant Kill
- No Aiming Restrictions
- Instant Aiming

### Visuals
- Visible ESP
- Not Visible ESP
- Snaplines (Top/Middle/Bottom)
- Players Chams
- Local Chams
- Weapon Chams
- Nickname
- Health Bar
- Team Check
- No Flash
- Drones ESP
- Bomb ESP

### Exploits
- View Fov Slider
- Gravity Multiplier Slider (must be host)
- No Recoil
- Rapid Fire
- Full Auto
- Unlimited Ammo
- GodMode (must be host)


### Misc
- Show Mouse
- Suicide

## Credits
- [Valgar1](https://github.com/valgar12) - Cheat was developed using his work as a reference, with his support.
- [Kiero Hook](https://github.com/rdbo/ImGui-DirectX-11-Kiero-Hook) - The foundational base for this project.
- [Dumper-7](https://github.com/Encryqed/Dumper-7) - The tool that makes you wonder if the game was ever closed source.
