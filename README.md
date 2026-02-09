# Hunch Project 2025-2026

## Description
A robotics/mechanical system designed for this year's NASA HUNCH "Lunar Jumping Robot" challenge. Includes hardware, software, and control systems.

## Goals
- Develop a reliable control system for the final Jumping robot.
- Test and iterate within 20 weeks following timeline milestones.

## Team
- **Asher VerLee** - Team Lead, Electrical Systems Lead, Program Organizer, Machine Learning
- **Austin Ledin** - Programming Member, R&D Lead, Build Regulation Expert
- **Jack LaFreniere** - Programming Lead, Electrical Member, 3D render expert
- **Hunter Carlson** - Programming Member, Part Fabrication lead, Materials Expert. 

# Setup

To ensure that you have the correct environment setup, we made a simple step-by-step guide below so you can edit, build, and deploy code onto our Teensy 4.1 board.

## VS Code
- I would suggest using VS Code in this instance, even though you could use any, it would make it a lot harder to build and deploy code onto the board. Go to [https://code.visualstudio.com/download](https://code.visualstudio.com/download)
- Along with that, you will need to download Git, which you can find here [https://git-scm.com/install/](https://git-scm.com/install/), and, although optional, GitHub Desktop, found here [https://desktop.github.com/download/](https://desktop.github.com/download/)

#### VS Code Extensions
Since we are using the Teensy 4.1, we will need some extensions to help us build and deploy code on the board.
Documentation can be found for the board at [https://docs.platformio.org/en/latest/boards/teensy/teensy41.html](https://docs.platformio.org/en/latest/boards/teensy/teensy41.html)
If you don't want to figure out how to set that up all by yourself, I've got you.
- In VS Code, open the package manager or extensions manager.
- Search for the official PlatformIO IDE and click install.
- You will then want to go to the top search bar and search >PlatformIO:Focus on Quick Access View (It should pop up). If it asks to download anything, say yes, if it's in VS Code
- This should then open PIO Home. 
- You can now open your clone, or clone the repo and open it in VS Code
- You now want to go to the PIO Home by clicking the home button in the bottom right of your screen
- Now press open project, and select your clone of the repo, and it should open the clone in a PlatformIO project, making it so you can build and deploy code without going into any other editor.

## Code Architecture

project_root/
├── lib/
│   ├── JumpingMotors/
│   │   ├── JumpingMotors.h
│   │   └── JumpingMotors.cpp
│   └── BalanchingSensor/
│   │   ├── BalanchingSensor.h
│   │   └── BalanchingSensor.cpp
│   └── FlyWheelMotors/
│   │   ├── FlyWheelMotors.h
│   │   └── FlyWheelMotors.cpp
├── src/
│   └── main.cpp
└── platformio.ini



