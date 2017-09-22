#ifndef ROS_INTROSPECTION_HPP
#define ROS_INTROSPECTION_HPP

//#include <ros_type_introspection/renamer.hpp>
#include <ros_type_introspection/deserializer.hpp>
#include <ros_type_introspection/parser.hpp>
#include <set>

namespace RosIntrospection{

class Parser{

public:
  Parser() {}

  /**
   * @brief A single message definition will (most probably) generate myltiple ROSMessage(s).
   * In fact the "child" ROSTypes are parsed as well in a recursive and hierarchical way.
   * To make an example, given as input the [geometry_msgs/Pose](http://docs.ros.org/kinetic/api/geometry_msgs/html/msg/Pose.html)
   * the result will be a ROSTypeList containing Pose, Point and Quaternion.
   *
   * @param msg_identifier name to give to the main type to be extracted.
   *
   * @param msg_definition text obtained by either:
   *                       - topic_tools::ShapeShifter::getMessageDefinition()
   *                       - rosbag::MessageInstance::getMessageDefinition()
   *                       - ros::message_traits::Definition< __your_type__ >::value()
   */
  void registerMessageDefinition(const std::string& message_identifier,
                                 const ROSType &main_type,
                                 const std::string& definition);

  const ROSMessageInfo* getMessageInfo(const std::string& msg_identifier);

  void deserializeIntoFlatContainer(const std::string& msg_identifier,
                                    const nonstd::VectorView<uint8_t>& buffer,
                                    ROSTypeFlat* flat_container_output,
                                    const uint32_t max_array_size );


  private:

    std::map<std::string,ROSMessageInfo> _registred_messages;

    void createStringTree(ROSMessageInfo &info, const std::string &type_name);
};

}

#endif // ROS_INTROSPECTION_HPP
