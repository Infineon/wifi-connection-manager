# Cypress Wi-Fi Connection Manager (WCM)
This library provides a set of APIs that are useful to establish and monitor Wi-Fi connections on Cypress platforms that support Wi-Fi connectivity. WCM library APIs are easy to use; the library also provides additional features such as Wi-Fi Protected Setup (WPS) and connection monitoring.

## Features and Functionality
The current implementation has the following features and functionality:
* Supports only STA mode. AP and Concurrent AP + STA modes will be added in the future.
* Support for Wi-Fi Protected Setup (WPS) - Enrollee role.
* Exposes Wi-Fi APIs to scan the Wi-Fi network, and join and leave a Wi-Fi network.
* Connection monitoring: Monitor active connections and link events. Provides a mechanism to register for event notification. Re-authenticates the connection with the AP when intermittent connection loss occurs.
* Built on top of FreeRTOS, LwIP, and Mbed TLS (that are bundled as part of Wi-Fi Middleware Core Library).


## Supported Platforms
This library and its features are supported on the following Cypress platforms:
* [PSoC 6 Wi-Fi BT Prototyping Kit (CY8CPROTO-062-4343W)](https://www.cypress.com/documentation/development-kitsboards/psoc-6-wi-fi-bt-prototyping-kit-cy8cproto-062-4343w)
* [PSoC 62S2 Wi-Fi BT Pioneer Kit (CY8CKIT-062S2-43012)](https://www.cypress.com/documentation/development-kitsboards/psoc-62s2-wi-fi-bt-pioneer-kit-cy8ckit-062s2-43012)

## Dependent Libraries
This library depends on the following:
* [Wi-Fi Middleware Core](https://github.com/cypresssemiconductorco/wifi-mw-core)
* [Wi-Fi Host Driver](https://github.com/cypresssemiconductorco/wifi-host-driver)

## Quick Start
* A set of pre-defined configuration files have been bundled with wifi-mw-core library for FreeRTOS, LwIP, and Mbed TLS. The developer is expected to review the configuration and make adjustments. See the "Quick Start" section in [README.md](https://github.com/cypresssemiconductorco/wifi-mw-core/blob/master/README.md)
for more details.
* A set of COMPONENTS have to be defined in the code example project's Makefile for this library. Refer to the Quick start section in [README.md](https://github.com/cypresssemiconductorco/wifi-mw-core/blob/master/README.md) for more details.
* The WCM library enables only error prints by default. For debugging purposes, the application may additionally enable debug and info log messages. To enable these messages, add `WCM_ENABLE_PRINT_INFO`, `WCM_ENABLE_PRINT_DEBUG`, `WPS_ENABLE_PRINT_INFO`, and `WPS_ENABLE_PRINT_DEBUG` macros to the *DEFINES* in the code example's Makefile. The Makefile entry would look as follows:
  ```
  DEFINES+=WCM_ENABLE_PRINT_INFO WCM_ENABLE_PRINT_DEBUG
  DEFINES+=WPS_ENABLE_PRINT_INFO WPS_ENABLE_PRINT_DEBUG
  ```

## Additional Information
* [Wi-Fi Connection Manager RELEASE.md](./RELEASE.md)
* [Wi-Fi Connection Manager API Documentation](https://cypresssemiconductorco.github.io/wifi-connection-manager/api_reference_manual/html/index.html)
* [ModusToolbox Software Environment, Quick Start Guide, Documentation, and Videos](https://www.cypress.com/products/modustoolbox-software-environment)
* [Wi-Fi Connection Manager version](./version.txt)
* [ModusToolbox AnyCloud code examples](https://github.com/cypresssemiconductorco?q=mtb-example-anycloud%20NOT%20Deprecated)
