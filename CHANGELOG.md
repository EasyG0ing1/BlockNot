# Changelog
All notable changes to this project will be documented in this file.

The format is based on **Keep a Changelog**, and this project adheres to **Semantic Versioning**.

---

## [Unreleased]


## [2.4.0] – 2025-XX-XX
### Added
- `triggerNext()` method and `TRIGGER_NEXT` macro.
- Ability to define timers in **MINUTES**.
- `getHelp()` method.

### Changed
- Restructured internal code for efficiency.
- Rewrote the ResetAll example for clarity.

---

## [2.3.0] – 2025-XX-XX
### Added
- `speedComp()` feature for preventing rapid unintended triggers on high-speed MCUs (e.g., Raspberry Pi Pico).

---

## [2.2.0] – 2025-XX-XX
### Added
- `setFirstTriggerResponse(bool)` method to control behavior of `firstTrigger()`.

---

## [2.1.5] – 2025-XX-XX
### Added
- Advanced Auto Flashers example.
- Updated README documentation.

---

## [2.1.4] – 2025-XX-XX
### Changed
- Minor internal updates (per PR #19).

---

## [2.1.3] – 2025-XX-XX
### Changed
- `firstTrigger()` no longer performs calculations after triggering, improving efficiency of repeated calls.

---

## [2.1.2] – 2025-XX-XX
### Fixed
- Bug fix.

---

## [2.1.1] – 2025-XX-XX
### Changed
- Renamed public enums to avoid name conflicts with other libraries.

---

## [2.1.0] – 2025-XX-XX
### Added
- Added **Last Trigger Duration** support.

---

## [2.0.7] – 2025-XX-XX
### Changed
- Updated variable declarations to improve thread-safety (see Thread Safety notes).

---

## [2.0.6] – 2025-XX-XX
### Added
- Constructors allowing creation of timers in a **STOPPED** state.
- `setDuration(unsigned long time, Unit inUnits)` to change duration using alternative units without modifying base units.
- Ability to start a timer *while resetting it*, modifying start/stop behavior to act more like a stopwatch.

---

## [2.0.5] – 2025-XX-XX
### Changed
- Adjusted handling of millis offset.

### Added
- `setMicrosOffset(unsigned long)` for testing micros() rollover.

---

## [2.0.3] – 2025-XX-XX
### Added
- Undocumented methods to aid in testing millis() rollover.
- Example sketch demonstrating rollover behavior.

---

## [2.0.0] – 2025-XX-XX
### Added
- Major upgrade enabling timers based on **seconds**, **milliseconds**, and **microseconds**.
- Significant internal redesign to support microsecond precision.

---

## [1.8.5] – 2024-XX-XX
### Added
- `TRIGGERED_ALL` and `TRIGGERED_ON_MARK` macros.
- Millis() Rollover section added to documentation.

### Changed
- Rewrote the triggeredOnDuration documentation and simplified related graphs.

---

## [1.8.4] – 2024-XX-XX
### Changed
- Renamed `STARTED`, `RUNNING`, and `STOPPED` to `ISSTARTED`, `ISRUNNING`, and `ISSTOPPED` to avoid conflicts.

---

## [1.8.3] – 2024-XX-XX
### Fixed
- Minor bug fixes.

---

## [1.8.2] – 2024-XX-XX
### Changed
- Global Reset option is now default again.
- Updated README to reflect internal public API adjustments.

---

## [1.8.1] – 2024-XX-XX
### Added
- `timeTillTrigger()` method and `TIME_TILL_TRIGGER` macro.

### Removed
- Removed unnecessary millis() rollover code.

### Changed
- Cleaned up redundant macros.
- Renamed internal methods for consistency.

---

## [1.8.0] – 2024-XX-XX
### Changed
- Major restructuring into separate header and code files.
- Fixed ODR (One Definition Rule) violation.

---

## [1.7.4] – 2024-XX-XX
### Fixed
- Corrected `triggeredOnDuration` calculations; now accounts for rollover.

---

## [1.7.3] – 2024-XX-XX
### Changed
- Now compatible with millis rollover at ~49 days.

---

## [1.7.2] – 2024-XX-XX
### Added
- `start()` and `stop()` methods.
- `START` and `STOP` macros.

### Changed
- Timers can now be modified while stopped.

---

## [1.7.1] – 2024-XX-XX
### Changed
- Minor efficiency improvements.

---

## [1.7.0] – 2024-XX-XX
### Added
- New `resetAllTimers()` (`RESET_TIMERS`) with bug fixes.
- `triggeredOnDuration()` method and corresponding macro.

### Fixed
- Corrected accumulated drift caused by earlier resetAllTimers behavior.

---

## [1.6.7] – 2024-XX-XX
### Fixed
- Bug preventing compilation when declaring a timer with duration alone.

---

## [1.6.6] – 2024-XX-XX
### Added
- `resetAllTimers()` / `RESET_TIMERS` contributed by @bizprof.

---

## [1.6.5] – 2024-XX-XX
### Added
- Added **SECONDS Mode**.
