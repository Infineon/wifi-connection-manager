/*
 * Copyright 2020, Cypress Semiconductor Corporation or a subsidiary of
 * Cypress Semiconductor Corporation. All Rights Reserved.
 *
 * This software, including source code, documentation and related
 * materials ("Software"), is owned by Cypress Semiconductor Corporation
 * or one of its subsidiaries ("Cypress") and is protected by and subject to
 * worldwide patent protection (United States and foreign),
 * United States copyright laws and international treaty provisions.
 * Therefore, you may use this Software only as provided in the license
 * agreement accompanying the software package from which you
 * obtained this Software ("EULA").
 * If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
 * non-transferable license to copy, modify, and compile the Software
 * source code solely for use in connection with Cypress's
 * integrated circuit products. Any reproduction, modification, translation,
 * compilation, or representation of this Software except as specified
 * above is prohibited without the express written permission of Cypress.
 *
 * Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
 * reserves the right to make changes to the Software without notice. Cypress
 * does not assume any liability arising out of the application or use of the
 * Software or any product or circuit described in the Software. Cypress does
 * not authorize its products for use in any products where a malfunction or
 * failure of the Cypress product may reasonably be expected to result in
 * significant property damage, injury or death ("High Risk Product"). By
 * including Cypress's product in a High Risk Product, the manufacturer
 * of such system or application assumes all risk of such use and in doing
 * so agrees to indemnify Cypress against all liability.
*/

/**
* @file cy_wcm.h
* @brief Wi-Fi connection manager (WCM) provides a set of APIs that can be used 
* to establish and monitor Wi-Fi connections on Cypress platforms that support Wi-Fi connectivity.
* WCM library APIs are easy to use compared to WHD APIs; in addition, the library also provides additional features that are not part of WHD.
* See individual API functions for more details.
*/

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "cy_result.h"

#if defined(__ICCARM__)
#define CYPRESS_WEAK            __WEAK
#define CYPRESS_PACKED(struct)  __packed struct
#elif defined(__GNUC__) || defined(__clang__) || defined(__CC_ARM)
#define CYPRESS_WEAK            __attribute__((weak))
#define CYPRESS_PACKED(struct)  struct __attribute__((packed))
#else
#define CYPRESS_WEAK           __attribute__((weak))
#define CYPRESS_PACKED(struct) struct __attribute__((packed))
#endif  /* defined(__ICCARM__) */

