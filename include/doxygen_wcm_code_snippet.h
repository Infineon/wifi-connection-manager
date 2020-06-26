/******************************Code Snippet 1**********************************/
/* This code snippet demonstrates how to initialize the Wi-Fi device and network stack,
 * and start scanning of Wi-Fi AP without any filters.
 * The scan_callback receives the scan results and prints them over the serial terminal.
*/

/*! [snippet_wcm_scan_without_filter] */

/* Scan callback function */
void scan_callback(cy_wcm_scan_result_t *result_ptr, void *user_data, cy_wcm_scan_status_t status)
{
    /* Print the scan result only when scan is incomplete because on scan completion
     * the contents of result_ptr are empty.
     */
    if (status == CY_WCM_SCAN_INCOMPLETE)
    {
        printf(" %-32s     %4d     %2d\n",
               result_ptr->SSID, result_ptr->signal_strength, result_ptr->channel);
    }
}

void snippet_wcm_scan_without_filter()
{
    cy_rslt_t result;

    /* Initialize the Wi-Fi device as a STA.*/
    cy_wcm_config_t config = {.interface = CY_WCM_INTERFACE_TYPE_STA};

    /* Initialize the Wi-Fi device, Wi-Fi transport, and lwIP network stack.*/
    result = cy_wcm_init(&config);

    /* Filter is set to NULL.*/
    result = cy_wcm_start_scan(scan_callback, NULL, NULL);
}
/*! [snippet_wcm_scan_without_filter] */

/******************************Code Snippet 2**********************************/
/* This code snippet demonstrates configuring the scan filter to filter by the SSID provided
 * and start a scan. The scan_callback receives the scan results and prints them over the serial terminal.
*/

/*! [snippet_wcm_scan_with_ssid_filter] */

void snippet_wcm_scan_with_ssid_filter()
{
    cy_rslt_t result;
    cy_wcm_scan_filter_t scan_filter;

    /* SSID value which is used to filter the scan results.*/
    static const cy_wcm_ssid_t ssid = "SSID";

    /* Configure the scan filter specified by the variable ssid.*/
    scan_filter.mode = CY_WCM_SCAN_FILTER_TYPE_SSID;
    memcpy(scan_filter.param.SSID, ssid, sizeof(cy_wcm_ssid_t));
    printf("\n---------------------------------------------------------------------------------\n"
           "                  SSID                  RSSI   Channel      \n"
           "---------------------------------------------------------------------------------\n");
    result = cy_wcm_start_scan(scan_callback, NULL, &scan_filter);

}

/*! [snippet_wcm_scan_with_ssid_filter] */


/******************************Code Snippet 3**********************************/
/* This code snippet demonstrates scanning for WIFI_SSID to get the security type of the AP and then
 * connecting to it. After a successful connection, the device registers an event
 * callback through which the middleware notifies the application on
 * disconnection, reconnection, and IP change events. If the connection to the AP
 * fails, it is retried up to MAX_WIFI_RETRY_COUNT times before reporting failure.
 */

/*! [snippet_wcm_connect_ap] */

/* Wi-Fi Credentials: Modify WIFI_SSID and WIFI_PASSWORD to match your Wi-Fi network
 * credentials.
 */
#define WIFI_SSID                         "SSID"
#define WIFI_PASSWORD                     "PASSWORD"

#define MAX_WIFI_RETRY_COUNT                (3u)
#define WIFI_CONN_RETRY_INTERVAL_MSEC       (100u)

/* The size of the cy_wcm_ip_address_t array that is passed to
 * cy_wcm_get_ip_addr API. In the case of stand-alone AP or STA mode, the size of
 * the array is 1. In concurrent AP/STA mode, the size of the array is 2 where
 * the first index stores the IP address of the STA and the second index
 * stores the IP address of the AP.
 */
#define SIZE_OF_IP_ARRAY_STA                (1u)

