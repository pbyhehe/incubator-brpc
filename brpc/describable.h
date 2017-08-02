// Baidu RPC - A framework to host and access services throughout Baidu.
// Copyright (c) 2014 Baidu.com, Inc. All Rights Reserved

// Author: The baidu-rpc authors (pbrpc@baidu.com)
// Date: Wed Dec 31 21:51:29 CST 2014

#ifndef BRPC_DESCRIBABLE_H
#define BRPC_DESCRIBABLE_H

#include <ostream>
#include "base/macros.h"
#include "base/class_name.h"

namespace brpc {

struct DescribeOptions {
    DescribeOptions()
        : verbose(true)
        , use_html(false)
    {}

    bool verbose;
    bool use_html;
};

class Describable {
public:
    virtual ~Describable() {}
    virtual void Describe(std::ostream& os, const DescribeOptions&) const {
        os << base::class_name_str(*this);
    }
};

class NonConstDescribable {
public:
    virtual ~NonConstDescribable() {}
    virtual void Describe(std::ostream& os, const DescribeOptions&) {
        os << base::class_name_str(*this);
    }
};

inline std::ostream& operator<<(std::ostream& os, const Describable& obj) {
    DescribeOptions options;
    options.verbose = false;
    obj.Describe(os, options);
    return os;
}

inline std::ostream& operator<<(std::ostream& os,
                                NonConstDescribable& obj) {
    DescribeOptions options;
    options.verbose = false;
    obj.Describe(os, options);
    return os;
}

} // namespace brpc

// Append `indent' spaces after each newline.
// Example:
//   IndentingOStream os1(std::cout, 2);
//   IndentingOStream os2(os1, 2);
//   std::cout << "begin1\nhello" << std::endl << "world\nend1" << std::endl;
//   os1 << "begin2\nhello" << std::endl << "world\nend2" << std::endl;
//   os2 << "begin3\nhello" << std::endl << "world\nend3" << std::endl;
// Output:
// begin1
// hello
// world
// end1
// begin2
//   hello
//   world
//   end2
//   begin3
//     hello
//     world
//     end3
class IndentingOStream : virtual private std::streambuf, public std::ostream {
public:
    IndentingOStream(std::ostream& dest, int indent)
        : std::ostream(this)
        , _dest(dest.rdbuf())
        , _is_at_start_of_line(false)
        , _indent(indent, ' ')
    {}
protected:
    virtual int overflow(int ch) {
        if (_is_at_start_of_line && ch != '\n' ) {
            _dest->sputn(_indent.data(), _indent.size());
        }
        _is_at_start_of_line = (ch == '\n');
        return _dest->sputc(ch);
    }
private:
    DISALLOW_COPY_AND_ASSIGN(IndentingOStream);
    std::streambuf* _dest;
    bool _is_at_start_of_line;
    std::string _indent;
};

#endif  // BRPC_DESCRIBABLE_H
