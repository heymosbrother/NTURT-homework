#include "ros/ros.h"
#include "std_msgs/Int8.h"

#include <math.h>
#include <string>

using namespace std;

/**
 * A global array to store the received number and a global index for that array
 */
int counter = 0;
int dataArr[440];
/**
 * @brief Decrypt and sending out the result
 * @param msg - the received ASCII number plus 3
 */
void decryptFunc(const std_msgs::Int8::ConstPtr &msg)
{
    dataArr[counter] = (int)(msg->data);
    // Print the received message on terminal
    ROS_INFO("[%d]\tReceived, #%d,\ttransform to [%c]", msg->data, counter + 1, dataArr[counter] - 3);
    counter++;

    if (counter >= 440)
    {
        string originalMsg = "";

        int b = ceil(sqrt(440));
        int a = floor(sqrt(440));

        if(a*b<440) a++;

        // Create and fill the Matrix with Encrypted message
        char matrix[a][b];

        int index = 0; // Track the unfilled numbers
        for (int j = 0; j < b; j++)
        {
            for (int i = 0; i < a; i++)
            {
                if (index < counter)
                {
                    matrix[j][i] = dataArr[index] - 3;
                }
                index++;
            }
        }

        // Read and store the decrypted message
        for (int j = 0; j < a; j++)
        {
            for (int i = 0; i < b; i++)
            {
                originalMsg = originalMsg + matrix[i][j];
            }
        }

        // Print out the decrypted message on the terminal
        ROS_INFO("a = %d, b = %d, Decrypted Message: \n%s", a, b, originalMsg.c_str());
    }
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "decryped_node");
    ros::NodeHandle nh;
    ros::Subscriber sub = nh.subscribe("encrypted_message", 1000, decryptFunc);
    ros::spin();

    return 0;
}