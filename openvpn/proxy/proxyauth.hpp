//
//  proxyauth.hpp
//  OpenVPN
//
//  Copyright (c) 2012 OpenVPN Technologies, Inc. All rights reserved.
//

#ifndef OPENVPN_PROXY_PROXYAUTH_H
#define OPENVPN_PROXY_PROXYAUTH_H

#include <vector>

#include <openvpn/common/rc.hpp>
#include <openvpn/common/split.hpp>
#include <openvpn/http/header.hpp>

namespace openvpn {
  namespace HTTPProxy {

    // parse the Proxy-Authenticate HTTP header
    class ProxyAuthenticate : public RC<thread_unsafe_refcount> {
    public:
      typedef boost::intrusive_ptr<ProxyAuthenticate> Ptr;

      ProxyAuthenticate() {}

      ProxyAuthenticate(const std::string& line)
      {
	do_parse(line);
      }

      void parse(const std::string& line)
      {
	method = "";
	parms.clear();
	do_parse(line);
      }

      std::string to_string() const
      {
	std::ostringstream out;
	out << "Proxy-Authenticate header" << std::endl;
	out << "method=" << method << std::endl;
	out << parms.to_string();
	return out.str();
      }

      std::string method;
      HTTP::HeaderList parms;

    private:
      void do_parse(const std::string& line)
      {
	std::vector<std::string> tuple = Split::by_char<std::vector<std::string>, StandardLex, Split::NullLimit>(line, ' ', 0, 1);
	if (tuple.size() >= 1)
	  method = tuple[0];
	if (tuple.size() == 2)
	  {
	    std::vector<std::string> list = Split::by_char<std::vector<std::string>, StandardLex, Split::NullLimit>(tuple[1], ',', Split::TRIM_LEADING_SPACES|Split::TRIM_SPECIAL);
	    for (std::vector<std::string>::const_iterator i = list.begin(); i != list.end(); ++i)
	      {
		std::vector<std::string> pair = Split::by_char<std::vector<std::string>, StandardLex, Split::NullLimit>(*i, '=', 0, 1);
		if (pair.size() == 2)
		  parms.push_back(HTTP::Header(pair[0], pair[1]));
	      }
	  }
      }
    };
  }
}

#endif