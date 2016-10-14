#include <catch.h>

#include <sensor_msgs/JointState.h>
#include <ros_type_introspection/renamer.hpp>

using namespace ros::message_traits;
using namespace RosIntrospection;


TEST_CASE("Deserialize JointState and rename", "Deserialize")
//int func()
{
    std::vector<SubstitutionRule> rules;
    rules.push_back( SubstitutionRule("position.#", "name.#", "@.position") );
    rules.push_back( SubstitutionRule("velocity.#", "name.#", "@.velocity") );
    rules.push_back( SubstitutionRule("effort.#",   "name.#", "@.effort") );

    ROSTypeList type_map = buildROSTypeMapFromDefinition(
                DataType<sensor_msgs::JointState >::value(),
                Definition<sensor_msgs::JointState >::value() );

    sensor_msgs::JointState joint_state;

    joint_state.header.seq = 2016;
    joint_state.header.stamp.sec  = 1234;
    joint_state.header.stamp.nsec = 567*1000*1000;
    joint_state.header.frame_id = "pippo";

    joint_state.name.resize( 3 );
    joint_state.position.resize( 3 );
    joint_state.velocity.resize( 3 );
    joint_state.effort.resize( 3 );

    std::string names[3];
    names[0] = ("hola");
    names[1] = ("ciao");
    names[2] = ("bye");

    for (int i=0; i<3; i++)
    {
        joint_state.name[i] = names[i];
        joint_state.position[i]= 11+i;
        joint_state.velocity[i]= 21+i;
        joint_state.effort[i]= 31+i;
    }

    std::vector<uint8_t> buffer(64*1024);
    ros::serialization::OStream stream(buffer.data(), buffer.size());
    ros::serialization::Serializer<sensor_msgs::JointState>::write(stream, joint_state);

    uint8_t* buffer_ptr = buffer.data();

    ROSType main_type( DataType<sensor_msgs::JointState >::value() );

    ROSTypeFlat flat_container;

    buildRosFlatType(type_map, main_type, "JointState", &buffer_ptr, &flat_container);
    applyNameTransform( rules, &flat_container );

    for(auto&it: flat_container.renamed_value) {
        std::cout << it.first << " >> " << it.second << std::endl;
    }

    int i = 0;

    REQUIRE( flat_container.renamed_value[i].first  == SString("JointState.hola.position"));
    REQUIRE( flat_container.renamed_value[i++].second == 11 );

    REQUIRE( flat_container.renamed_value[i].first  == SString("JointState.ciao.position"));
    REQUIRE( flat_container.renamed_value[i++].second == 12 );

    REQUIRE( flat_container.renamed_value[i].first  == SString("JointState.bye.position"));
    REQUIRE( flat_container.renamed_value[i++].second == 13 );

    REQUIRE( flat_container.renamed_value[i].first  == SString("JointState.hola.velocity"));
    REQUIRE( flat_container.renamed_value[i++].second == 21 );

    REQUIRE( flat_container.renamed_value[i].first  == SString("JointState.ciao.velocity"));
    REQUIRE( flat_container.renamed_value[i++].second == 22 );

    REQUIRE( flat_container.renamed_value[i].first  == SString("JointState.bye.velocity"));
    REQUIRE( flat_container.renamed_value[i++].second == 23 );

    REQUIRE( flat_container.renamed_value[i].first  == SString("JointState.hola.effort"));
    REQUIRE( flat_container.renamed_value[i++].second == 31 );

    REQUIRE( flat_container.renamed_value[i].first  == SString("JointState.ciao.effort"));
    REQUIRE( flat_container.renamed_value[i++].second == 32 );

    REQUIRE( flat_container.renamed_value[i].first  == SString("JointState.bye.effort"));
    REQUIRE( flat_container.renamed_value[i++].second == 33 );

    REQUIRE( flat_container.renamed_value[i].first  == SString("JointState.header.seq"));
    REQUIRE( flat_container.renamed_value[i++].second == 2016 );

    REQUIRE( flat_container.renamed_value[i].first  == SString("JointState.header.stamp"));
    REQUIRE( flat_container.renamed_value[i++].second == 1234.567 );

}
