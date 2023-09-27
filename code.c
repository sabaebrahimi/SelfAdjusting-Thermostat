// Include the necessary libraries
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Define the constants and parameters
#define NUM_STATES 120    // The number of possible states (5*4*3*2*2)
#define NUM_ACTIONS 3     // The number of possible actions (increase, decrease, or maintain the temperature)
#define ALPHA 0.1         // The learning rate
#define GAMMA 0.9         // The discount factor
#define EPSILON 0.1       // The exploration rate
#define MAX_EPISODES 1000 // The maximum number of episodes to run
#define MAX_STEPS 24      // The maximum number of steps per episode

// Define the state variables and their ranges
int time_i;  // The current time_i (0-4)
int day;   // The current day (0-3)
int temp;  // The current temperature (0-2)
int humid; // The current humidity (0-1)
int user;  // The user presence (0-1)

// Define the action values and their meanings
int action;                                                 // The current action
double action_value[NUM_STATES][NUM_ACTIONS];               // The Q-table that stores the action values for each state and action

char *action_name[] = {"Increase", "Decrease", "Maintain"}; // The names of the actions

// Define the reward function and its components
double reward;    // The current reward
double temp_diff; // The difference between the current temperature and the user's preferred temperature
double energy;    // The amount of energy consumed by the thermostat
double override;  // The frequency of user overrides

// Define the sensors and actuators for the thermostat
int sensor_time;   // The sensor that reads the current time_i
int sensor_day;    // The sensor that reads the current day
int sensor_temp;   // The sensor that reads the current temperature
int sensor_humid;  // The sensor that reads the current humidity
int sensor_user;   // The sensor that reads the user presence
int actuator_temp; // The actuator that controls the temperature

// Define some helper functions

// A function that returns a random number between 0 and 1
double rand_double()
{
    return (double)rand() / (double)RAND_MAX;
}

// A function that returns a random integer between min and max (inclusive)
int rand_int(int min, int max)
{
    return min + rand() % (max - min + 1);
}

// A function that returns the index of the state based on the state variables
int get_state_index()
{
    return time_i * 24 + day * 6 + temp * 2 + humid * 1 + user * 0;
}

// A function that returns the index of the best action for a given state based on the Q-table
int get_best_action(int state)
{
    int best_action = 0;
    double best_value = -INFINITY;
    for (int i = 0; i < NUM_ACTIONS; i++)
    {
        if (action_value[state][i] > best_value)
        {
            best_action = i;
            best_value = action_value[state][i];
        }
    }
    return best_action;
}

// A function that returns an action based on an epsilon-greedy policy
int get_action(int state)
{
    if (rand_double() < EPSILON)
    {
        return rand_int(0, NUM_ACTIONS - 1); // Explore a random action
    }
    else
    {
        return get_best_action(state); // Exploit the best action
    }
}

// A function that updates the state variables based on the sensor readings
void update_state()
{
    time_i = sensor_time / 5;    // Divide by 5 to get a range of 0-4
    day = sensor_day % 4;      // Modulo by 4 to get a range of 0-3
    temp = sensor_temp / 10;   // Divide by 10 to get a range of 0-2
    humid = sensor_humid / 50; // Divide by 50 to get a range of 0-1
    user = sensor_user;        // Use directly as a range of 0-1
}

// A function that updates the reward based on the state variables and the action
void update_reward()
{
    // Calculate the difference between the current temperature and the user's preferred temperature
    // Assume the user's preferred temperature is 20 degrees Celsius
    temp_diff = abs(sensor_temp - 20);

    // Calculate the amount of energy consumed by the thermostat
    // Assume the energy consumption is proportional to the action value
    // Increase = 2, Decrease = 1, Maintain = 0
    energy = action;

    // Calculate the frequency of user overrides
    // Assume the user overrides the thermostat if the temperature difference is greater than 5 degrees Celsius
    override = (temp_diff > 5) ? 1 : 0;

    // Define the reward function as a weighted sum of the factors
    // Use negative weights to penalize high values of the factors
    reward = -0.5 * temp_diff - 0.2 * energy - 0.3 * override;
}

// A function that updates the Q-table based on the state, action, reward, and next state
void update_q_table(int state, int action, double reward, int next_state)
{
    // Apply the Q-learning update rule
    action_value[state][action] = action_value[state][action] + ALPHA * (reward + GAMMA * action_value[next_state][get_best_action(next_state)] - action_value[state][action]);
}

// A function that updates the actuator based on the action
void update_actuator()
{
    // Set the actuator to increase, decrease, or maintain the temperature by a certain amount
    // Assume the amount is 1 degree Celsius for each action
    actuator_temp = sensor_temp + (action - 1);
}

// A function that prints the state, action, reward, and Q-table for debugging purposes
void print_debug_info()
{
    printf("State: time_i=%d, day=%d, temp=%d, humid=%d, user=%d\n", time_i, day, temp, humid, user);
    printf("Action: %s\n", action_name[action]);
    printf("Reward: %f\n", reward);
    printf("Q-table:\n");
    for (int i = 0; i < NUM_STATES; i++)
    {
        for (int j = 0; j < NUM_ACTIONS; j++)
        {
            printf("%f ", action_value[i][j]);
        }
        printf("\n");
    }
}

// The main function that runs the Q-learning algorithm for the smart thermostat problem
int main()
{

    // Initialize the random seed
    srand(time(0));

    // Initialize the Q-table to zero
    for (int i = 0; i < NUM_STATES; i++)
    {
        for (int j = 0; j < NUM_ACTIONS; j++)
        {
            action_value[i][j] = 0.0;
        }
    }

    // Loop over the episodes
    for (int e = 0; e < MAX_EPISODES; e++)
    {
        printf("Enter values\n");
        scanf("%d %d %d %d %d", &sensor_time, &sensor_day, &sensor_temp, &sensor_user, &sensor_humid);

        // Reset the state variables based on the sensor readings
        update_state();

        // Loop over the steps
        for (int s = 0; s < MAX_STEPS; s++)
        {
            // Get the index of the current state
            int state = get_state_index();

            // Get an action based on the epsilon-greedy policy
            action = get_action(state);

            // Update the reward based on the state variables and the action
            update_reward();

            // Update the state variables based on the sensor readings
            update_state();

            // Get the index of the next state
            int next_state = get_state_index();

            // Update the Q-table based on the state, action, reward, and next state
            update_q_table(state, action, reward, next_state);

            // Update the actuator based on the action
            update_actuator();

            // Print some debug information
            print_debug_info();
        }
    }

    // Return 0 to indicate success
    return 0;
}
