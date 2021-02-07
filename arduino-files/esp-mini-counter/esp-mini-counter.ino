#define _BAWSL

void setup()
{
    // Setup Environment
    controller_setup();

    // Initialize Output for User interaction
    output_init();

    // Initialize Sensor for reading input
    sensor_init();

    // Set Status to INIT
    status_init();

    // Connect to Network if required
    network_connect();

    // Set Status to CONNECTED
    status_connected();

    // Initialize Storage for data from sensor
    storage_init();

    // Set Status to READY
    status_ready();
}

void loop()
{
    // run init for loop
    loop_action_init();

    // Check if 
    int is_active = action_is_active();

    if (is_active == 1)
    {
        // Run commands if action is active
        action_on_active();
    }
    else
    {
        // Run commands if action is inactive
        action_on_inactive();
    }
}
