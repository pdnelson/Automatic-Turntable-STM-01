# The Statimatic STM-01

This is (going to be) a fully-automatic, modular turntable, called the Statimatic STM-01. The development of this turntable is being documented in the following YouTube series: [https://www.youtube.com/watch?v=k4UXI1rkMYs\&list=PL\_8A3xK4AntYpp8yXHuAbhb6lULQ2ivFA](https://www.youtube.com/playlist?list=PL_8A3xK4AntYpp8yXHuAbhb6lULQ2ivFA)

At this time, an AR-XA turntable (left) is being used as the fill-in "turntable" for the STM-01 (right) during development. Eventually, the STM-01 will get its own turtable
<img width="4032" height="2268" alt="20260521_185642" src="https://github.com/user-attachments/assets/cd819c7b-82fa-4b46-9678-90f65ebb569b" />

## Electronics
This project is driven by a Teensy 4.1, three stepper motors to facilitate movement, multiplexers, a shift register, and more! See the `electrical` folder for the full schematic (so far).

Development is progressing on a breadboard, with questionable organization. Eventually, the plan is to etch a custom PCB.
<img width="4032" height="2268" alt="20260521_190106" src="https://github.com/user-attachments/assets/b46482c6-eddd-4caf-a0df-032dace4a9f7" />

### Schematic

This is the schematic as of 7 June 2026. The objective is to eliminate as much multiplexing as possible at the end of the project, if there are extra Teensy pins left over. The movement stepper demultiplexers (part `SN74LVC1G18DBVR`) will be the first to go. So far, it's looking like I may be able to eliminate these, as their use only saves me 3 pins. I'm not going to pull the trigger on this quite yet, though, because I may encounter other difficulties later on in the project, requiring the use of more pins. It's up in the air at this point.
<img width="1590" height="1038" alt="schematic" src="https://github.com/user-attachments/assets/157d63a6-9e6b-4a52-a04b-6f32d3de2962" />


## Mechanisms
The automatic tonerm's operation is facilitated by two mechanisms: One for vertical movement (right), and one for horizontal (left).

The vertical axis has a stepper motor driving movement, and a 10k slide potentiometer acting as an absolute encoder, so its position can be monitored.

The horizontal axis has a stepper motor driving horizontal movements, and another stepper motor to engage/disengage the horizontal motor for automatic movement. This mechanism is currently in progress, so there is some more work to do here.

<img width="4032" height="2268" alt="20260521_190203" src="https://github.com/user-attachments/assets/3c295208-4761-462c-82d0-73f3fe41d063" />

## Previous Turntable
I designed a prototype turntable previously, separately from this repository, around late 2020/early 2021. That prototype lives in [another repository](https://github.com/pdnelson/Automatic-Record-Player). Though I'm no longer working on that project, much of the research, and many of the ideas I experimented with have made their way into this project.

## Planned Features
The built-in features of this turntable will be kept to a minimum, so that development can go more quickly. These features will be able to be expanded using the 5 expansion ports on the back of the turntable.

### Commands
Built-in, the plan is to have two commands: Play and pause. 

"Play" will move the tonearm from the "home" position to the outer edge of the record on the turntable. If the tonearm is not in the home position when "play" is pressed, then it will return home.

"Pause" will lift the tonearm up if it's set down, and set the tonearm down if it's lifted. "Up" movement will be quick, while "down" movement will be slow, to keep records from being damaged.

### Automatic pickup
The turntable will be able to automatically detect the end of the record, and pick itself up when it reaches that point.

### Size Selection
There will be four settings for size selection: 7", 10" and 12". The fourth option is "auto," where the turntable will use the built-in IR sensor to detect the record edge.

One caveat of using "auto" is that the record edge detection will likely be slower, while selecting a specific size will immediately know where to go.

### Speed Selection
There will be four settings for speed selection: 33 1/3, 45, and 78 RPM. The fourth option is "auto," where the turntable will guess, based on the record size, what speed you want.

For 12" records, the 33 1/3 RPM speed will be used. For 10" records, the 78 RPM speed will be used. For 7" records, the 45 RPM speed will be used. If an unusual size is detected, the fallback will be 78 RPM. Why? There are a lot of unusual 78-RPM record sizes out there, so I feel like this one's fitting.

### Statistics
The turntable will maintain a set of statistics that will be able to be viewed on the "Control Panel" module, or "Web Server" module.

These statistics could include:
- Current number of hours/minutes on the stylus
- How many records you've played
  - This will (likely) be counted by size and speed
- How long you've had the turntable powered on
- How long you've had the turntable playing a record

### Modularity

The plan is to allow connection of up to 5 external modules to the turntable, concurrently. The intention is to design the turntable as its own standalone unit, before any modules, so that each module can be designed in isolation.

<img width="1368" height="823" alt="mod-all-modules-clock" src="https://github.com/user-attachments/assets/7fb54b00-15cf-4e43-a67d-0cf0cc34333e" />


These could be, for example:
- Record flipper
- Record changer
- Second tonearm
- FM transmitter
- Preamp
- Alarm clock
- Remote control
- Splitter (to allow for even more modules!)
- Communication adapter to your favorite vintage computer from the '70s or '80s
- Control panel
- Web server

... And so on!

Got an idea for a module that you think would be cool? Send me a message; I might agree! Though I'm not developing any modules yet, it will be good to build up a nice list of things so I never run out of things to work on.

## Progress

This project has a lot of work that needs done to it. Here's what's been completed so far, and what's left to do:
- [x] Prototype documentation
- [x] Lift mechanism
- [x] Pause/unpause routine
- [x] Design pivoting mechanism
- [x] Design tonearm and headshell assembly
- [x] Connect tonearm to electronics
- [x] Status shift register
- [x] Command Button UI
- [x] Error handling
- [x] External Commands
- [x] Horizontal clutch mechanism
- [ ] Horizontal position tracking
- [ ] "Specific Size" play routine
- [ ] IR record/track detection
- [ ] "Automatic" play routine
- [ ] Automatic homing routine
- [ ] Track forward/backward routine
- [ ] Headshell capacitive touch sensor
- [ ] Audio cut-off circuit
- [ ] Prototype motor/3-phase belt-driven turntable setup
- [ ] Speed monitoring routine
- [ ] Finalized turntable setup (heavy platter and big pulley)
- [ ] Design power supply
- [ ] Save settings when powered off
- [ ] Prototype plinth using plywood
- [ ] Finalized plinth using whatever material I decide to use
- [ ] Design PCB
- [ ] Etch PCB
- [ ] Celebrate a complete STM-01 turntable





