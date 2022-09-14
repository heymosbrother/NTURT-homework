#include "ros/ros.h"
#include "std_msgs/Int8.h"
#include "std_msgs/String.h"

#include <math.h>
#include <string>
#include <sstream>

using namespace std;

/**
 * @brief Class used for handling all publish and subscribe issues, also stores the received data.
 * 
 * Place the commonly used ros codes in the class scope to make publish and subscribe 
 * at the same node easier, further more, make the main function clean and neat.
 * @author heymosbrother
 * @date July 2022
 */
class SubscribeAndPublish
{
private:
    ros::NodeHandle nh;
    ros::Publisher pub;
    ros::Subscriber sub;
    int dataArr[441]; /**<An array to store the received data*/
    int counter = 0;/**<An index to point out where to store the data*/
    string originalMsg = "";

public:

    /**
     * @brief Construct a new Subscribe And Publish object
     * 
     * Initializes and declares publisher and subscriber ROS objects.
     * clean the array where the received data will be put
     */
    SubscribeAndPublish()
    {
        pub = nh.advertise<std_msgs::String>("decrypted_message", 1);
        sub = nh.subscribe("encrypted_message", 100, &SubscribeAndPublish::receiveMsg, this);
        for (int i = 0; i < 440; i++)
            dataArr[i] = 0;
    }

    /**
     * Get the message from bag file and store them into the array member
     * @param msg - the received ASCII number in ROS msg formation
     */
    void receiveMsg(const std_msgs::Int8::ConstPtr &msg)
    {
        dataArr[counter] = (int)(msg->data);
        // Print the received message on terminal
        // ROS_INFO("[%d]\tReceived, #%d,\ttransform to [%c]", msg->data, counter + 1, dataArr[counter] - 3);
        counter++;
        if (counter == 440)
            decryptMsg();
    }

    /**
     * Decrypt the message stored in the global array and then print on the terminal
     */
    void decryptMsg()
    {

        int b = ceil(sqrt(440));
        int a = floor(sqrt(440));

        if (a * b < 440)
            a++;

        // Create and fill the Matrix with Encrypted message
        char matrix[a][b];

        int index = 0;
        for (int j = 0; j < b; j++)
        {
            for (int i = 0; i < a; i++)
            {
                if (index < 440)
                {
                    matrix[j][i] = dataArr[index] - 3;
                }
                index++;
            }
        }

        // Read and store the decrypted message
        int makeStrIndex = 0;
        for (int j = 0; j < a; j++)
        {
            for (int i = 0; i < b; i++)
            {
                if(makeStrIndex < 440)
                {
                    originalMsg = originalMsg + matrix[i][j];
                    makeStrIndex++;
                } 
            }
        }

        // Print out the decrypted message on the terminal
        ROS_INFO("a = %d, b = %d, Decrypted Message: \n%s", a, b, originalMsg.c_str());

        std_msgs::String decryptedResult;
        stringstream ss;
        ss << originalMsg;
        decryptedResult.data = ss.str();

        pub.publish(decryptedResult);
    }
};

int main(int argc, char **argv)
{
    // Initialize the current node
    ros::init(argc, argv, "decryped_node");

    SubscribeAndPublish SubPubObject;

    ros::spin();

    return 0;
}