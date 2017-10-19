#include <ndn-cxx/face.hpp>
#include <ndn-cxx/interest.hpp>
#include <ndn-cxx/data.hpp>

#include <iostream>
#include <string>

class Server
{
public:
    explicit Server(ndn::Face& face, const std::string& uri)
            : m_face(face)
            , m_basename(uri)
            , m_currentSeqNo(0)
    {
        m_face.setInterestFilter(ndn::InterestFilter(ndn::Name("/Senario/Meteorology/Temperature/MyArea")),
                                 std::bind(&Server::onInterest, this, _2),
                                 std::bind(&Server::onSuccess, this, _1),
                                 std::bind(&Server::onFailure, this, _1, _2)
        );
        requestNext();
    }

private:
    void requestNext()
    {
        //ndn::Name nextName = ndn::Name(m_basename).appendSequenceNumber(m_currentSeqNo);
        std::cout << "\nInterest for " << m_basename.toUri() << std::endl;

        m_face.expressInterest(ndn::Interest(m_basename).setMustBeFresh(true),
                               std::bind(&Server::onData, this, _2),
                               std::bind(&Server::onNack, this, _1),
                               std::bind(&Server::onTimeout, this, _1));
        //++m_currentSeqNo;
    }


    void onData(const ndn::Data& data)
    {
        temp = std::string(reinterpret_cast<const char*>(data.getContent().value()),
                           data.getContent().value_size());
        std::cout << ">> The temperature is "
                  << temp
                  << std::endl;
        sleep(5);
        requestNext();
    }

    void onNack(const ndn::Interest& interest){
        std::cout << ">> Nack for " << interest.toUri() << std::endl;
    }

    void onTimeout(const ndn::Interest& interest)
    {
        // re-express interest
        std::cout << ">> Timeout for " << interest.toUri() << std::endl;
        m_face.expressInterest(interest.getName(),
                               std::bind(&Server::onData, this, _2),
                               std::bind(&Server::onNack, this, _1),
                               std::bind(&Server::onTimeout, this, _1));
    }

    void onInterest(const ndn::Interest& interest){

        auto data = std::make_shared<ndn::Data>(interest.getName());
        data->setContent(reinterpret_cast<const uint8_t*>(temp.c_str()), temp.size());

        // set metainfo parameters
        data->setFreshnessPeriod(ndn::time::seconds(5));


        // sign data packet
        m_keyChain.sign(*data);

        // make data packet available for fetching
        m_face.put(*data);
    }

    void onSuccess(const ndn::Name& prefix){
        std::cout << "Prefix registered for " << prefix.toUri() << std::endl;
    }

    void onFailure(const ndn::Name& prefix, const std::string& reason){
        std::cout << "Failed to register prefix " << prefix.toUri() << std::endl;
        std::cout << "The reason: " << reason << std::endl;
    }

private:
    ndn::Face& m_face;
    ndn::Name m_basename;
    uint64_t m_currentSeqNo;
    ndn::KeyChain m_keyChain;
    std::string temp;
};

int main()
{
    try {
        // create Face instance
        ndn::Face face;

        // create client instance
        Server server(face, "/Senario/Weather/Sample/MyArea/Average");

        // start processing loop (it will block until everything is fetched)
        face.processEvents();
    }
    catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
    }

    return 0;
}
