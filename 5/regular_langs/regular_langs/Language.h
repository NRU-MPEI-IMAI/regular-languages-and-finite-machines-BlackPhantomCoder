#pragma once
#include <unordered_map>
#include <string>
#include <unordered_set>
#include <iostream>

//класс языка (в форме ДКА)
class Language
{
public:
	using zetta_type = std::unordered_set<char>;
	using T_type =	   std::unordered_set<std::string>;
	using Q_type =	   std::unordered_set<std::string>;
	using delta_type = std::unordered_map<std::string, std::unordered_map<char, std::string>>;
	using S_type = std::string;
public:
	Language() = default;

	const zetta_type&	get_zetta()const;
	const T_type&		get_T()const;
	const Q_type&		get_Q()const;
	const delta_type&	get_delta()const;
	const S_type& get_S()const;

	void set_zetta(const zetta_type& rh);
	void set_T(const T_type& rh);
	void set_Q(const Q_type& rh);
	void set_delta(const delta_type& rh);
	void set_S(const S_type& rh);

	void set_zetta(zetta_type&& rh);
	void set_T(T_type&& rh);
	void set_Q(Q_type&& rh);
	void set_delta(delta_type&& rh);
	void set_S(S_type&& rh);
private:
	zetta_type t_zetta;
	T_type t_T;
	Q_type t_Q;
	delta_type t_delta;
	S_type t_S;
};

//пересечение
Language make_union(const Language& lh, const Language& rh);
//объединение
Language make_intersection(const Language& lh, const Language& rh);
//дополнение
Language make_complement(const Language& rh);
//разность
Language make_difference(const Language& lh, const Language& rh);
//объединение из пересечения
Language make_intersection_from_union(const Language& lh, const Language& rh, Language u);

std::istream& operator>>(std::istream& is, Language& rh);
std::ostream& operator<<(std::ostream& os, const Language& rh);