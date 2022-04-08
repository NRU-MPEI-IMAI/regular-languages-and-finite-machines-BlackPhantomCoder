#include "Language.h"
#include <algorithm>
#include <sstream>
#include <optional>

using namespace std;

const Language::zetta_type& Language::get_zetta() const
{
    return t_zetta;
}

const Language::T_type& Language::get_T() const
{
    return t_T;
}

const Language::Q_type& Language::get_Q() const
{
    return t_Q;
}

const Language::delta_type& Language::get_delta() const
{
    return t_delta;
}

const Language::S_type& Language::get_S() const
{
    return t_S;
}

void Language::set_zetta(const zetta_type& rh)
{
    t_zetta = rh;
}

void Language::set_T(const T_type& rh)
{
    t_T = rh;
}

void Language::set_Q(const Q_type& rh)
{
    t_Q = rh;
}

void Language::set_delta(delta_type&& rh)
{
    t_delta = move(rh);
}

void Language::set_S(S_type&& rh)
{
    t_S = move(rh);
}

void Language::set_zetta(zetta_type&& rh)
{
    t_zetta = move(rh);
}

void Language::set_T(T_type&& rh)
{
    t_T = move(rh);
}

void Language::set_Q(Q_type&& rh)
{
    t_Q = move(rh);
}

void Language::set_delta(const delta_type& rh)
{
    t_delta = rh;
}

void Language::set_S(const S_type& rh)
{
    t_S = rh;
}


string make_vtuple(const string& lh, const string& rh) {
    return "<"s + lh + "," + rh + ">";
}


std::unordered_set<std::string> make_dec(const std::unordered_set<std::string>& lh, const std::unordered_set<std::string>& rh) {
    auto result = std::unordered_set<std::string>();
    for (const auto& q : lh) {
        for (const auto& s : rh) {
            result.insert(make_vtuple(q, s));
        }
    }
    return result;
}

Language make_pattern(const Language& lh, const Language& rh) {
    auto result = Language();

    auto zetta = lh.get_zetta();
    zetta.insert(begin(rh.get_zetta()), end(rh.get_zetta()));
    result.set_zetta(move(zetta));

    result.set_S(make_vtuple(lh.get_S(), rh.get_S()));

    result.set_Q(make_dec(lh.get_Q(), rh.get_Q()));

    auto delta = Language::delta_type();

    const auto& q_d = lh.get_delta();
    const auto& q_s = rh.get_delta();
    for (const auto& ch : result.get_zetta()) {
        for (const auto& q : lh.get_Q()) {
            if (auto it_q = q_d.at(q).find(ch); it_q != end(q_d.at(q))){
                for (const auto& s : rh.get_Q()) {
                    if (auto it_s = q_s.at(s).find(ch); it_s != end(q_s.at(s))){
                        delta[make_vtuple(q, s)][ch] = make_vtuple(it_q->second, it_s->second);
                    }
                }
            }
        }
    }

    result.set_delta(move(delta));

    return result;
}

Language make_union(const Language& lh, const Language& rh)
{
    auto result = make_pattern(lh, rh);
    result.set_T(make_dec(lh.get_T(), rh.get_T()));
    return result;
}

Language make_intersection(const Language& lh, const Language& rh)
{
    auto result = make_pattern(lh, rh);

    auto l_T = make_dec(lh.get_T(), rh.get_Q());
    auto r_T = make_dec(lh.get_Q(), rh.get_T());

    l_T.insert(begin(r_T), end(r_T));
    result.set_T(l_T);

    return result;
}

Language make_complement(const Language& rh)
{
    auto result = rh;

    auto buf = rh.get_Q();
    for (const auto& q : rh.get_T()) {
        buf.erase(q);
    }
    result.set_T(move(buf));

    return result;
}

Language make_difference(const Language& lh, const Language& rh)
{
    return make_union(lh, make_complement(rh));
}