/* Scan callback function */
void scan_to_get_security(cy_wcm_scan_result_t *result_ptr, void *user_data, cy_wcm_scan_status_t status)
{
    cy_wcm_security_t* security = (cy_wcm_security_t *)user_data;
    /* Scan for SSID and pass the security type through user_data.*/
    if ((strlen((const char *)result_ptr->SSID) != 0) && (status == CY_WCM_SCAN_INCOMPLETE))
    {
        *security = result_ptr->security;
        xTaskNotify(snippet_task_handle, 0, eNoAction);
        cy_wcm_stop_scan();
    }
}

void network_event_callback(cy_wcm_event_t event, cy_wcm_event_data_t *event_data)
{
    if (CY_WCM_EVENT_DISCONNECTED == event)
    {
        printf("Disconnected from Wi-Fi\n");
    }
    else if (CY_WCM_EVENT_RECONNECTED == event)
    {
        printf("Reconnected to Wi-Fi.\n");
    }
    /* This event corresponds to the event when the IP address of the device
     * changes.
     */
    else if (CY_WCM_EVENT_IP_CHANGED == event)
    {
        cy_wcm_ip_address_t ip_addr;
        cy_wcm_get_ip_addr(CY_WCM_INTERFACE_TYPE_STA, &ip_addr, SIZE_OF_IP_ARRAY_STA);

        if (event_data->ip_addr.version == CY_WCM_IP_VER_V4)
        {
            printf("IP Address: %d.%d.%d.%d\n", (uint8_t)event_data->ip_addr.ip.v4,
                   (uint8_t)(event_data->ip_addr.ip.v4 >> 8), (uint8_t)(event_data->ip_addr.ip.v4 >> 16),
                   (uint8_t)(event_data->ip_addr.ip.v4 >> 24));
        }
        else if(event_data->ip_addr.version == CY_WCM_IP_VER_V6)
        {
            printf("IP Address: %lX:%lX:%lX:%lX\n", event_data->ip_addr.ip.v6[0],
                   (event_data->ip_addr.ip.v6[1]), (event_data->ip_addr.ip.v6[2]),
                   (event_data->ip_addr.ip.v6[3]));
        }
    }
}

void snippet_wcm_connect_ap()
{
    cy_rslt_t result;
    cy_wcm_connect_params_t connect_param;
    cy_wcm_ip_address_t ip_address;
    cy_wcm_security_t security;
    cy_wcm_scan_filter_t scan_filter;

    /* Configure the scan filter specified by the macro WIFI_SSID.*/
    scan_filter.mode = CY_WCM_SCAN_FILTER_TYPE_SSID;
    memcpy(scan_filter.param.SSID, WIFI_SSID, sizeof(WIFI_SSID));

    /* Scan with the filter to obtain the security type. The function waits
     * until the security type is obtained in scan_to_get_security.
     */
    cy_wcm_start_scan(scan_to_get_security, (void *)(&security), &scan_filter);
    xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);

    memset(&connect_param, 0, sizeof(cy_wcm_connect_params_t));
    memset(&ip_address, 0, sizeof(cy_wcm_ip_address_t));
    memcpy(connect_param.ap_credentials.SSID, WIFI_SSID, sizeof(WIFI_SSID));
    memcpy(connect_param.ap_credentials.password, WIFI_PASSWORD, sizeof(WIFI_PASSWORD));
    connect_param.ap_credentials.security = security;

    /* Attempt to connect to Wi-Fi until a connection is made or 
     * MAX_WIFI_RETRY_COUNT attempts have been made.
     */
    for (uint32_t conn_retries = 0; conn_retries < MAX_WIFI_RETRY_COUNT; conn_retries++)
    {
        result = cy_wcm_connect_ap(&connect_param, &ip_address);

        if (result == CY_RSLT_SUCCESS)
        {
            printf("Successfully connected to Wi-Fi network '%s'.\n", connect_param.ap_credentials.SSID);

            /* Register event callbacks for changes in the Wi-Fi link status. These
             * events could be related to IP address changes, connection, and
             * disconnection events.
             */
            cy_wcm_register_event_callback(network_event_callback);
            break;
        }

        printf("Connection to Wi-Fi network failed with error code %d."
                "Retrying in %d ms...\n", (int)result, WIFI_CONN_RETRY_INTERVAL_MSEC);

        vTaskDelay(pdMS_TO_TICKS(WIFI_CONN_RETRY_INTERVAL_MSEC));
    }

}
/*! [snippet_wcm_connect_ap] */

