#ifndef KIE_JSON_KIE_JSON_H
#define KIE_JSON_KIE_JSON_H

#include <string>
#include <string_view>
#include <type_traits>
#include <vector>
#include <list>
#include <array>

#include <iostream>

#include <boost/pfr.hpp>
#include <nlohmann/json.hpp>

namespace kie::json
{

    template <std::size_t N>
    class StringLiteral
    {
    public:
        static_assert(N >= 1, "string literal size must be more than 1");

        char value[N];

        constexpr StringLiteral(const char (&str)[N])
        {
            std::copy_n(str, N, value);
        }

        [[nodiscard]] constexpr std::string_view to_string_view() const
        {
            return {value, N - 1};
        }
    };

    template <typename T, StringLiteral _tag>
    struct Field
    {
        using Type = T;
        T value;

        Field() = default;

        Field(const T &v)
        {
            value = v;
        }

        Field(T &&v)
        {
            value = std::move(v);
        }

        Field(const Field &v)
        {
            value = v.value;
        }

        Field(Field &&v)
        {
            value = std::move(v.value);
        }

        Field &operator=(const Field &v)
        {
            value = v.value;
            return *this;
        }

        Field &operator=(Field &&v)
        {
            value = std::move(v.value);
            return *this;
        }

        constexpr std::string_view tag() const
        {
            return _tag.to_string_view();
        }

        operator T &()
        {
            return value;
        }
    };

    namespace type_trait
    {
        template <class...>
        constexpr std::false_type not_implemented{};

        template <typename T, template <typename...> class Z>
        struct is_specialization_of : std::false_type
        {
        };

        template <typename... Args, template <typename...> class Z>
        struct is_specialization_of<Z<Args...>, Z> : std::true_type
        {
        };

        template <typename T>
        struct is_array_class : std::false_type
        {
        };

        template <typename T, std::size_t size>
        struct is_array_class<std::array<T, size>> : std::true_type
        {
        };

        template <typename T>
        concept is_container = is_specialization_of<T, std::vector>::value ||
            is_specialization_of<T, std::list>::value ||
            is_array_class<T>::value;

        template <typename T>
        concept is_dynamic_container = (is_specialization_of<T, std::vector>::value ||
                                        is_specialization_of<T, std::list>::value) &&
                                       requires
        {
            typename T::value_type;
        };

        template <typename T>
        struct is_field : std::false_type
        {
        };

        template <typename T, StringLiteral str>
        struct is_field<Field<T, str>> : std::true_type
        {
        };
    }

    template <type_trait::is_container T>
    nlohmann::json to_json(const T &t);

    template <typename T>
    nlohmann::json to_json(const T &t)
    {
        nlohmann::json j;
        boost::pfr::for_each_field(t, [&j]<typename TT>(const TT &field, std::size_t)
                                   {
            if constexpr(type_trait::is_field<TT>::value){
                if constexpr(std::is_class_v<typename TT::Type> && !std::is_same_v<typename TT::Type, std::string>){
                    j[std::string{field.tag()}] = to_json(field.value);
                }else{
                    j[std::string{field.tag()}] = field.value;
                }
            } });
        return j;
    }

    template <>
    nlohmann::json to_json(const std::string &)
    {
        nlohmann::json j;
        return j; // return null for string without wrapping with JsonField
    }

    template <type_trait::is_container T>
    nlohmann::json to_json(const T &t)
    {
        nlohmann::json j;
        for (const auto &item : t)
        {
            if constexpr (std::is_class_v<std::decay_t<decltype(item)>> && !std::is_same_v<std::decay_t<decltype(item)>, std::string>)
            {
                j.push_back(to_json(item));
            }
            else
            {
                j.push_back(item);
            }
        }
        return j;
    }

    namespace impl
    {
        template <typename T>
        T from_json(const nlohmann::json &j)
        {
            T t{};
            j.get_to(t);
            return t;
        }

        template <type_trait::is_dynamic_container T>
        T from_json(const nlohmann::json &j);

        template <typename T>
        requires std::is_aggregate_v<T>
            T from_json(const nlohmann::json &j)
        {
            T t{};
            boost::pfr::for_each_field(t, [&j]<typename TT>(TT &field, std::size_t)
                                       {
                if constexpr(type_trait::is_field<TT>::value){
                    if constexpr(std::is_class_v<typename TT::Type> && !std::is_same_v<typename TT::Type, std::string>){
                        field = impl::from_json<typename TT::Type>(j.at(std::string{field.tag()}));
                    }else{
                        j.at(std::string{field.tag()}).get_to(field.value);
                    }
                } });
            return t;
        }

        template <type_trait::is_dynamic_container T>
        T from_json(const nlohmann::json &j)
        {
            if (!j.is_array())
            {
                return {};
            }
            T t;
            if constexpr (type_trait::is_specialization_of<T, std::vector>::value)
            {
                t.reserve(j.size());
            }
            for (const auto &item : j)
            {
                t.push_back(impl::from_json<std::decay_t<typename T::value_type>>(item));
            }
            return t;
        }
    }

    template <typename T>
    requires std::is_aggregate_v<T> && std::is_class_v<T>
        T from_json(std::string_view json_str)
    {
        nlohmann::json j = nlohmann::json::parse(json_str);
        T t{};
        boost::pfr::for_each_field(t, [&j]<typename TT>(TT &field, std::size_t)
                                   {
            if constexpr(type_trait::is_field<TT>::value){
                if constexpr(std::is_class_v<typename TT::Type> && !std::is_same_v<typename TT::Type, std::string>){
                    field = impl::from_json<typename TT::Type>(j.at(std::string{field.tag()}));
                }else{
                    j.at(std::string{field.tag()}).get_to(field.value);
                }
            } });
        return t;
    }

    template <type_trait::is_dynamic_container T>
    T from_json(std::string_view json_str)
    {
        nlohmann::json j = nlohmann::json::parse(json_str);
        if (!j.is_array())
        {
            return {};
        }
        T t;
        if constexpr (type_trait::is_specialization_of<T, std::vector>::value)
        {
            t.reserve(j.size());
        }
        for (const auto &item : j)
        {
            t.push_back(impl::from_json<std::decay_t<typename T::value_type>>(item));
        }
        return t;
    }

} // namespace kie::json

#endif