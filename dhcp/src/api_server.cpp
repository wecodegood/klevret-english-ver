#include "api_server.hpp"

#include <boost/asio.hpp>

ApiServer& ApiServer::instance(){
    if (!_api_server){
        _mutex.lock();
        if (!_api_server){
            _api_server = new ApiServer();
        }
    }
    return *_api_server;
}


void ApiServer::start(){

}