/**
*
********************************************************************************
* \mainpage Overview
********************************************************************************
*
* This library provides a set of APIs that can be used to establish and monitor Wi-Fi connections on Cypress platforms that support Wi-Fi connectivity. WCM library APIs are easy to use; in addition, the library provides additional features such Wi-Fi Protected Setup (WPS) and connection monitoring.
*
********************************************************************************
* \section section_features Features and Functionality
********************************************************************************
* Current implementation has the following features and functionality: 
* * Supports only STA mode. AP and Concurrent AP + STA modes will be added in the future.
* * Support for Wi-Fi Protected Setup (WPS) - Enrollee role.
* * Exposes Wi-Fi APIs to scan the Wi-Fi network, and join and leave a Wi-Fi network.
* * Connection monitoring: Monitor active connections and link events. Provides a mechanism to register for event notification. Re-authenticates the connection with the AP when intermittent connection loss occurs.
* * Built on top of FreeRTOS, LwIP, and mbed TLS (that are bundled as part of Wi-Fi Middleware Core Library).

*
********************************************************************************
* \section section_supported_platforms Supported Platforms
********************************************************************************
* This library and its features are supported on the following Cypress platforms:
* * [PSoC 6 Wi-Fi BT Prototyping Kit (CY8CPROTO-062-4343W)](https://www.cypress.com/documentation/development-kitsboards/psoc-6-wi-fi-bt-prototyping-kit-cy8cproto-062-4343w)
* * [PSoC 62S2 Wi-Fi BT Pioneer Kit (CY8CKIT-062S2-43012)](https://www.cypress.com/documentation/development-kitsboards/psoc-62s2-wi-fi-bt-pioneer-kit-cy8ckit-062s2-43012)
*
********************************************************************************
* \section section_dependencies Dependent Libraries
********************************************************************************
* This library includes <a href="https://github.com/cypresssemiconductorco/wifi-mw-core">Wi-Fi middleware core</a> by default. It helps code examples to enable Wi-Fi connectivity components without having to include additional libraries. The Wi-Fi host driver library is part of the Wi-Fi middleware core.
*
********************************************************************************
* \section section_integration Quick Start
********************************************************************************
* * A set of pre-defined configuration files have been bundled with the wifi-mw-core library for FreeRTOS, LwIP, and Mbed TLS. The developer is expected to review the configuration and make adjustments. See the Quick start section in <a href="https://github.com/cypresssemiconductorco/wifi-mw-core/blob/master/README.md">README.md</a> for more details
* A set of COMPONENTS have to be defined in the code example project's Makefile for this library. Refer to the Quick start section in <a href="https://github.com/cypresssemiconductorco/wifi-mw-core/blob/master/README.md">README.md</a> for more details.
* * WCM library enables only error prints by default. For debugging purposes, the application may additionally enable debug and info log messages. To enable these messages, add the WCM_ENABLE_PRINT_INFO, WCM_ENABLE_PRINT_DEBUG, WPS_ENABLE_PRINT_INFO, and WPS_ENABLE_PRINT_DEBUG macros to the DEFINES in the code example's Makefile. The Makefile entry would look like as follows:
*   \code
*    DEFINES+=WCM_ENABLE_PRINT_INFO WCM_ENABLE_PRINT_DEBUG
*    DEFINES+=WPS_ENABLE_PRINT_INFO WPS_ENABLE_PRINT_DEBUG
*   \endcode
*
*********************************************************************************
* \section section_code_snippet Code Snippets
********************************************************************************
********************************************************************************
* \subsection snip1 Snippet 1: Scan for all Wi-Fi APs
* The following snippet demonstrates how to Initialize Wi-Fi device, network stack,
* and starts scanning for AP without any filters.
* The scan_callback receives the scan results and prints them over the serial terminal

* \snippet doxygen_wcm_code_snippet.h snippet_wcm_scan_without_filter
*
* \subsection snip2 Snippet 2: Scan for a Specific Wi-Fi AP
* The following snippet demonstrates configuration of scan filter to filter by the SSID provided
* and starts a scan. The scan_callback receives the scan results and prints over the serial terminal
*
* \snippet doxygen_wcm_code_snippet.h snippet_wcm_scan_with_ssid_filter
*
* \subsection snip3 Snippet 3: Connect to Wi-Fi AP
* This code snippet demonstrates scanning for WIFI_SSID to get the security type of the AP and then
* connects to it. After successful connection, the device registers an event callback through which
* the middleware notifies the application on disconnection, reconnection, and IP change events.
* If the connection to AP fails it is retried up to MAX_WIFI_RETRY_COUNT times before admitting failure
*
* \snippet doxygen_wcm_code_snippet.h snippet_wcm_connect_ap
*
* \subsection snip4 Snippet 4: Connect to Wi-Fi AP using WPS-Push Button
* This code snippet demonstrates joining a WPS AP and obtains its credentials through WPS using the
* push button mode. The credentials obtained are printed on the serial terminal.
*
* \snippet doxygen_wcm_code_snippet.h snippet_wcm_wps_pbc
*
* \subsection snip5 Snippet 5: Connect to Wi-Fi AP using WPS-Pin
* This code snippet demonstrates joining a WPS AP and obtains its credentials through WPS using the
* PIN mode. The credentials obtained are printed on the serial terminal
*
* \snippet doxygen_wcm_code_snippet.h snippet_wcm_wps_pin
*
*/


/**
 * \defgroup group_wcm_mscs Message Sequence Charts
 * \defgroup group_wcm_macros Macros
 * \defgroup group_wcm_enums Enumerated Types
 * \defgroup group_wcm_typedefs Typedefs
 * \defgroup group_wcm_structures Structures
 * \defgroup group_wcm_functions Functions
 */

/**
*
* \addtogroup group_wcm_mscs
* \{
*
********************************************************************************
* \section section_wps_enrollee WPS Enrollee
********************************************************************************
*
* \image html uml_wps_enrollee.png
*
* \}
*
*/

/**
 * \addtogroup group_wcm_macros
 * \{
 */

/******************************************************
 *                    Constants
 ******************************************************/
#define CY_WCM_MAX_SSID_LEN                (32)        /**< Max SSID length.*/
#define CY_WCM_MAX_PASSPHRASE_LEN          (64)        /**< Max passphrase length. */
#define CY_WCM_MAC_ADDR_LEN                (6)         /**< MAC address length. */
#define WEP_ENABLED                        0x0001      /**< Flag to enable WEP security.        */
#define TKIP_ENABLED                       0x0002      /**< Flag to enable TKIP encryption.     */
#define AES_ENABLED                        0x0004      /**< Flag to enable AES encryption.      */
#define SHARED_ENABLED                     0x00008000  /**< Flag to enable shared key security. */
#define WPA_SECURITY                       0x00200000  /**< Flag to enable WPA security.        */
#define WPA2_SECURITY                      0x00400000  /**< Flag to enable WPA2 security.       */
#define WPA3_SECURITY                      0x01000000  /**< Flag to enable WPA3 PSK security.   */
#define ENTERPRISE_ENABLED                 0x02000000  /**< Flag to enable enterprise security. */
#define WPS_ENABLED                        0x10000000  /**< Flag to enable WPS security.        */
#define IBSS_ENABLED                       0x20000000  /**< Flag to enable IBSS mode.           */
#define FBT_ENABLED                        0x40000000  /**< Flag to enable FBT.                 */

/** WPS password length for PIN mode. */
#define CY_WCM_WPS_PIN_LENGTH              (9)

/** Maximum number of callbacks that can be registered with the WCM library. */
#define CY_WCM_MAXIMUM_CALLBACKS_COUNT     (5)

