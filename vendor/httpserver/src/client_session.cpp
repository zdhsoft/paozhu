#include "http_socket.h"
#include "http2_frame.h"
#include "client_session.h"
#include "http2_parse.h"
#include "terminal_color.h"
#include "http2_define.h"
#include "http2_huffman.h"
#include "https_brotli.h"
#include "unicode.h"
#include "directory_fun.h"
#include "http_header.h"
#include "clientdatacache.h"
namespace http
{
client_session::client_session(std::list<asio::ip::tcp::socket> sock) : _socket(std::move(sock))
{
    auto &cc    = get_client_data_cache();
    _cache_data = cc.get_data_ptr();
    // cache_back_obj.setptr(_cache_data);
    isssl = false;
}

client_session::client_session(std::list<asio::ssl::stream<asio::ip::tcp::socket>> sslsocket)
    : _sslsocket(std::move(sslsocket))
{
    auto &cc    = get_client_data_cache();
    _cache_data = cc.get_data_ptr();

    // cache_back_obj.setptr(_cache_data);
    isssl = true;
}
client_session::~client_session()
{
    if (_cache_data != nullptr)
    {
        auto &cc = get_client_data_cache();
        cc.back_data_ptr(_cache_data);
    }
}
std::shared_ptr<client_session> client_session::get_ptr() { return shared_from_this(); }

bool client_session::send_data(const std::string &msg)
{
    if (isclose)
    {
        return false;
    }
    try
    {
        if (isssl)
        {
            if (_sslsocket.front().lowest_layer().is_open())
            {
                asio::write(_sslsocket.front(), asio::buffer(msg));
            }
            else
            {
                return false;
            }
        }
        else
        {
            if (_socket.front().is_open())
            {
                asio::write(_socket.front(), asio::buffer(msg));
            }
            else
            {
                return false;
            }
        }
        return true;
    }
    catch (std::exception &)
    {
        return false;
    }
}
bool client_session::isopensocket()
{

    try
    {
        if (isssl)
        {
            if (_sslsocket.front().lowest_layer().is_open())
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            if (_socket.front().is_open())
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }
    catch (std::exception &)
    {
        return false;
    }
}

bool client_session::send_switch101()
{
    try
    {
        std::string tempswitch = "HTTP/1.1 101 Switching Protocols\r\nConnection: Upgrade\r\nUpgrade: h2c\r\n\r\n";
        if (isssl)
        {
            asio::write(_sslsocket.front(), asio::buffer(tempswitch));
        }
        else
        {

            asio::write(_socket.front(), asio::buffer(tempswitch));
        }
        return true;
    }
    catch (std::exception &)
    {
        return false;
    }
}
bool client_session::send_setting()
{

    try
    {
        unsigned char _setting[] = {0x00, 0x00, 0x0C, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x64, 0x00, 0x04, 0x00, 0xFF, 0xFF, 0xFF};

        // if (isssl)
        // {
        //     asio::write(_sslsocket.front(), asio::buffer(_setting, 21));
        // }
        // else
        // {
        //     asio::write(_socket.front(), asio::buffer(_setting, 21));
        // }
        std::string msg((char *)_setting, 21);//= {0x00, 0x00, 0x0C, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x64, 0x00, 0x04, 0x00, 0xFF, 0xFF, 0xFF};
        //msg.append((char *)_setting, 21);
        setting_lists.push(std::move(msg));
        if (!sendtype)
        {
            http2_send_data_loop();
        }
        return true;
    }
    catch (std::exception &)
    {
        return false;
    }
}

asio::awaitable<void> client_session::co_send_setting()
{

    try
    {
        unsigned char _setting[] = {0x00, 0x00, 0x0C, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x64, 0x00, 0x04, 0x00, 0xFF, 0xFF, 0xFF};

        if (isssl)
        {
            co_await asio::async_write(_sslsocket.front(), asio::buffer(_setting, 21), asio::use_awaitable);
        }
        else
        {
            co_await asio::async_write(_socket.front(), asio::buffer(_setting, 21), asio::use_awaitable);
        }
        co_return;
    }
    catch (std::exception &)
    {
        co_return;
    }
}
bool client_session::send_enddata(unsigned int s_stream_id)
{

    try
    {
        unsigned char _recvack[] = {0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00};
        _recvack[8]              = s_stream_id & 0xFF;
        s_stream_id              = s_stream_id >> 8;
        _recvack[7]              = s_stream_id & 0xFF;
        s_stream_id              = s_stream_id >> 8;
        _recvack[6]              = s_stream_id & 0xFF;
        s_stream_id              = s_stream_id >> 8;
        _recvack[5]              = s_stream_id & 0x7F;
        if (isssl)
        {
            asio::write(_sslsocket.front(), asio::buffer(_recvack, 9));
        }
        else
        {
            asio::write(_socket.front(), asio::buffer(_recvack, 9));
        }
        return true;
    }
    catch (std::exception &)
    {
        return false;
    }
}

asio::awaitable<void> client_session::co_send_enddata(unsigned int s_stream_id)
{
    try
    {
        unsigned char _recvack[] = {0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00};
        _recvack[8]              = s_stream_id & 0xFF;
        s_stream_id              = s_stream_id >> 8;
        _recvack[7]              = s_stream_id & 0xFF;
        s_stream_id              = s_stream_id >> 8;
        _recvack[6]              = s_stream_id & 0xFF;
        s_stream_id              = s_stream_id >> 8;
        _recvack[5]              = s_stream_id & 0x7F;
        // if (isssl)
        // {

        //     co_await asio::async_write(_sslsocket.front(), asio::buffer(_recvack, 9), asio::use_awaitable);
        // }
        // else
        // {

        //     co_await asio::async_write(_socket.front(), asio::buffer(_recvack, 9), asio::use_awaitable);
        // }
        std::string msg((char *)_recvack, 9);// = {0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00};
        //msg.append((char *)_recvack, 9);
        setting_lists.push(std::move(msg));
        if (!sendtype)
        {
            co_await http2_send_data_loop_co();
        }
        co_return;
    }
    catch (std::exception &)
    {
        co_return;
    }
}

bool client_session::send_goway()
{

    try
    {
        unsigned char _recvack[] =
            {0x00, 0x00, 0x08, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        // if (isssl)
        // {
        //     asio::write(_sslsocket.front(), asio::buffer(_recvack, 17));
        // }
        // else
        // {
        //     asio::write(_socket.front(), asio::buffer(_recvack, 17));
        // }
        std::string msg((char *)_recvack, 17);
        //msg.append((char *)_recvack, 17);
        setting_lists.push(std::move(msg));
        if (!sendtype)
        {
            http2_send_data_loop();
        }
        return true;
    }
    catch (std::exception &)
    {
        return false;
    }
}
bool client_session::send_recv_setting()
{

    try
    {
        unsigned char _recvack[] = {0x00, 0x00, 0x00, 0x04, 0x01, 0x00, 0x00, 0x00, 0x00};
        if (isssl)
        {
            asio::write(_sslsocket.front(), asio::buffer(_recvack, 9));
        }
        else
        {
            asio::write(_socket.front(), asio::buffer(_recvack, 9));
        }
        return true;
    }
    catch (std::exception &)
    {
        return false;
    }
}
void client_session::send_window_update(unsigned int up_num, unsigned int stmid)
{

    unsigned char _recvack[] = {0x00, 0x00, 0x04, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    _recvack[12]             = up_num & 0xFF;
    up_num                   = up_num >> 8;
    _recvack[11]             = up_num & 0xFF;
    up_num                   = up_num >> 8;
    _recvack[10]             = up_num & 0xFF;
    up_num                   = up_num >> 8;
    _recvack[9]              = up_num & 0xFF;

    std::string msg;
    msg.resize(13);
    for (int i = 0; i < 13; i++)
    {
        msg[i] = _recvack[i];
    }
    //send_data(msg);
    setting_lists.push(msg);

    _recvack[8] = stmid & 0xFF;
    stmid       = stmid >> 8;
    _recvack[7] = stmid & 0xFF;
    stmid       = stmid >> 8;
    _recvack[6] = stmid & 0xFF;
    stmid       = stmid >> 8;
    _recvack[5] = stmid & 0xFF;

    msg.clear();
    for (int i = 0; i < 13; i++)
    {
        msg[i] = _recvack[i];
    }
    //send_data(msg);
    setting_lists.push(msg);
    if (!sendtype)
    {
        http2_send_data_loop();
    }
}
void client_session::recv_window_update(unsigned int up_num, unsigned int stmid)
{

    unsigned char _recvack[] = {0x00, 0x00, 0x04, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    _recvack[12]             = up_num & 0xFF;
    up_num                   = up_num >> 8;
    _recvack[11]             = up_num & 0xFF;
    up_num                   = up_num >> 8;
    _recvack[10]             = up_num & 0xFF;
    up_num                   = up_num >> 8;
    _recvack[9]              = up_num & 0xFF;

    std::string msg;
    msg.resize(13);
    for (int i = 0; i < 13; i++)
    {
        msg[i] = _recvack[i];
    }

    _recvack[8] = stmid & 0xFF;
    stmid       = stmid >> 8;
    _recvack[7] = stmid & 0xFF;
    stmid       = stmid >> 8;
    _recvack[6] = stmid & 0xFF;
    stmid       = stmid >> 8;
    _recvack[5] = stmid & 0xFF;

    // msg.resize(13);
    for (int i = 0; i < 13; i++)
    {
        msg.push_back(_recvack[i]);
    }

    //send_data(msg);
    setting_lists.push(msg);
    if (!sendtype)
    {
        http2_send_data_loop();
    }
}

bool client_session::send_data(const unsigned char *buffer, unsigned int buffersize)
{
    if (isclose)
    {
        return false;
    }

    try
    {
        if (isssl)
        {
            if (_sslsocket.front().lowest_layer().is_open())
            {
                asio::write(_sslsocket.front(), asio::buffer(buffer, buffersize));
            }
            else
            {
                return false;
            }
        }
        else
        {
            if (_socket.front().is_open())
            {
                asio::write(_socket.front(), asio::buffer(buffer, buffersize));
            }
            else
            {
                return false;
            }
        }
        return true;
    }
    catch (std::exception &)
    {
        return false;
    }
}
void client_session::http2_send_data(const unsigned char *buffer, unsigned int buffersize)
{
    std::string msg;
    msg.append((char *)buffer, buffersize);
    setting_lists.push(std::move(msg));
    if (!sendtype)
    {
        http2_send_data_loop();
    }
}
void client_session::http2_send_data(const std::string &msg)
{
    setting_lists.push(msg);
    if (!sendtype)
    {
        http2_send_data_loop();
    }
}
void client_session::http2_send_data_loop()
{
    try
    {
        while (true)
        {
            if (setting_lists.size() > 0)
            {
                sendtype    = true;
                auto buffer = std::move(setting_lists.front());
                setting_lists.pop();
                if (isssl)
                {
                    if (_sslsocket.front().lowest_layer().is_open())
                    {
                        asio::write(_sslsocket.front(), asio::buffer(buffer));
                    }
                    else
                    {
                        return;
                    }
                }
                else
                {
                    if (_socket.front().is_open())
                    {
                        asio::write(_socket.front(), asio::buffer(buffer));
                    }
                    else
                    {
                        return;
                    }
                }
            }
            else
            {
                sendtype = false;
                break;
            }
        }
    }
    catch (const std::exception &e)
    {
    }
}
asio::awaitable<void> client_session::http2_send_data_loop_co()
{
    try
    {
        while (true)
        {
            if (setting_lists.size() > 0)
            {
                sendtype    = true;
                auto buffer = std::move(setting_lists.front());
                setting_lists.pop();
                if (isssl)
                {
                    if (_sslsocket.front().lowest_layer().is_open())
                    {
                        asio::write(_sslsocket.front(), asio::buffer(buffer));
                    }
                    else
                    {
                        co_return;
                    }
                }
                else
                {
                    if (_socket.front().is_open())
                    {
                        asio::write(_socket.front(), asio::buffer(buffer));
                    }
                    else
                    {
                        co_return;
                    }
                }
            }
            else
            {
                sendtype = false;
                break;
            }
        }
    }
    catch (const std::exception &e)
    {
    }
    co_return;
}

asio::awaitable<void> client_session::loopwriter(const unsigned char *buffer, unsigned int buffersize)
{
    try
    {
        if (buffersize == 0)
        {
            co_return;
        }
        if (isssl)
        {
            co_await asio::async_write(_sslsocket.front(), asio::buffer(buffer, buffersize), asio::use_awaitable);
        }
        else
        {
            co_await asio::async_write(_socket.front(), asio::buffer(buffer, buffersize), asio::use_awaitable);
        }
    }
    catch (...)
    {
        stop();
    }
}

asio::awaitable<void> client_session::http2_send_writer(const unsigned char *buffer, unsigned int buffersize)
{
    if (isclose)
    {
        co_return;
    }
    try
    {
        if (buffersize == 0)
        {
            co_return;
        }
        if (sendtype)
        {
            std::string msg((char *)buffer, buffersize);
            //msg.append(buffer, buffersize);
            setting_lists.push(std::move(msg));
            if (!sendtype)
            {
                co_await http2_send_data_loop_co();
            }
            co_return;
        }
        if (isssl)
        {
            sendtype = true;
            co_await asio::async_write(_sslsocket.front(), asio::buffer(buffer, buffersize), asio::use_awaitable);
            sendtype = false;
            if (!setting_lists.empty())
            {
                co_await http2_send_data_loop_co();
            }
            co_return;
        }
        else
        {
            sendtype = true;
            co_await asio::async_write(_socket.front(), asio::buffer(buffer, buffersize), asio::use_awaitable);
            sendtype = false;
            if (!setting_lists.empty())
            {
                co_await http2_send_data_loop_co();
            }
            co_return;
        }
    }
    catch (...)
    {
        stop();
    }
    co_return;
}

asio::awaitable<void> client_session::http2_send_writer(const std::string &msg)
{
    if (isclose)
    {
        co_return;
    }
    try
    {
        if (msg.size() == 0)
        {
            co_return;
        }

        if (sendtype)
        {
            setting_lists.push(msg);
            if (!sendtype)
            {
                co_await http2_send_data_loop_co();
            }
            co_return;
        }

        if (isssl)
        {
            sendtype = true;
            co_await asio::async_write(_sslsocket.front(), asio::buffer(msg), asio::use_awaitable);
            sendtype = false;
            if (!setting_lists.empty())
            {
                co_await http2_send_data_loop_co();
            }
            co_return;
        }
        else
        {
            sendtype = true;
            co_await asio::async_write(_socket.front(), asio::buffer(msg), asio::use_awaitable);
            sendtype = false;
            if (!setting_lists.empty())
            {
                co_await http2_send_data_loop_co();
            }
            co_return;
        }
    }
    catch (...)
    {
        stop();
    }
    co_return;
}

asio::awaitable<void> client_session::send_writer(const unsigned char *buffer, unsigned int buffersize)
{
    if (isclose)
    {
        co_return;
    }
    try
    {
        if (buffersize == 0)
        {
            co_return;
        }
        if (isssl)
        {
            co_await asio::async_write(_sslsocket.front(), asio::buffer(buffer, buffersize), asio::use_awaitable);
        }
        else
        {
            co_await asio::async_write(_socket.front(), asio::buffer(buffer, buffersize), asio::use_awaitable);
        }
    }
    catch (...)
    {
        stop();
    }
}

asio::awaitable<void> client_session::send_writer(const std::string &msg)
{
    if (isclose)
    {
        co_return;
    }
    try
    {
        if (msg.size() == 0)
        {
            co_return;
        }
        if (isssl)
        {
            co_await asio::async_write(_sslsocket.front(), asio::buffer(msg), asio::use_awaitable);
        }
        else
        {
            co_await asio::async_write(_socket.front(), asio::buffer(msg), asio::use_awaitable);
        }
    }
    catch (...)
    {
        stop();
    }
}

asio::awaitable<void> client_session::co_send_writer(const unsigned char *buffer, unsigned int buffersize)
{
    if (isclose)
    {
        co_return;
    }
    try
    {
        if (buffersize == 0)
        {
            co_return;
        }
        if (isssl)
        {
            co_await asio::async_write(_sslsocket.front(), asio::buffer(buffer, buffersize), asio::use_awaitable);
        }
        else
        {
            co_await asio::async_write(_socket.front(), asio::buffer(buffer, buffersize), asio::use_awaitable);
        }
    }
    catch (...)
    {
        stop();
    }
}

asio::awaitable<void> client_session::co_send_writer(const std::string &msg)
{
    if (isclose)
    {
        co_return;
    }
    try
    {
        if (msg.size() == 0)
        {
            co_return;
        }
        if (isssl)
        {
            co_await asio::async_write(_sslsocket.front(), asio::buffer(msg), asio::use_awaitable);
        }
        else
        {
            co_await asio::async_write(_socket.front(), asio::buffer(msg), asio::use_awaitable);
        }
    }
    catch (...)
    {
        stop();
    }
}

void client_session::stop()
{
    isclose = true;
    if (isssl)
    {
        asio::error_code ec;
        _sslsocket.front().shutdown(ec);
        if (_sslsocket.front().lowest_layer().is_open())
        {
            _sslsocket.front().lowest_layer().close();
        }
    }
    else
    {
        _socket.front().close();
    }

    // timer_.cancel();
}

std::string client_session::getremoteip()
{
    if (isssl)
    {
        client_ip = _sslsocket.front().lowest_layer().remote_endpoint().address().to_string();
    }
    else
    {
        client_ip = _socket.front().remote_endpoint().address().to_string();
    }
    return client_ip;
}
unsigned int client_session::getremoteport()
{
    if (isssl)
    {
        client_port = _sslsocket.front().lowest_layer().remote_endpoint().port();
    }
    else
    {
        client_port = _socket.front().remote_endpoint().port();
    }
    return client_port;
}
std::string client_session::getlocalip()
{
    if (isssl)
    {
        server_ip = _sslsocket.front().lowest_layer().local_endpoint().address().to_string();
    }
    else
    {
        server_ip = _socket.front().local_endpoint().address().to_string();
    }
    return server_ip;
}
unsigned int client_session::getlocalport()
{
    if (isssl)
    {
        server_port = _sslsocket.front().lowest_layer().local_endpoint().port();
    }
    else
    {
        server_port = _socket.front().local_endpoint().port();
    }
    return server_port;
}

}// namespace http
