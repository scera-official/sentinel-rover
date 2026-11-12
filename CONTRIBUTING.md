# Contributing

Thank you for your interest in SentinelRover.

## Before making changes

- Read the README and relevant documents in `docs/`.
- Preserve the distinction between implemented features and proposed future work.
- Do not commit Wi-Fi passwords, API keys, access tokens or other private credentials.
- Keep the transmitter and receiver `SensorPacket` definitions compatible.

## Suggested workflow

1. Create a feature branch.
2. Make a focused change.
3. Test the affected firmware on the intended hardware.
4. Update documentation and pin tables when behaviour or wiring changes.
5. Submit a pull request describing the change and how it was tested.

## Pull request checklist

- [ ] The change has a clear purpose.
- [ ] Firmware compiles for the intended board, where applicable.
- [ ] Transmitter/receiver packet compatibility has been checked.
- [ ] Wiring or configuration changes are documented.
- [ ] No credentials or private data are included.
- [ ] Known limitations are stated honestly.