/** \} group_wcm_macros */

/**
 * \addtogroup group_wcm_enums
 * \{
 */

/******************************************************
 *            Enumerations
 ******************************************************/

/**
 * IP Version
 */
typedef enum
{
    CY_WCM_IP_VER_V4 = 4,      /**< Denotes IPv4 version. */
    CY_WCM_IP_VER_V6 = 6       /**< Denotes IPv6 version. */
} cy_wcm_ip_version_t;

/**
 * Enumeration of Wi-Fi Security Modes
 */
typedef enum
{
    CY_WCM_SECURITY_OPEN           = 0,                                                                   /**< Open security.                                         */
    CY_WCM_SECURITY_WEP_PSK        = WEP_ENABLED,                                                         /**< WEP PSK security with open authentication.             */
    CY_WCM_SECURITY_WEP_SHARED     = ( WEP_ENABLED   | SHARED_ENABLED ),                                  /**< WEP PSK security with shared authentication.           */
    CY_WCM_SECURITY_WPA_TKIP_PSK   = ( WPA_SECURITY  | TKIP_ENABLED ),                                    /**< WPA PSK security with TKIP.                            */
    CY_WCM_SECURITY_WPA_AES_PSK    = ( WPA_SECURITY  | AES_ENABLED ),                                     /**< WPA PSK security with AES.                             */
    CY_WCM_SECURITY_WPA_MIXED_PSK  = ( WPA_SECURITY  | AES_ENABLED | TKIP_ENABLED ),                      /**< WPA PSK security with AES & TKIP.                      */
    CY_WCM_SECURITY_WPA2_AES_PSK   = ( WPA2_SECURITY | AES_ENABLED ),                                     /**< WPA2 PSK security with AES.                            */
    CY_WCM_SECURITY_WPA2_TKIP_PSK  = ( WPA2_SECURITY | TKIP_ENABLED ),                                    /**< WPA2 PSK security with TKIP.                           */
    CY_WCM_SECURITY_WPA2_MIXED_PSK = ( WPA2_SECURITY | AES_ENABLED | TKIP_ENABLED ),                      /**< WPA2 PSK security with AES and TKIP.                     */
    CY_WCM_SECURITY_WPA2_FBT_PSK   = ( WPA2_SECURITY | AES_ENABLED | FBT_ENABLED),                        /**< WPA2 FBT PSK security with AES and TKIP.                 */
    CY_WCM_SECURITY_WPA3_SAE       = ( WPA3_SECURITY | AES_ENABLED ),                                     /**< WPA3 security with AES.                                */
    CY_WCM_SECURITY_WPA3_WPA2_PSK  = ( WPA3_SECURITY | WPA2_SECURITY | AES_ENABLED ),                     /**< WPA3 WPA2 PSK security with AES.                       */

    CY_WCM_SECURITY_IBSS_OPEN      = ( IBSS_ENABLED ),                                                    /**< Open security on IBSS ad-hoc network.                  */
    CY_WCM_SECURITY_WPS_SECURE     = ( WPS_ENABLED | AES_ENABLED),                                        /**< WPS with AES security.                                 */

    CY_WCM_SECURITY_UNKNOWN        = -1,                                                                  /**< Returned by \ref cy_wcm_scan_result_callback_t if security is unknown. Do not pass this to the join function! */

    CY_WCM_SECURITY_FORCE_32_BIT   = 0x7fffffff                                                           /**< Exists only to force whd_security_t type to 32 bits. */
} cy_wcm_security_t;

/**
 * Enumeration of 802.11 Radio Bands
 */
typedef enum
{
    CY_WCM_WIFI_BAND_ANY = 0,     /**< The platform will choose an available band.  */
    CY_WCM_WIFI_BAND_5GHZ,        /**< 5-GHz radio band.                         */
    CY_WCM_WIFI_BAND_2_4GHZ,      /**< 2.4-GHz radio band.                       */
} cy_wcm_wifi_band_t;


/**
 * Enumeration of RSSI Range
 */
typedef enum
{
    CY_WCM_SCAN_RSSI_FAIR      = -90,      /**< Wi-Fi RSSI values greater than -90 dbm. */
    CY_WCM_SCAN_RSSI_GOOD      = -60,      /**< Wi-Fi RSSI values greater than -60 dbm. */
    CY_WCM_SCAN_RSSI_EXCELLENT = -40       /**< Wi-Fi RSSI values greater than -40 dbm. */
} cy_wcm_scan_rssi_range_t;


/**
 * Enumeration of WCM Interfaces Types.
 */
typedef enum
{
    CY_WCM_INTERFACE_TYPE_STA = 0,    /**< STA or Client interface.                                                */
    CY_WCM_INTERFACE_TYPE_AP,         /**< SoftAP interface. \note Not supported, will be added in future.         */
    CY_WCM_INTERFACE_TYPE_AP_STA      /**< Concurrent AP + STA mode. \note Not supported, will be added in future. */
} cy_wcm_interface_t;


/**
 * Enumeration of Scan Status
 */
