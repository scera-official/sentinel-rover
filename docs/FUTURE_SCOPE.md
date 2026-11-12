# Future Scope

The following items are ideas for future development. They are not represented as completed features in the current firmware.

## Possible enhancements

1. **Camera integration** — add a camera for visual observation.
2. **Map interface** — plot valid GPS coordinates on a digital map.
3. **Operator dashboard** — provide a web or mobile interface for sensor and GPS information.
4. **Additional environmental sensors** — explore temperature, humidity and gas sensing.
5. **Autonomous navigation** — investigate safe route planning and obstacle avoidance in controlled test environments.
6. **Data logging** — store timestamped sensor readings and monitoring events.
7. **Communication diagnostics** — add packet counters, sequence numbers and clearer link-status reporting.
8. **Power management** — improve supply regulation, battery monitoring and runtime.
9. **PIR event handling** — add an explicit receiver-side indication for motion events if required.
10. **Firmware modularisation** — separate radio packet definitions and shared configuration into reusable headers.

## Development priorities

A practical next sequence would be:

- Validate stable power and radio communication.
- Add clear diagnostics for GPS validity and packet reception.
- Improve alert presentation so warnings do not obscure important LCD information.
- Add a test checklist and record measured performance.
- Consider additional features only after the current system is repeatably reliable.
