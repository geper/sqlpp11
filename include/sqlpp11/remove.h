/*
 * Copyright (c) 2013-2014, Roland Bock
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 *   Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 * 
 *   Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef SQLPP_REMOVE_H
#define SQLPP_REMOVE_H

#include <sqlpp11/statement.h>
#include <sqlpp11/type_traits.h>
#include <sqlpp11/parameter_list.h>
#include <sqlpp11/prepared_remove.h>
#include <sqlpp11/noop.h>
#include <sqlpp11/from.h>
#include <sqlpp11/extra_tables.h>
#include <sqlpp11/using.h>
#include <sqlpp11/where.h>

namespace sqlpp
{
	struct remove_name_t {};
	struct remove_t: public statement_name_t<remove_name_t>
	{
		using _traits = make_traits<no_value_t, tag::return_value>;
		struct _name_t {};

		template<typename Policies>
			struct _result_methods_t
			{
				using _statement_t = typename Policies::_statement_t;

				const _statement_t& _get_statement() const
				{
					return static_cast<const _statement_t&>(*this);
				}

				template<typename Db>
					auto _run(Db& db) const -> decltype(db.remove(this->_get_statement()))
					{
						_statement_t::_check_consistency();

						static_assert(_statement_t::_get_static_no_of_parameters() == 0, "cannot run remove directly with parameters, use prepare instead");
						return db.remove(_get_statement());
					}

					 template<typename Db>
					 auto _prepare(Db& db) const
					 -> prepared_remove_t<Db, _statement_t>
					 {
						 _statement_t::_check_consistency();

						 return {{}, db.prepare_remove(_get_statement())};
					 }
			};
	};


	template<typename Context>
		struct serializer_t<Context, remove_name_t>
		{
			using T = remove_name_t;

			static Context& _(const T& t, Context& context)
			{
				context << "DELETE";

				return context;
			}
		};

	template<typename Database>
		using blank_remove_t = statement_t<Database,
					remove_t,
					no_from_t,
					no_using_t,
					no_extra_tables_t,
					no_where_t<true>
						>;

	auto remove()
		-> blank_remove_t<void>
		{
			return { blank_remove_t<void>() };
		}

	template<typename Table>
		auto remove_from(Table table)
		-> decltype(blank_remove_t<void>().from(table))
		{
			return { blank_remove_t<void>().from(table) };
		}

	template<typename Database>
		auto  dynamic_remove(const Database&)
		-> decltype(blank_remove_t<Database>())
		{
			return { blank_remove_t<Database>() };
		}

	template<typename Database, typename Table>
		auto  dynamic_remove_from(const Database&, Table table)
		-> decltype(blank_remove_t<Database>().from(table))
		{
			return { blank_remove_t<Database>().from(table) };
		}
}

#endif