/******************************Code Snippet 4**********************************/

/*******************************************************************************
 * Function Name: snippet_wcm_wps_pbc
 *******************************************************************************
 * Summary: Joins a WPS AP and obtains its credentials through WPS using 
 * push button mode. The credentials obtained are printed on the serial
 * terminal.
 *
 ******************************************************************************/
/*! [snippet_wcm_wps_pbc] */

/* The value of this macro specifies the maximum number of Wi-Fi networks the
 * device can join through WPS in one call to cy_wcm_wps_enrollee. This value is
 * 2 for dual-band AP. Note that the device can obtain Wi-Fi credentials of only
 * one network and therefore can receive a maximum of 1 for single-band Wi-Fi AP or
 * 2 for dual-band AP.
 */
#define MAX_WIFI_CREDENTIALS_COUNT          (2)

/* Device's enrollee details. Details of WPS mode, WPS authentication, and
 * encryption methods supported are provided in this structure.
 */
const cy_wcm_wps_device_detail_t enrollee_details =
{
    .device_name               = "PSoC 6",
    .manufacturer              = "Cypress",
    .model_name                = "PSoC 6",
    .model_number              = "1.0",
    .serial_number             = "1234567",
    .device_category           = CY_WCM_WPS_DEVICE_COMPUTER,
    .sub_category              = 7,
    .config_methods            = CY_WCM_WPS_CONFIG_LABEL | CY_WCM_WPS_CONFIG_VIRTUAL_PUSH_BUTTON | CY_WCM_WPS_CONFIG_VIRTUAL_DISPLAY_PIN,
    .authentication_type_flags = CY_WCM_WPS_OPEN_AUTHENTICATION | CY_WCM_WPS_WPA_PSK_AUTHENTICATION | CY_WCM_WPS_WPA2_PSK_AUTHENTICATION | CY_WCM_WPS_WPA2_WPA_PSK_MIXED_AUTHENTICATION,
    .encryption_type_flags     = CY_WCM_WPS_NO_ENCRYPTION | CY_WCM_WPS_AES_ENCRYPTION | CY_WCM_WPS_TKIP_ENCRYPTION
};

void snippet_wcm_wps_pbc()
{
    cy_rslt_t result;

    /* Store the credentials obtained through WPS.*/
    cy_wcm_wps_credential_t credentials[MAX_WIFI_CREDENTIALS_COUNT];
    cy_wcm_wps_config_t wps_config;

    /* Store the PIN used for WPS. It is unused when PBC mode is employed.*/
    char pin_string[CY_WCM_WPS_PIN_LENGTH];
    uint16_t credential_count = MAX_WIFI_CREDENTIALS_COUNT;

    /*Configure the mode of the WPS as CY_WCM_WPS_PBC_MODE.*/
    wps_config.mode = CY_WCM_WPS_PBC_MODE;
    wps_config.password = pin_string;
    memset(credentials, 0, sizeof(credentials));

    printf("Press the push button on your WPS AP.\n");
    result = cy_wcm_wps_enrollee(&wps_config, &enrollee_details, credentials, &credential_count);

    if (CY_RSLT_SUCCESS == result)
    {
        printf("WPS Success.\n");

        /* Print the WPS credentials obtained through WPS.*/
        for (uint32_t loop = 0; loop < credential_count; loop++)
        {
            printf("%ld. SSID = %s, Password = %c%c******.\n", loop + 1, credentials[loop].ssid, credentials[loop].passphrase[0], credentials[loop].passphrase[1]);
        }
    }
}
/*! [snippet_wcm_wps_pbc] */

/******************************Code Snippet 5**********************************/