typedef enum
{
    CY_WCM_SCAN_INCOMPLETE,                /**< Scan is in progress and more scan results will be returned. */
    CY_WCM_SCAN_COMPLETE                   /**< Scan is completed. */
} cy_wcm_scan_status_t;

/**
 * Enumeration of WPS Connection Modes
 */
typedef enum
{
    CY_WCM_WPS_PBC_MODE,  /**< Push button mode. */
    CY_WCM_WPS_PIN_MODE   /**< PIN mode.         */
} cy_wcm_wps_mode_t;

/**
 * Enumeration of WPS Configuration Method
 */
typedef enum
{
    CY_WCM_WPS_CONFIG_USBA                  = 0x0001, /**< USB configuration. */
    CY_WCM_WPS_CONFIG_ETHERNET              = 0x0002, /**< Ethernet configuration. */
    CY_WCM_WPS_CONFIG_LABEL                 = 0x0004, /**< Label configuration. */
    CY_WCM_WPS_CONFIG_DISPLAY               = 0x0008, /**< Display configuration. */
    CY_WCM_WPS_CONFIG_EXTERNAL_NFC_TOKEN    = 0x0010, /**< External NFC configuration. */
    CY_WCM_WPS_CONFIG_INTEGRATED_NFC_TOKEN  = 0x0020, /**< Internal NFC configuration. */
    CY_WCM_WPS_CONFIG_NFC_INTERFACE         = 0x0040, /**< NFC Interface. */
    CY_WCM_WPS_CONFIG_PUSH_BUTTON           = 0x0080, /**< Push button configuration. */
    CY_WCM_WPS_CONFIG_KEYPAD                = 0x0100, /**< Keypad configuration. */
    CY_WCM_WPS_CONFIG_VIRTUAL_PUSH_BUTTON   = 0x0280, /**< Virtual push button configuration. */
    CY_WCM_WPS_CONFIG_PHYSICAL_PUSH_BUTTON  = 0x0480, /**< Physical push button configuration. */
    CY_WCM_WPS_CONFIG_VIRTUAL_DISPLAY_PIN   = 0x2008, /**< Virtual display pin configuration. */
    CY_WCM_WPS_CONFIG_PHYSICAL_DISPLAY_PIN  = 0x4008  /**< Physical display pin configuration. */
} cy_wcm_wps_configuration_method_t;


/**
 * Enumeration of WPS Authentication Types
 */
typedef enum
{
    CY_WCM_WPS_OPEN_AUTHENTICATION               = 0x0001, /**< Authentication type OPEN. */
    CY_WCM_WPS_WPA_PSK_AUTHENTICATION            = 0x0002, /**< WPA-PSK authentication type - Deprecated in version 2.0. */
    CY_WCM_WPS_SHARED_AUTHENTICATION             = 0x0004, /**< WPS-SHARED authentication type - Deprecated in version 2.0. */
    CY_WCM_WPS_WPA_ENTERPRISE_AUTHENTICATION     = 0x0008, /**< WPA-ENTERPRISE authentication type - Deprecated in version 2.0. */
    CY_WCM_WPS_WPA2_ENTERPRISE_AUTHENTICATION    = 0x0010, /**< WPA2-ENTERPRISE authentication type. */
    CY_WCM_WPS_WPA2_PSK_AUTHENTICATION           = 0x0020, /**< WPA2-PSK authentication type. */
    CY_WCM_WPS_WPA2_WPA_PSK_MIXED_AUTHENTICATION = 0x0022, /**< WPA2-WPA-PSK authentication type. */
} cy_wcm_wps_authentication_type_t;

/**
 * Enumeration of WPS Encryption Type
 */
typedef enum
{
    CY_WCM_WPS_MIXED_ENCRYPTION = 0x000c, /**< MIXED encryption. */
    CY_WCM_WPS_AES_ENCRYPTION   = 0x0008, /**< AES encryption. */
    CY_WCM_WPS_TKIP_ENCRYPTION  = 0x0004, /**< TKIP encryption - Deprecated in WSC 2.0. */
    CY_WCM_WPS_WEP_ENCRYPTION   = 0x0002, /**< WEP encryption - Deprecated in WSC 2.0. */
    CY_WCM_WPS_NO_ENCRYPTION    = 0x0001, /**< OPEN - No encryption. */
    CY_WCM_WPS_NO_UNDEFINED     = 0x0000, /**< Undefined encryption type. */
} cy_wcm_wps_encryption_type_t;

/**
 * Enumeration of WPS Device Category from the WSC2.0 Spec
 */
