/*
 * Copyright 2020 The casbin Authors. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef CASBIN_CPP_MODEL_EXPRTK_CONFIG
#define CASBIN_CPP_MODEL_EXPRTK_CONFIG

#include <memory>

#include "casbin/exprtk/exprtk.hpp"
#include "casbin/rbac/default_role_manager.h"
#include "casbin/rbac/role_manager.h"
#include "casbin/util/util.h"

namespace casbin {
using numerical_type = std::string;

using symbol_table_t = exprtk::symbol_table<numerical_type>;
using expression_t = exprtk::expression<numerical_type>;
using parser_t = exprtk::parser<numerical_type>;
using exprtk_func_t = exprtk::igeneric_function<numerical_type>;

struct ExprtkGFunction : public exprtk::igeneric_function<numerical_type> {
    typedef typename exprtk::igeneric_function<numerical_type>::generic_type generic_type;

    typedef typename generic_type::scalar_view scalar_t;
    typedef typename generic_type::vector_view vector_t;
    typedef typename generic_type::string_view string_t;

    typedef typename exprtk::igeneric_function<numerical_type>::parameter_list_t parameter_list_t;

private:
    std::shared_ptr<casbin::RoleManager> rm_;

public:
    ExprtkGFunction(const std::string& idenfier) : exprtk::igeneric_function<numerical_type>(idenfier), rm_(nullptr) {}

    ExprtkGFunction(const std::string& idenfier, std::shared_ptr<RoleManager> rm) : exprtk::igeneric_function<numerical_type>(idenfier), rm_(rm) {}

    bool UpdateRoleManager(std::shared_ptr<RoleManager> rm) {
        this->rm_ = rm;

        return true;
    }

    inline numerical_type operator()(parameter_list_t parameters) {
        numerical_type res = "";

        // check value cnt
        if (parameters.size() != 2 && parameters.size() != 3) {
            return res;
        }

        // check value type
        for (std::size_t i = 0; i < parameters.size(); ++i) {
            generic_type& gt = parameters[i];

            if (generic_type::e_scalar == gt.type) {
                return res;
            } else if (generic_type::e_vector == gt.type) {
                return res;
            }
        }

        std::string name1 = exprtk::to_str(string_t(parameters[0]));
        std::string name2 = exprtk::to_str(string_t(parameters[1]));
        std::string domain;
        std::vector<std::string> domains;

        if (parameters.size() == 3) {
            domain = exprtk::to_str(string_t(parameters[2]));
            domains.push_back(domain);
        }

        if (this->rm_ == nullptr)
            res = name1 == name2 ? "T" : "";
        else {
            res = rm_->HasLink(name1, name2, domains) ? "T" : "";
        }

        return res;
    }
};

struct ExprtkMatchFunction : public exprtk::igeneric_function<numerical_type> {
    typedef typename exprtk::igeneric_function<numerical_type>::generic_type generic_type;

    typedef typename generic_type::scalar_view scalar_t;
    typedef typename generic_type::vector_view vector_t;
    typedef typename generic_type::string_view string_t;

    typedef typename exprtk::igeneric_function<numerical_type>::parameter_list_t parameter_list_t;

private:
    casbin::MatchingFunc func_;

public:
    ExprtkMatchFunction(const std::string& idenfier, casbin::MatchingFunc func) : exprtk::igeneric_function<numerical_type>(idenfier), func_(func) {}

    ExprtkMatchFunction() : exprtk::igeneric_function<numerical_type>("ss") {}

    inline numerical_type operator()(parameter_list_t parameters) {
        numerical_type res = "";

        // check value cnt
        if (parameters.size() != 2) {
            return res;
        }

        // check value type
        for (std::size_t i = 0; i < parameters.size(); ++i) {
            generic_type& gt = parameters[i];

            if (generic_type::e_scalar == gt.type) {
                return res;
            } else if (generic_type::e_vector == gt.type) {
                return res;
            }
        }

        std::string name1 = exprtk::to_str(string_t(parameters[0]));
        std::string name2 = exprtk::to_str(string_t(parameters[1]));

        if (this->func_ == nullptr)
            res = name1 == name2 ? "T" : "";
        else {
            res = this->func_(name1, name2) ? "T" : "";
        }

        return res;
    }
};

struct ExprtkGetFunction : public exprtk::igeneric_function<numerical_type> {
    typedef typename exprtk::igeneric_function<numerical_type> igenfunct_t;
    typedef typename exprtk::igeneric_function<numerical_type>::generic_type generic_type;

    typedef typename generic_type::scalar_view scalar_t;
    typedef typename generic_type::vector_view vector_t;
    typedef typename generic_type::string_view string_t;

    typedef typename exprtk::igeneric_function<numerical_type>::parameter_list_t parameter_list_t;

private:
    casbin::GettingFunc func_;

public:
    ExprtkGetFunction(const std::string& idenfier, casbin::GettingFunc func) : exprtk::igeneric_function<numerical_type>(idenfier), func_(func) {}
    
    ExprtkGetFunction() : exprtk::igeneric_function<numerical_type>("ss", igenfunct_t::e_rtrn_string) {}

    inline numerical_type operator()(parameter_list_t parameters) {
        numerical_type res = "";

        // check value cnt
        if (parameters.size() != 2) {
            return res;
        }

        // check value type
        for (std::size_t i = 0; i < parameters.size(); ++i) {
            generic_type& gt = parameters[i];

            if (generic_type::e_scalar == gt.type) {
                return res;
            } else if (generic_type::e_vector == gt.type) {
                return res;
            }
        }

        std::string name1 = exprtk::to_str(string_t(parameters[0]));
        std::string name2 = exprtk::to_str(string_t(parameters[1]));

        if (this->func_ != nullptr)
            res = this->func_(name1, name2);

        return res;
    }
};

struct ExprtkGetWithPathFunction : public exprtk::igeneric_function<numerical_type> {
    typedef typename exprtk::igeneric_function<numerical_type> igenfunct_t;
    typedef typename exprtk::igeneric_function<numerical_type>::generic_type generic_type;

    typedef typename generic_type::scalar_view scalar_t;
    typedef typename generic_type::vector_view vector_t;
    typedef typename generic_type::string_view string_t;

    typedef typename exprtk::igeneric_function<numerical_type>::parameter_list_t parameter_list_t;

private:
    casbin::GetWithPathFunc func_;

public:
    ExprtkGetWithPathFunction(const std::string& idenfier, casbin::GetWithPathFunc func) : exprtk::igeneric_function<numerical_type>(idenfier), func_(func) {}

    ExprtkGetWithPathFunction() : exprtk::igeneric_function<numerical_type>("sss", igenfunct_t::e_rtrn_string) {}

    inline numerical_type operator()(parameter_list_t parameters) {
        numerical_type res = "";

        // check value cnt
        if (parameters.size() != 3) {
            return res;
        }

        // check value type
        for (std::size_t i = 0; i < parameters.size(); ++i) {
            generic_type& gt = parameters[i];

            if (generic_type::e_scalar == gt.type) {
                return res;
            } else if (generic_type::e_vector == gt.type) {
                return res;
            }
        }

        std::string name1 = exprtk::to_str(string_t(parameters[0]));
        std::string name2 = exprtk::to_str(string_t(parameters[1]));
        std::string name3 = exprtk::to_str(string_t(parameters[2]));

        if (this->func_ != nullptr)
            res = this->func_(name1, name2, name3);

        return res;
    }
};

enum class ExprtkFunctionType {
    Unknown,
    Gfunction,
    KeyMatch,
    KeyMatch2,
    KeyMatch3,
    KeyMatch4,
    RegexMatch,
    IpMatch,
    KeyGet,
    KeyGet2,
    KeyGet3,
};

class ExprtkFunctionFactory {
public:
    static std::shared_ptr<exprtk_func_t> GetExprtkFunction(ExprtkFunctionType type, int narg, std::shared_ptr<RoleManager> rm = nullptr) {
        std::string idenfier(narg, 'S');
        std::shared_ptr<exprtk_func_t> func = nullptr;
        switch (type) {
            case ExprtkFunctionType::Gfunction:
                func = std::make_shared<ExprtkGFunction>(idenfier, rm);
                break;
            case ExprtkFunctionType::KeyMatch:
                func.reset(new ExprtkMatchFunction(idenfier, KeyMatch));
                break;
            case ExprtkFunctionType::KeyMatch2:
                func.reset(new ExprtkMatchFunction(idenfier, KeyMatch2));
                break;
            case ExprtkFunctionType::KeyMatch3:
                func.reset(new ExprtkMatchFunction(idenfier, KeyMatch3));
                break;
            case ExprtkFunctionType::KeyMatch4:
                func.reset(new ExprtkMatchFunction(idenfier, KeyMatch4));
                break;
            case ExprtkFunctionType::IpMatch:
                func.reset(new ExprtkMatchFunction(idenfier, IPMatch));
                break;
            case ExprtkFunctionType::RegexMatch:
                func.reset(new ExprtkMatchFunction(idenfier, RegexMatch));
                break;
            case ExprtkFunctionType::KeyGet:
                func.reset(new ExprtkGetFunction(idenfier, KeyGet));
                break;
            case ExprtkFunctionType::KeyGet2:
                func.reset(new ExprtkGetWithPathFunction(idenfier, KeyGet2));
                break;
            case ExprtkFunctionType::KeyGet3:
                func.reset(new ExprtkGetWithPathFunction(idenfier, KeyGet3));
                break;
            default:
                func = nullptr;
        }

        return func;
    }
};
} // namespace casbin

#endif