/*******************************************************************************
 * Function Name: snippet_wcm_wps_pin
 *******************************************************************************
 * Summary: Joins a WPS AP and obtains its credentials through WPS using 
 * PIN mode. The credentials obtained are printed on the serial terminal.
 *
 ******************************************************************************/

/*! [snippet_wcm_wps_pin] */
void snippet_wcm_wps_pin()
{
    cy_rslt_t result;
    cy_wcm_wps_credential_t credentials[MAX_WIFI_CREDENTIALS_COUNT];
    cy_wcm_wps_config_t wps_config;
    char pin_string[CY_WCM_WPS_PIN_LENGTH];
    uint16_t credential_count = MAX_WIFI_CREDENTIALS_COUNT;

    memset(credentials, 0, sizeof(credentials));

    /* Here, the WPS PIN is generated by the device. The user must
     * enter the PIN in the AP to join the network through WPS.
     */
    cy_wcm_wps_generate_pin(pin_string);

    /*Configure the mode of WPS as CY_WCM_WPS_PIN_MODE.*/
    wps_config.mode = CY_WCM_WPS_PIN_MODE;
    wps_config.password = pin_string;
    printf("Enter this PIN: \'%s\' in your AP.\n", pin_string);
    result = cy_wcm_wps_enrollee(&wps_config, &enrollee_details, credentials, &credential_count);

    if (CY_RSLT_SUCCESS == result)
    {
        printf("WPS Success.\n");

        /* Print the WPS credentials obtained through WPS.*/
        for (uint32_t loop = 0; loop < credential_count; loop++)
        {
            printf("%ld. SSID = %s, Password = %c%c******.\n", loop + 1, credentials[loop].ssid, credentials[loop].passphrase[0], credentials[loop].passphrase[1]);
        }
    }
}
/*! [snippet_wcm_wps_pin] */

void snippet_task(void *arg)
{
    snippet_wcm_scan_without_filter();
    vTaskDelay(pdMS_TO_TICKS(SNIPPET_TASK_DELAY_BETWEEN_SNIPPETS_MS));
    snippet_wcm_scan_with_ssid_filter();
    vTaskDelay(pdMS_TO_TICKS(SNIPPET_TASK_DELAY_BETWEEN_SNIPPETS_MS));
    snippet_wcm_connect_ap();
    vTaskDelay(pdMS_TO_TICKS(SNIPPET_TASK_DELAY_BETWEEN_SNIPPETS_MS));
    snippet_wcm_wps_pbc();
    vTaskDelay(pdMS_TO_TICKS(SNIPPET_TASK_DELAY_BETWEEN_SNIPPETS_MS));
    snippet_wcm_wps_pin();
    vTaskDelay(pdMS_TO_TICKS(SNIPPET_TASK_DELAY_BETWEEN_SNIPPETS_MS));
    while(true)
    {
        vTaskDelay(pdMS_TO_TICKS(SNIPPET_TASK_DELAY_BETWEEN_SNIPPETS_MS));
    }
}

int main()
{
    cy_rslt_t result;

    /* This enables RTOS-aware debugging in OpenOCD */
    uxTopUsedPriority = configMAX_PRIORITIES - 1;

    /* Initialize the BSP. */
    result = cybsp_init();
    CY_ASSERT(CY_RSLT_SUCCESS == result);

    /* Initialize retarget-io to use the debug UART port. */
    result = cy_retarget_io_init(CYBSP_DEBUG_UART_TX, CYBSP_DEBUG_UART_RX,
                                 CY_RETARGET_IO_BAUDRATE);

    /* Retarget-io init failed. Stop program execution. */
    CY_ASSERT(CY_RSLT_SUCCESS == result);

    /* Enable global interrupts. */
    __enable_irq();

    /* Create the task. */
    xTaskCreate(snippet_task, "Snippet Task", SNIPPET_TASK_STACK_SIZE, NULL, SNIPPET_TASK_PRIORITY, &snippet_task_handle);

    /* Start the FreeRTOS scheduler. */
    vTaskStartScheduler();

    /* Should never get here. */
    CY_ASSERT(0);
}