typedef enum
{
    CY_WCM_WPS_DEVICE_COMPUTER               = 1,     /**< Computer devices.                */
    CY_WCM_WPS_DEVICE_INPUT                  = 2,     /**< Input devices.                */
    CY_WCM_WPS_DEVICE_PRINT_SCAN_FAX_COPY    = 3,     /**< Devices such as printers, scanners, faxes and copiers.    */
    CY_WCM_WPS_DEVICE_CAMERA                 = 4,     /**< Camera devices.                 */
    CY_WCM_WPS_DEVICE_STORAGE                = 5,     /**< Storage devices.                */
    CY_WCM_WPS_DEVICE_NETWORK_INFRASTRUCTURE = 6,     /**< Network infrastructure devices. */
    CY_WCM_WPS_DEVICE_DISPLAY                = 7,     /**< Display devices.                */
    CY_WCM_WPS_DEVICE_MULTIMEDIA             = 8,     /**< Multimedia devices.             */
    CY_WCM_WPS_DEVICE_GAMING                 = 9,     /**< Gaming devices.                 */
    CY_WCM_WPS_DEVICE_TELEPHONE              = 10,    /**< Telephony devices.             */
    CY_WCM_WPS_DEVICE_AUDIO                  = 11,    /**< Audio devices.                */
    CY_WCM_WPS_DEVICE_DOCK                   = 12,    /**< Docking devices.               */
    CY_WCM_WPS_DEVICE_OTHER                  = 0xFF,  /**< Other devices.                  */
} cy_wcm_wps_device_category_t;

/**
 * Enumeration of WCM Events
 */
typedef enum
{
    CY_WCM_EVENT_RECONNECTED = 0,  /**< STA reconnected to AP. */
    CY_WCM_EVENT_DISCONNECTED,     /**< STA disconnected with AP. */
    CY_WCM_EVENT_IP_CHANGED,       /**< IP address change event. This event is notified after connection, re-connection, and IP address change due to DHCP renewal. */
} cy_wcm_event_t;

/**
 * Enumeration of Scan Filter Types
 */
typedef enum
{
   CY_WCM_SCAN_FILTER_TYPE_SSID = 0,   /**< SSID-based scan filtering. */
   CY_WCM_SCAN_FILTER_TYPE_MAC,        /**< MAC-based scan filtering. */
   CY_WCM_SCAN_FILTER_TYPE_BAND,       /**< BAND-based scan filtering. */
   CY_WCM_SCAN_FILTER_TYPE_RSSI,       /**< RSSI-based scan filtering. */
}cy_wcm_scan_filter_type_t;

/**
 * Enumeration of Network Types
 */
typedef enum
{
    CY_WCM_BSS_TYPE_INFRASTRUCTURE =  0, /**< Infrastructure network.                  */
    CY_WCM_BSS_TYPE_ADHOC          =  1, /**< 802.11 ad-hoc IBSS network.           */
    CY_WCM_BSS_TYPE_ANY            =  2, /**< Either infrastructure or ad-hoc network. */
    CY_WCM_BSS_TYPE_MESH           =  3, /**< 802.11 mesh network.                     */
    CY_WCM_BSS_TYPE_UNKNOWN        = -1  /**< Returned by \ref cy_wcm_scan_result_callback_t if BSS type is unknown. Do not pass this to the Join function. */
} cy_wcm_bss_type_t;

/** \} group_wcm_enums */

/**
 * \addtogroup group_wcm_typedefs
 * \{
 */
/******************************************************
 *                      Typedefs
 ******************************************************/

typedef uint8_t cy_wcm_ssid_t[CY_WCM_MAX_SSID_LEN + 1];              /**< SSID name (AP name in null-terminated string format). */

typedef uint8_t cy_wcm_mac_t[CY_WCM_MAC_ADDR_LEN];                   /**< Unique 6-byte MAC address. */

typedef uint8_t cy_wcm_passphrase_t[CY_WCM_MAX_PASSPHRASE_LEN + 1];  /**< Passphrase in null-terminated string format. */

/** \} group_wcm_typedefs */

/**
 * \addtogroup group_wcm_structures
 * \{
 */

/******************************************************
 *             Structures
 ******************************************************/

/**
 * Structure used to pass WPS configuration parameters to \ref cy_wcm_wps_enrollee.
 * Password is mandatory only for CY_WCM_WPS_PIN mode and not used when mode is CY_WCM_WPS_PBC.
 */
typedef struct
{
	cy_wcm_wps_mode_t mode;        /**< WPS mode. */
	char*             password;    /**< Used only for CY_WCM_WPS_PIN mode. */
} cy_wcm_wps_config_t;

/**
 * Structure used to pass WCM configurations to \ref cy_wcm_init.
 */
typedef struct
{
    cy_wcm_interface_t interface;  /**< Interface type. */
} cy_wcm_config_t;


/**
 * Structure used to receive the IP address information from \ref cy_wcm_connect_ap.
 */
typedef struct
{
    cy_wcm_ip_version_t version;  /**< IP version. */
    union
    {
        uint32_t v4;     /**< IPv4 address bytes. */
        uint32_t v6[4];  /**< IPv6 address bytes. */
    } ip;                /**< IP address bytes. */
} cy_wcm_ip_address_t;

/**
 * Structure used to receive the IP address information through the callback registered using \ref cy_wcm_register_event_callback.
 */
typedef union
{
    cy_wcm_ip_address_t ip_addr;  /**< Contains the IP address for the CY_WCM_EVENT_IP_CHANGED event. */
} cy_wcm_event_data_t;


