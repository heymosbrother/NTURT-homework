#include "ros/ros.h"
#include "std_msgs/Int8.h"
#include "std_msgs/String.h"
#include <vector>
#include <math.h>
#include <string>
using namespace std;

/**
 * @brief This is the ROS subscriber node for decryption
 */

/**
 * A global array to store the received number and a global index for that array
 */
int globalIndex = 0;
std_msgs::Int8 dataArr[10000];
/**
 * This function is used to decrypt and sending out the result
 * @param num - the received ASCII number
 */
void decryptFunc(const std_msgs::Int8::ConstPtr& msg)
{
    std_msgs::Int8 num;
    // num = msg->data;
    dataArr[globalIndex] = msg.data;
    string originalMsg;
    int b = ceil(sqrt(globalIndex));
    int a = floor(sqrt(globalIndex));

    // Create and fill the Matrix with Encrypted message
    char matrix[a][b];

    int index = 0; // Track the unfilled numbers
    for (int j = 0; j < b; j++)
    {
        for (int i = 0; i < a; i++)
        {
            if (index < globalIndex)
            {
                matrix[j][i] = (char)(num - std_msgs::Int8::3);
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

    // Print the decrypted message on ROS terminal
    ROS_INFO("Decrypted Message: [%s]", originalMsg.c_str());
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "decryped_node");
    ros::NodeHandle nh;
    ros::Subscriber sub = nh.subscribe("encrypted_message", 1000, decryptFunc);
    ros::spin();

    return 0;
}
