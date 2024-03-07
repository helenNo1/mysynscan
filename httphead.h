#ifndef HTTPHEAD_HPP
#define HTTPHEAD_HPP

#include <iostream>
#include <fstream>
#include <regex>
#include "HTTPRequest.hpp"

int reqHead(const std::string &url)
{
    try
    {
        std::string uri;
        std::string method = "GET";
        std::string arguments;
        std::string output;
        auto protocol = http::InternetProtocol::v4;

        uri = url;
        http::Request request{uri, protocol};

        const auto response = request.send(method, arguments, {{"Content-Type", "application/x-www-form-urlencoded"}, {"User-Agent", "runscope/0.1"}, {"Accept", "*/*"}}, std::chrono::seconds(2));

        std::cout << response.status.reason << '\n';

        if (response.status.code == http::Status::Ok)
        {
            if (!output.empty())
            {
                std::ofstream outfile{output, std::ofstream::binary};
                outfile.write(reinterpret_cast<const char *>(response.body.data()),
                              static_cast<std::streamsize>(response.body.size()));
            }
            else
            {
                std::string res;
                std::regex title_reg("<title>(.*?)</title>");
                std::smatch m;

                res.insert(res.begin(), response.body.begin(), response.body.end());

                auto ret = std::regex_search(res, m, title_reg);
                if (ret)
                {
                    if (m.size() > 0)
                    {
                        std::cout << url << "-----" << m[1].str() << std::endl;

                        std::fstream f;
                        // 追加写入,在原来基础上加了ios::app
                        f.open("/tmp/2012", std::ios::out | std::ios::app);
                        // 输入你想写入的内容
                        f << url << "-----" << m[1].str()  << std::endl;
                        f.close();

                        // std::ofstream outfile{"/tmp/1955", std::ofstream::binary | std::ofstream::binary};
                        // outfile.write(url.c_str(), url.length());
                        // outfile.write(m[1].str().c_str(), m[1].str().length());
                        // outfile.write("\n", 1);
                        // outfile.close();
                    }
                }
                else
                {

                    // std::cout << res << std::endl;
                }

                // std::cout << std::string{response.body.begin(), response.body.end()} << '\n';
            }
        }
    }
    catch (const http::RequestError &e)
    {
        // std::cerr << "Request error: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
    catch (const http::ResponseError &e)
    {
        // std::cerr << "Response error: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
    catch (const std::exception &e)
    {
        // std::cerr << "Error: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
#endif