/**
 * Structure used for providing the Wi-Fi AP credential to connect to a Wi-Fi AP using \ref cy_wcm_connect_ap.
 */
typedef struct
{
    cy_wcm_ssid_t        SSID;                /**< SSID of the Wi-Fi network to join; should be a null-terminated string. */
    cy_wcm_passphrase_t  password;            /**< Password needed to join the AP; should be a null-terminated string. */
    cy_wcm_security_t    security;            /**< Wi-Fi Security. @see cy_wcm_security_t. */
} cy_wcm_ap_credentials_t;

/**
 * Structure used to pass the static IP address information to \ref cy_wcm_connect_ap.
 *
 */
typedef struct
{
    cy_wcm_ip_address_t  ip_address;  /**< IP address.      */
    cy_wcm_ip_address_t  gateway;     /**< Gateway address. */
    cy_wcm_ip_address_t  netmask;     /**< Netmask.         */
} cy_wcm_ip_setting_t;

/**
 * Structure used to pass the Wi-Fi connection parameter information to \ref cy_wcm_connect_ap.
 *
 */
typedef struct
{
    cy_wcm_ap_credentials_t  ap_credentials;       /**< Access point credentials. */
    cy_wcm_mac_t             BSSID;                /**< MAC address of Access Point (optional). */
    cy_wcm_ip_setting_t      *static_ip_settings;  /**< Static IP settings of the device (optional). */
    cy_wcm_wifi_band_t       band;                 /**< Radio band to be connected (optional). */
} cy_wcm_connect_params_t;

/**
 * Structure used to pass scan filters to \ref cy_wcm_start_scan.
 */
typedef struct
{
    cy_wcm_scan_filter_type_t      mode;        /**< Scan filter mode. */
    union
    {
        cy_wcm_ssid_t              SSID;        /**< Service Set Identification. */
        cy_wcm_mac_t               BSSID;       /**< MAC address of AP. */
        cy_wcm_wifi_band_t         band;        /**< Radio band. */
        cy_wcm_scan_rssi_range_t   rssi_range;  /**< RSSI range. */
    } param;                                    /**< Paramter specific to scan filter mode.  */
} cy_wcm_scan_filter_t;


/**
 * Structure used for storing scan results.
 */
typedef struct
{
    cy_wcm_ssid_t                SSID;             /**< SSID (i.e., name of the AP).                                     */
    cy_wcm_mac_t                 BSSID;            /**< Basic Service Set Identification (BSSID), i.e., MAC address of the AP.                        */
    int16_t                      signal_strength;  /**< RSSI in dBm. (<-90=Very poor, >-30=Excellent).                  */
    uint32_t                     max_data_rate;    /**< Maximum data rate in kbps.                                       */
    cy_wcm_bss_type_t            bss_type;         /**< Network type.                                                                               */
    cy_wcm_security_t            security;         /**< Security type.                                                                              */
    uint8_t                      channel;          /**< Radio channel that the AP beacon was received on.                                           */
    cy_wcm_wifi_band_t           band;             /**< Radio band.                                                                                 */
    uint8_t                      ccode[2];         /**< Two-letter ISO country code from AP.                                                        */
    uint8_t                      flags;            /**< Indicates if the scan results are from the same channel if flag is 1; otherwise from the beacon. */
    uint8_t                      *ie_ptr;          /**< Pointer to received Beacon/Probe Response IE (Information Element).          */
    uint32_t                     ie_len;           /**< Length of IE.                                           */
} cy_wcm_scan_result_t;


/**
 * Structure used to pass the device information to \ref cy_wcm_wps_enrollee.
 */
typedef struct
{
    const cy_wcm_wps_device_category_t device_category; /**< Device category.                */
    const uint16_t sub_category;                        /**< Device sub-category.            */
    const char    *device_name;                         /**< Device name.                    */
    const char    *manufacturer;                        /**< Manufacturer details.           */
    const char    *model_name;                          /**< Model name.                     */
    const char    *model_number;                        /**< Model number.                   */
    const char    *serial_number;                       /**< Serial number.                  */
    const uint32_t config_methods;                      /**< Configuration methods.          */
    const uint32_t os_version;                          /**< Operating system version.       */
    const uint16_t authentication_type_flags;           /**< Supported authentication types. */
    const uint16_t encryption_type_flags;               /**< Supported encryption types.     */
    const uint8_t  add_config_methods_to_probe_resp;    /**< Add configuration methods to probe response for Windows enrollees (Not compliant with WPS 2.0). */
} cy_wcm_wps_device_detail_t;

/**
 * Structure used to receive the AP credential after WPS is completed successfully from \ref cy_wcm_wps_enrollee.
 */
typedef struct
{
    cy_wcm_ssid_t         ssid;                                    /**< AP SSID (name) - must be null-terminated */
    cy_wcm_security_t     security;                                /**< AP security type                         */
    cy_wcm_passphrase_t   passphrase;                              /**< AP passphrase - must be null-terminated  */
} cy_wcm_wps_credential_t;

