# X-Tinguish — Autonomous Fire-Extinguishing Robot

An autonomous embedded robot built on an Arduino Uno that detects and extinguishes small fires using a pivoting tower equipped with an IR flame sensor and a water nozzle.

Built as a group project for the **Autonomous Systems** course at Tilburg University (2024). Final grade: **9/10**.

See [`src/x_tinguish.ino`](src/x_tinguish.ino) for the full annotated Arduino code.

## What it does

X-Tinguish continuously rotates its sensor tower to scan its environment for flames. When a fire is detected:

1. Servo rotation halts and locks the nozzle on target.
2. A relay-driven 9V water pump activates, suppressing the fire.
3. A yellow LED and buzzer trigger to alert nearby people.

It also monitors its own water reservoir using two probe pins. When water drops below a threshold, a blue LED blinks to signal the supply is running low — preventing silent operational failures.

## Hardware

- Arduino Uno (ATmega328P)
- 3-pin IR flame sensor (digital)
- Servo motor (~180° rotation range) for the scanning tower
- 9V water pump (powered externally via relay; the Arduino cannot supply this current directly)
- Relay module to isolate pump current from MCU
- Two-prong wire-probe water-level sensor
- Yellow LED + buzzer (fire alert), blue LED (low-water alert)
- 9V batteries (one for the Arduino, one for the pump)

The relay was a deliberate hardware design choice: it lets the Arduino control the pump via a digital signal without bearing the load itself, which would damage the board.

## Software

Written in C++ in the Arduino IDE. The control logic is event-driven: the main loop continuously reads the flame sensor and water-level probe, then dispatches behaviour based on state. Flame detection is debounced to prevent spurious triggers from ambient IR sources (e.g., direct sunlight).

Key implementation details:
- Pin-level digital I/O for all sensors and actuators
- Servo scanning controlled via incremental position updates
- Non-blocking LED blink using `millis()` (rather than `delay()`) so the blink doesn't stall the scan loop
- Sensor-triggered state transitions: scanning → flame detected → suppression → return to scanning

See `xtinguish.ino` for the full annotated code.

## What I worked on

Group project of five. My contributions spanned coding, hardware assembly, debugging, and writeup:

- **Code**: Co-wrote the C++ control logic with the team — flame-detection branching, servo scanning loop, water-pump triggering, and the non-blocking LED blink for the low-water indicator. Reviewed and refined teammates' code during integration.
- **Debugging**: Tuned flame-sensor threshold values empirically to suppress false positives from ambient IR (especially direct sunlight). Tested debounce timing and helped diagnose servo behaviour during the integration phase.
- **Hardware**: Helped assemble the physical robot — wiring sensors, mounting components, and verifying connections during build sessions.
- **Writeup**: Co-authored the final report, particularly the theoretical framing (autonomy, sensor-based decision making, adaptive control) and the discussion section linking implementation choices to the robotics literature.

The hardware build was led by Soham (who hosted the components); I worked closely with him and the rest of the team across all phases.

## Theoretical framing

The system demonstrates several core robotics concepts at an introductory level:
- **Autonomy** — operates without human intervention once started
- **Sensor-based decision making** — perception-action loops processing flame sensor input in real time
- **Adaptive behaviour** — modifies actions based on detection state and reports its own operational health (low-water alert)
- **Behaviour-based robotics** — visible scanning + audio-visual cues create perceived intentionality

## Demo

[Short video showing the robot in action](https://youtu.be/CD0NKUrg6g0)

## Limitations and future work

Known limitations:
- Single flame sensor → no spatial prioritisation if multiple fires are present
- Servo range physically limited to ~180°
- Water hose occasionally tangles during rotation — a mechanical design issue
- Reactive control only; no learning or planning

Possible extensions: multiple sensors or a camera for richer detection, wireless notifications to predefined users, more robust mechanical mounting.

## Course context

Autonomous Systems (810031-B-6), BSc Cognitive Science & Artificial Intelligence, Tilburg University.


