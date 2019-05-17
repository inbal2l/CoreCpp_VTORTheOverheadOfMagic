#include <iostream>
#include <boost/noncopyable.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/exception/diagnostic_information.hpp> 

using namespace boost::asio;
using namespace boost::asio::ip;
using namespace std;
using boost::asio::ip::tcp;
template <class SpecificProtocol>
class Protocol
{
public:

    Protocol():
        m_derived(static_cast<SpecificProtocol*>(this)),
		isDisconnect(false)
        {}
    
    void SetParams(void* m_params) 
    {
        m_derived->SetParams(m_params);
    }
    
    void Start()
    {   
        // TODO: call the child's connect() method
        m_derived->connect();
        time (&m_connectionStart);
        // TODO: call the child's receive() method in a loop
		while(!isDisconnect)
		{
			m_derived->receive();
		}
    }

    void Disconnect() 
    { 
        /* TODO: call the child’s disconnect() method */
        m_derived->disconnect();
    }
    size_t GetTotalReceivedBytes() 
    { 
         /* TODO: */    
        return m_derived->receive();
    }
    size_t GetReceivedBytesPerSec() 
    { 
        /* TODO: B/s since connection */
        return GetTotalReceivedBytes()/(m_connectionStart-time(NULL));
    }


protected:
    bool isDisconnect;
    time_t m_connectionStart;
    SpecificProtocol* m_derived;
    
    void connect() { cout<<"Error: the base protocol connect function was called"<<endl;   }
    void disconnect() { cout<<"Error: the base protocol disconnect function was called"<<endl; }
    size_t receive() { cout<<"Error: the base protocol receive function was called"<<endl;   return 0; } // Returns the number of received bytes
    bool hasData() { return false; }
};



class SampleProtocol1 : public Protocol<SampleProtocol1>, private boost::noncopyable
{
public:
	SampleProtocol1():
		m_isDisconnect(false),
		m_numBytes(0)
		{}
		
     void SetParams(void* param)
     {
        m_domain = *(static_cast<int*>(param));
     }
        
    // NOTE: can use a wrapper class for the protocol, and define it as friend, in order to avoid exposing the "Protocol" class to the user
    friend class Protocol;

protected:
    size_t m_numBytes;   
    int m_domain;
	boost::asio::io_service io_service;
	boost::asio::streambuf receive_buffer;
	tcp::socket *socket_ptr;	
	bool m_isDisconnect;

    void connect() 
    { 
        tcp::socket socket(io_service);
        socket_ptr = &socket;
		try{    	
				socket.connect( tcp::endpoint( boost::asio::ip::address::from_string("127.0.0.1"), 1234 ));
		}
		catch (boost::exception& error){
				std::cerr << "Exception: " << diagnostic_information(&error) << "\n";
		}
    }
	
    void disconnect() 
	{
		m_isDisconnect = true;
    }
	
    size_t receive() 
	{
		if (isDisconnect)
		{
			return 0;
		}
        boost::system::error_code error;
		boost::asio::read(*socket_ptr, receive_buffer, boost::asio::transfer_all(), error);
		if(error) 
		{
			cout << "receive failed: " << error.message()  << endl;
		}
		else 
		{
			const char* data = boost::asio::buffer_cast<const char*>(receive_buffer.data());
			cout << data << endl;
			m_numBytes += strlen(data);
		}
		
		return m_numBytes;
	}
	
    bool hasData()  { return false; }
};

int main()
{
    cout << "------TCP Protocol--------" << endl;
    Protocol<SampleProtocol1> myProtocol;
    int i=3;
    myProtocol.SetParams(&i);
    myProtocol.Start();   
}