/** \} group_wcm_structures */

/**
 * \addtogroup group_wcm_typedefs
 * \{
 */
/**
 * Wi-Fi Scan result callback function pointer type.
 *
 * @param[in] result_ptr       : A pointer to the scan result; the scan result will be freed once the callback function returns from the application.
 *                               There will not be any scan result when the scan status is CY_WCM_SCAN_COMPLETE.
 * @param[in] user_data        : User-provided data
 * @param[in] status           : Status of the scan process.
 *
 * Note: The callback function will be executed in the context of WCM.
 */
typedef void (*cy_wcm_scan_result_callback_t)( cy_wcm_scan_result_t *result_ptr, void *user_data, cy_wcm_scan_status_t status );


/**
 * WCM event callback function pointer type; events are invoked when WHD posts events to the application.
 * @param[in] event            : WCM events.
 * @param[in] event_data       : A pointer to the event data. The event data will be freed once the callback returns from the application.
 *
 * Note: The callback function will be executed in the context of WCM.
 */
typedef void (*cy_wcm_event_callback_t)(cy_wcm_event_t event, cy_wcm_event_data_t *event_data);

/** \} group_wcm_typedefs */
/**
 * \addtogroup group_wcm_functions
 * \{
 * WCM and WPS internally create a thread each; the created threads are executed with the "CY_RTOS_PRIORITY_ABOVENORMAL" priority.
 * The WCM and WPS API functions are thread-safe.
 * The definition of the CY_RTOS_PRIORITY_ABOVENORMAL macro is located at "libs/abstraction-rtos/include/COMPONENT_FREERTOS/cyabs_rtos_impl.h".
 * All API functions except cy_wcm_scan are blocking.
 * \ref cy_wcm_start_scan is a non-blocking API function; scan results are delivered via \ref cy_wcm_scan_result_callback_t.
 * All application callbacks invoked by WCM will be running in the context of WCM; pointers passed in the callback will be freed once the callback returns.
 */

/**
 * Initializes WCM.
 *
 * This function initializes WCM resources, initializes the WHD, initializes the Wi-Fi transport,
 * turns the Wi-Fi on, and starts up the network stack. This function should be called before calling other WCM API functions.
 * 
 * \note Current implementation supports only STA mode configuration. AP and Concurrent AP + STA mode configurations will be supported in the future.
 *
 * @param[in]  config: The configuration to be initialized.
 *
 * @return CY_RSLT_SUCCESS if WCM initialization was successful; returns [WCM specific error codes](./cy_wcm_error.h) otherwise.
 *
 */
cy_rslt_t cy_wcm_init(cy_wcm_config_t *config);

/**
 * Shuts down WCM.
 *
 * This function cleans up all the resources of WCM and brings down the Wi-Fi driver.
 *
 * \note This API function does not bring down the network stack because the default underlying stack does not have
 *       an implementation for deinit. Therefore, the expectation is that \ref cy_wcm_init and this API should
 *       be invoked only once.
 *
 * @return CY_RSLT_SUCCESS if the Wi-Fi module was successfully turned off; returns [WCM specific error codes](./cy_wcm_error.h) otherwise.
 */
cy_rslt_t cy_wcm_deinit();

/**
 * Performs Wi-Fi network scan.
 * The scan progressively accumulates results over time and may take between 1 and 10 seconds to complete.
 * The results of the scan will be individually provided to the callback function.
 * This API function can be invoked while being connected to an AP.
 *
 *  @param[in]  scan_callback  : Callback function which receives the scan results;
 *                               callback will be executed in the context of WCM.
 *
 *  @param[in]  user_data      : User data to be returned as an argument in the callback function
 *                               when the callback function is invoked.
 *  @param[in]  scan_filter    : Scan filter parameter passed for scanning (optional).
 *
 * @return CY_RSLT_SUCCESS if the Wi-Fi network scan was successful; returns [WCM specific error codes](./cy_wcm_error.h) otherwise.
 * While a scan is in progress, if the user issues another scan, this API returns "CY_RSLT_WCM_SCAN_IN_PROGRESS".
 *
 */
cy_rslt_t cy_wcm_start_scan(cy_wcm_scan_result_callback_t scan_callback, void *user_data, cy_wcm_scan_filter_t *scan_filter);

/**
 * Stops an ongoing Wi-Fi network scan.
 *
 * @return CY_RSLT_SUCCESS if the Wi-Fi network scan was successful; returns [WCM specific error codes](./cy_wcm_error.h) otherwise.
 *
 */
cy_rslt_t cy_wcm_stop_scan();

/**
 * Connects to the Wi-Fi AP using the Wi-Fi credentials and configuration parameters provided,
 * and returns the IP address after successfully joining the Wi-Fi network. This API function is a blocking call.
 * This function additionally performs the following checks:
 * 1. Checks for and ignores duplicate connect requests.
 * 2) Checks the current connection state; if already connected, disconnects from the current
 *    Wi-Fi network and connects to the new Wi-Fi network.
 *
 *
 * @param[in]   connect_params      : Configuration to join the AP.
 * @param[out]  ip_addr             : Pointer to return the IP address (optional).
 *
 * @return CY_RSLT_SUCCESS if connection is successful; returns [WCM specific error codes](./cy_wcm_error.h) otherwise.
 *
 */
