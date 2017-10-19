#include <ndn-cxx/face.hpp>
#include <ndn-cxx/security/key-chain.hpp>
#include <ndn-cxx/interest.hpp>
#include <iostream>


double temp1;
double temp2;
int  flag = 0;
double avg;

class Fog
{
public:
    Fog(ndn::Face& face)
            : m_face(face)
    {
        m_face.setInterestFilter(ndn::InterestFilter(ndn::Name("/Senario/Weather/Sample/MyArea/Sensors")),
                                 std::bind(&Fog::onInterestSensor, this, _2),
                                 std::bind(&Fog::onSuccessSensor, this, _1),
                                 std::bind(&Fog::onFailureSensor, this, _1, _2)
                                 );

        m_face.setInterestFilter(ndn::InterestFilter(ndn::Name("/Senario/Weather/Sample/MyArea/Average")),
                                 std::bind(&Fog::onInterestServer, this, _2),
                                 std::bind(&Fog::onSuccessServer, this, _1),
                                 std::bind(&Fog::onFailureServer, this, _1, _2)
        );
    }

private:

    void onInterestSensor(const ndn::Interest& interest)
    {
        flag++;
        std::cout << "Interest for " << interest.getName() << std::endl;

        if (interest.getName().getSubName(5, 1).toUri().substr(1) == "1") {
            temp1 = atof(interest.getName().getSubName(6).toUri().substr(1).c_str());
            std::cout << ">> Sensor" << interest.getName().getSubName(5, 1).toUri().substr(1) +
                        " temperature is " << temp1 << std::endl;
        }

        else if (interest.getName().getSubName(5, 1).toUri().substr(1) == "2") {
            temp2 = atof(interest.getName().getSubName(6).toUri().substr(1).c_str());
            std::cout <<">> Sensor" << interest.getName().getSubName(5, 1).toUri().substr(1) +
                        " temperature is " << temp2 << std::endl;
        }

        if (flag == 2){
            flag = 0;
            avg = (temp1 + temp2) / 2;
            std::cout << "\n>> Average is equal to " << std::to_string(avg) << "\n" << std::endl;
        }

        auto data = std::make_shared<ndn::Data>(interest.getName());

        std::string ack = "ok\n";

        data->setContent(reinterpret_cast<const uint8_t*>(ack.c_str()), ack.size());

        // set metainfo parameters
        data->setFreshnessPeriod(ndn::time::seconds(5));


        // sign data packet
        m_keyChain.sign(*data);

        // make data packet available for fetching
        m_face.put(*data);
    }

    void onSuccessSensor(const ndn::Name& prefix){
        std::cout << "Prefix registered for " << prefix.toUri() << std::endl;
    }

    void onFailureSensor(const ndn::Name& prefix, const std::string& reason){
        std::cout << "Failed to register prefix " << prefix.toUri() << std::endl;
        std::cout << "The reason: " << reason << std::endl;
    }

/////////////////////////////////////////////////////////////////////////////////////////////////////

    void onInterestServer(const ndn::Interest& interest){
        std::cout << "Interest for " << interest.getName() << std::endl;

        ndn::Name name = interest.getName();
        name.append(ndn::time::toIsoString(ndn::time::system_clock::now()));

        auto data = std::make_shared<ndn::Data>(name);

        std::string temp = std::to_string(avg);

        data->setContent(reinterpret_cast<const uint8_t*>(temp.c_str()), temp.size());

        // set metainfo parameters
        data->setFreshnessPeriod(ndn::time::seconds(5));


        // sign data packet
        m_keyChain.sign(*data);

        // make data packet available for fetching
        m_face.put(*data);
    }

    void onSuccessServer(const ndn::Name& prefix){
        std::cout << "Prefix registered for " << prefix.toUri() << std::endl;
    }

    void onFailureServer(const ndn::Name& prefix, const std::string& reason){
        std::cout << "Failed to register prefix " << prefix.toUri() << std::endl;
        std::cout << "The reason: " << reason << std::endl;
    }


private:
    ndn::Face& m_face;
    ndn::KeyChain m_keyChain;
};


int main(int argc, char** argv)
{
    std::string command = "nlsrc advertise /Senario/Weather/Sample/MyArea/Average";
    system(command.c_str());
    command = "nlsrc advertise /Senario/Weather/Sample/MyArea/Sensors";
    system(command.c_str());


    try {
        // create Face instance
        ndn::Face face;

        // create server instance
        Fog fog(face);

        // start processing loop (it will block forever)
        face.processEvents();
    }
    catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
    }
    return 0;
}
