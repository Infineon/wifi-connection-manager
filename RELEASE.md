# Cypress Wi-Fi Connection Manager (WCM)

## What's Included?
See the [README.md](./README.md) for a complete description of the Wi-Fi Connection Manager.

## Known Issues
| Problem | Workaround |
| ------- | ---------- |
| The `cy_wcm_deinit` API does not bring down the network stack as the default underlying lwIP stack does not have an implementation for deinit. Therefore, the expectation is that `cy_wcm_init` and `cy_wcm_deinit` APIs should be invoked only once. | No workaround. Support will be added in a future release. |
| This release does not support AP and Concurrent AP + STA mode. | No workaround. Support will be added in a future release. |

## Changelog
### v1.1.0
* Introduced APIs to get the following:
  - Connected AP information
  - WLAN interface statistics
  - IPv6 link local address
  - Gateway IP address
  - Netmask IP address
  - Gateway MAC address
* Introduced API to ping IPv4 address.
* Minor documentation updates.

### v1.0.1
* Code snippets added to the documentation.

### v1.0.0
* Initial release for Wi-Fi Connection Manager.

* Includes support for Wi-Fi Protected Setup (WPS) - Enrollee role.

* Exposes Wi-Fi APIs to scan, join, and leave a Wi-Fi network.

* Connection monitoring: Monitor active connections and link events. Provides a mechanism to register for event notification. Re-authenticates the connection with AP in case of intermittent connection loss.

### Supported Software and Tools
This version of the library was validated for compatibility with the following Software and Tools:

| Software and Tools                                      | Version |
| :---                                                    | :----:  |
| ModusToolbox Software Environment                       | 2.1     |
| - ModusToolbox Device Configurator                      | 2.1     |
| - ModusToolbox CSD Personality in Device Configurator   | 2.0     |
| - ModusToolbox CapSense Configurator / Tuner tools      | 3.0     |
| PSoC 6 Peripheral Driver Library (PDL)                   | 1.5.0   |
| GCC Compiler                                            | 7.2.1   |
| IAR Compiler                                            | 8.32    |