cy_rslt_t cy_wcm_connect_ap(const cy_wcm_connect_params_t *connect_params, cy_wcm_ip_address_t *ip_addr);

/**
 * Disconnects from the currently connected Wi-Fi AP.
 *
 * @return CY_RSLT_SUCCESS if disconnection was successful or if the device is already
 * disconnected; returns [WCM specific error codes](./cy_wcm_error.h) otherwise.
 */
cy_rslt_t cy_wcm_disconnect_ap();

/**
 * Retrieves the IP address of the Wi-Fi interface.
 * @param[in]   interface_type  : Type of WCM interface.
 * @param[out]  ip_addr         : Pointer to an IP address structure (or) an IP address structure array. Currently, upon return, index-0 stores the IPv4 address of CY_WCM_INTERFACE_TYPE_STA.\n
 *                                In future, IPv6 addresses and other \ref cy_wcm_interface_t interfaces will be supported.
 *
 * @param[in]   addr_count      : Length of the array passed in ip_addr.
 *
 * @return CY_RSLT_SUCCESS if IP-address get is successful; returns [WCM specific error codes](./cy_wcm_error.h) otherwise.
 */
cy_rslt_t cy_wcm_get_ip_addr(cy_wcm_interface_t interface_type, cy_wcm_ip_address_t *ip_addr, uint8_t addr_count);

/**
 * Retrieves the MAC address of the interface.
 *
 * @param[in]   interface_type  : Type of WCM interface.
 * @param[out]  mac_addr        : Pointer to a MAC address structure (or) a MAC address structure array. Currently, upon return, index-0 stores the MAC address of CY_WCM_INTERFACE_TYPE_STA.\n
 *                                In future, other \ref cy_wcm_interface_t interfaces will be supported.
 *
 * @param[in]   addr_count      : Length of the array passed in mac_addr.
 * 
 * @return CY_RSLT_SUCCESS if MAC address get is successful; returns [WCM specific error codes](./cy_wcm_error.h) otherwise.
 */
cy_rslt_t cy_wcm_get_mac_addr(cy_wcm_interface_t interface_type, cy_wcm_mac_t *mac_addr, uint8_t addr_count);

/**
 * Negotiates securely with a Wi-Fi Protected Setup (WPS) registrar (usually an
 *  AP) and obtains Wi-Fi network credentials.
 *
 * @param[in]  config               : Pointer to WPS configuration information.
 * @param[in]  details              : Pointer to a structure containing manufacturing details
 *                                    of this device.
 * @param[out] credentials          : Pointer to an array of credentials structure \ref cy_wcm_wps_credential_t to receive the AP credentials.
 * @param[in, out] credential_count : Upon invocation, this parameter stores the size of the credentials parameter. Upon return, denotes the actual
 *                                    number of credentials returned.
 *
 * @return CY_RSLT_SUCCESS if credentials are retrieved successfully; returns [WCM specific error codes](./cy_wcm_error.h) otherwise.
 */
cy_rslt_t cy_wcm_wps_enrollee(cy_wcm_wps_config_t* config, const cy_wcm_wps_device_detail_t *details, cy_wcm_wps_credential_t *credentials, uint16_t *credential_count);

/**
 * Generates random WPS PIN for PIN mode connection.
 *
 * @param[out]  wps_pin_string  : Pointer to store the WPS PIN as a null-terminated string.
 *
 * @return CY_RSLT_SUCCESS if WPS PIN generated; returns WCM specific error codes otherwise.
 */
cy_rslt_t cy_wcm_wps_generate_pin(char wps_pin_string[CY_WCM_WPS_PIN_LENGTH]);

/**
 * Registers an event callback to monitor the connection and IP address change events.
 * This is an optional registration; used if the application needs to monitor events across disconnection and reconnection.
 *  *
 * Note: This API is expected to be called typically while being connected to an AP.
 *
 * @param[in]  event_callback  : Callback function to be invoked for event notification.
 *                               The callback will be executed in the context of WCM.
 *
 * @return CY_RSLT_SUCCESS if application callback registration was successful; returns [WCM specific error codes](./cy_wcm_error.h) otherwise.
*/
cy_rslt_t cy_wcm_register_event_callback(cy_wcm_event_callback_t event_callback);

/**
 * De-registers an event callback.
 *
 * @param[in]  event_callback  : Callback function to de-register from getting notifications.
 *
 * @return CY_RSLT_SUCCESS if application callback de-registration was successful; returns [WCM specific error codes](./cy_wcm_error.h) otherwise.
*/
cy_rslt_t cy_wcm_deregister_event_callback(cy_wcm_event_callback_t event_callback);

/** \} group_wcm_functions */

#ifdef __cplusplus
} /* extern C */
#endif