Language make_intersection_from_union(const Language& lh, const Language& rh, Language u)
{
    auto l_T = make_dec(lh.get_T(), rh.get_Q());
    auto r_T = make_dec(lh.get_Q(), rh.get_T());

    l_T.insert(begin(r_T), end(r_T));
    u.set_T(l_T);

    return u;
}

pair<string, string::const_iterator>  read_next(string::const_iterator left, string::const_iterator e) {
    auto result = ""s;
    auto right = find(left, e, '|');
    if(right == e)  
        return { string(left, right), e };
    while (right != e) {
        if (*prev(right) != '\\') {
            result += string(left, right);
            break;
        }
        else {
            result += string(left, prev(right));
            result += "|";
            right = find(left+1, e, '|');
        }
    }
    return { move(result), (right==e) ? e : right+1 };
}


std::istream& operator>>(std::istream& is, Language& rh)
{
    if (is) {
        auto s = ""s;
        getline(is, s);

        auto zetta = vector<char>();
        for (const auto& ch : s) {
            zetta.push_back(ch);
        }


        auto delta = Language::delta_type();
        auto Q = Language::Q_type();


        while (getline(is, s)) {
            if (empty(s)) break;

            auto buf = ""s;

            auto zetta_it = begin(zetta);
            auto q = ""s;

            auto [name, it] = read_next(begin(s), end(s));
            if (empty(name))
                throw invalid_argument("входная вершина функции перехода не должна быть пустой");
            else
                q = move(name);

            Q.insert(q);

            while (it != end(s)) {
                tie(name, it) = read_next(it, end(s));
                if (zetta_it == end(zetta)) {
                    throw invalid_argument("переход по неизветсной букве");
                }
                if (!empty(name)) {
                    delta[q][*zetta_it] = name;
                }
                ++zetta_it;
            }
        }
        rh.set_Q(move(Q));
        rh.set_delta(move(delta));

        auto T = Language::T_type();
        while (getline(is, s)) {
            if (empty(s)) break;
            if (rh.get_Q().find(s) == end(rh.get_Q())) {
                throw invalid_argument("конечная вершина должна быть описана в функции перехода");
            }
            T.insert(move(s));
        }
        rh.set_T(move(T));

        if (getline(is, s)) {
            if (rh.get_Q().find(s) == end(rh.get_Q())) {
                throw invalid_argument("cnfhnjdfz вершина должна быть описана в функции перехода");
            }
            rh.set_S(move(s));
        }
        else {
            throw invalid_argument("стартовая вершина не должна быть пустой");
        }

        auto szetta = Language::zetta_type();
        copy(begin(zetta), end(zetta), inserter(szetta, begin(szetta)));
        rh.set_zetta(move(szetta));
    }
    return is;
}

std::ostream& operator<<(std::ostream& os, const Language& rh)
{
    if (os) {
        os << "digraph name {" << '\n';
        os << "node [shape=circle]" << '\n';


        for (const auto& q : rh.get_Q()) {
                
            if (rh.get_T().find(q) != end(rh.get_T())) {
                os << "\"" << q << "\"" << "[shape = doublecircle]" << '\n';
            }
            else {
                os << "\"" << q << "\"" << '\n';
            }
        }

        os << "start[label = \"\", shape = none]" << '\n';
        os << "start -> \"" << rh.get_S() << "\"" << '\n';

            
        for (const auto& [q, l_s] : rh.get_delta()) {
            auto buf = unordered_map<string, string>();
            for (const auto& [l, s] : l_s) {
                auto& str = buf[s];
                if (empty(str))
                    str = ""s + l;
                else
                    str += ","s + l;
            }
            for (const auto& [s, ls] : buf) {
                os << "\"" << q << "\"" << "->" << "\"" << s << "\"" << "[label=\"" << ls << "\"" << "]" << '\n';
            }
        }

        os << "}" << flush;
    }
    return os;
}
