# Introduction

This is (going to be) a fully-automatic, modular turntable, called the Statimatic STM-01. The development of this turntable is being documented in the following YouTube series: https://www.youtube.com/watch?v=k4UXI1rkMYs\&list=PL\_8A3xK4AntYpp8yXHuAbhb6lULQ2ivFA

# Planned Features
The built-in features of this turntable will be kept to a minimum, so that development can go more quickly. These features will be able to be expanded using the 5 expansion ports on the back of the turntable.

## Commands
Built-in, the plan is to have two commands: Play and pause. 

"Play" will move the tonearm from the "home" position to the outer edge of the record on the turntable. If the tonearm is not in the home position when "play" is pressed, then it will return home.

"Pause" will lift the tonearm up if it's set down, and set the tonearm down if it's lifted. "Up" movement will be quick, while "down" movement will be slow, to keep records from being damaged.

## Automatic pickup
The turntable will be able to automatically detect the end of the record, and pick itself up when it reaches that point.

## Size Selection
There will be four settings for size selection: 7", 10" and 12". The fourth option is "auto," where the turntable will use the built-in IR sensor to detect the record edge.

One caveat of using "auto" is that the record edge detection will likely be slower, while selecting a specific size will immediately know where to go.

## Speed Selection
There will be four settings for speed selection: 33 1/3, 45, and 78 RPM. The fourth option is "auto," where the turntable will guess, based on the record size, what speed you want.

For 12" records, the 33 1/3 RPM speed will be used. For 10" records, the 78 RPM speed will be used. For 7" records, the 45 RPM speed will be used. If an unusual size is detected, the fallback will be 78 RPM. Why? There are a lot of unusual 78-RPM record sizes out there, so I feel like this one's fitting.

## Statistics
The turntable will maintain a set of statistics that will be able to be viewed on the "Control Panel" module, or "Web Server" module.

These statistics could include:
- Current number of hours/minutes on the stylus
- How many records you've played
  - This will (likely) be counted by size and speed
- How long you've had the turntable powered on
- How long you've had the turntable playing a record

# Modularity

The plan is to allow connection of up to 5 external modules to the turntable, concurrently. The intention is to design the turntable as its own standalone unit, before any modules, so that each module can be designed in isolation.

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

# Progress

This project has a lot of work that needs done to it. Here's what's been completed so far, and what's left to do:
- [x] Prototype documentation
- [x] Lift mechanism
- [x] Pause/unpause routine
- [ ] Finalize tonearm assembly
- [ ] Status shift register
- [ ] Error handling
- [ ] Horizontal position tracking
- [ ] Horizontal clutch mechanism
- [ ] IR record/track detection
- [ ] Prototype motor/3-phase belt-driven turntable setup
- [ ] Finalized turntable setup (heavy platter and big pulley)
- [ ] Prototype plinth using plywood
- [ ] Finalized plinth using whatever material I decide to use
- [ ] Design PCB
- [ ] Etch PCB
- [ ] Celebrate a complete STM-01 turntable
