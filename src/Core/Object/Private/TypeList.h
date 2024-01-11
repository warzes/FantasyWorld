#pragma once

#include "Object/Private/Types.h"

namespace se::Private
{
	// based on book 'Modern C++ Design' by Andrei Alexandrescu.
	// This class is used by function traits and tuple
	template <typename... Types> 
	class TypeList
	{
		// typedef of type located at Index. (error if index goes out of range)
		template <int Index> struct _TypeAt
		{
			// decrease N by one, define type at N become to zero.
			template <int N, typename T, typename... Ts> struct _EnumTypes
			{
				using Result = typename _EnumTypes<N - 1, Ts...>::Result;
			};
			template <typename T, typename... Ts> struct _EnumTypes<0, T, Ts...>
			{
				using Result = T;
			};

			static_assert(Index < sizeof...(Types), "Index must be less than count");
			static_assert(Index >= 0, "Index cannot be negative value");

			using Result = typename _EnumTypes<Index, Types...>::Result;
		};
		// get index of type T. (error if not exists)
		template <typename T> struct _IndexOf
		{
			// increase Result till type T could be found.
			template <typename, typename...> struct _StepMatchingType;
			template <typename T1, typename T2, typename... Ts> struct _StepMatchingType<T1, T2, Ts...>
			{
				enum { Result = 1 + _StepMatchingType<T1, Ts...>::Result };
			};
			template <typename T1, typename... Ts> struct _StepMatchingType<T1, T1, Ts...>
			{
				enum { Result = 0 };
			};
			template <typename T1> struct _StepMatchingType<T1>
			{
				enum { Result = 0 };
			};

			enum { Result = _StepMatchingType<T, Types...>::Result };
			static_assert(Result < sizeof...(Types), "Type is not in list");
		};
		// get number of type T in list.
		template <typename T> struct _CountType
		{
			// iterating all types, increase Result when type is T.
			template <typename, typename...> struct _Count;
			template <typename T1, typename T2, typename... Ts> struct _Count<T1, T2, Ts...>
			{
				enum { Result = _Count<T1, Ts...>::Result };
			};
			template <typename T1, typename... Ts> struct _Count<T1, T1, Ts...>
			{
				enum { Result = 1 + _Count<T1, Ts...>::Result };
			};
			template <typename T1> struct _Count<T1>
			{
				enum { Result = 0 };
			};

			enum { Result = _Count<T, Types...>::Result };
		};

		// append Types into list.
		template <typename... Ts> struct _Append
		{
			using Result = TypeList<Types..., Ts...>;
		};
		// append all types in given list to list.
		template <typename... Ts> struct _Append<TypeList<Ts...>>
		{
			using Result = TypeList<Types..., Ts...>;
		};
		// determine two lists are equal.
		template <typename... Ts> struct _IsSame
		{
			enum { Result = _IsSame<TypeList<Ts...>>::Result };
		};
		template <typename... Ts> struct _IsSame<TypeList<Ts...>>
		{
			template <typename T1, typename T2> struct IsSameType { enum { Result = false }; };
			template <typename T> struct IsSameType<T, T> { enum { Result = true }; };

			// count until given types are not matched. (in range of S)
			template <int S, typename L1, typename L2> struct _CountSameTypes
			{
				enum { NextIndex = S - 1 };
				using Type1 = typename L1::template TypeAt<NextIndex>;
				using Type2 = typename L2::template TypeAt<NextIndex>;

				enum { Result = IsSameType<Type1, Type2>::Result ? _CountSameTypes<NextIndex, L1, L2>::Result + 1 : 0 };
			};
			template <typename L1, typename L2> struct _CountSameTypes<0, L1, L2>
			{
				enum { Result = 0 };
			};

			enum { LengthA = sizeof...(Types), LengthB = sizeof...(Ts) };
			enum { MinRange = LengthA < LengthB ? LengthA : LengthB };

			using _List1 = TypeList<Types...>;
			using _List2 = TypeList<Ts...>;

			enum { NumSameTypes = _CountSameTypes<MinRange, _List1, _List2>::Result };
			enum { Result = NumSameTypes == LengthA && NumSameTypes == LengthB };
		};
		// check types are convertible to given types.
		template <typename... Ts> struct _IsConvertible
		{
			enum { Result = _IsConvertible<TypeList<Ts...>>::Result };
		};
		template <typename... Ts> struct _IsConvertible<TypeList<Ts...>>
		{
			// count till given type(L1) is not convertible to type(L2). (in range of S)
			template <int S, typename L1, typename L2> struct _CountCompatibles
			{
				enum { NextIndex = S - 1 };
				using Type1 = typename L1::template TypeAt<NextIndex>;
				using Type2 = typename L2::template TypeAt<NextIndex>;

				enum { Result = Private::ConversionTest<Type1, Type2>::Result ? _CountCompatibles<NextIndex, L1, L2>::Result + 1 : 0 };
			};
			template <typename L1, typename L2> struct _CountCompatibles<0, L1, L2>
			{
				enum { Result = 0 };
			};

			enum { LengthA = sizeof...(Types), LengthB = sizeof...(Ts) };
			enum { MinRange = LengthA < LengthB ? LengthA : LengthB };

			using _List1 = TypeList<Types...>;
			using _List2 = TypeList<Ts...>;

			enum { NumCompatibles = _CountCompatibles<MinRange, _List1, _List2>::Result };
			enum { Result = NumCompatibles == LengthA && NumCompatibles == LengthB };
		};
		// remove matching types specified by Ts...
		template <typename... Ts> struct _Remove
		{
			using Result = TypeList<Types...>;
		};
		template <typename T, typename... Ts> struct _Remove<T, Ts...>
		{
			// re-construct list without T1.
			template <typename T1, typename... Ts2> struct _EraseOne
			{
				using Result = TypeList<>;
			};
			template <typename T1, typename... Ts2> struct _EraseOne<T1, T1, Ts2...>
			{
				using Result = typename _EraseOne<T1, Ts2...>::Result;
			};
			template <typename T1, typename T2, typename... Ts2> struct _EraseOne<T1, T2, Ts2...>
			{
				using _Rest = typename _EraseOne<T1, Ts2...>::Result;
				using Result = typename TypeList<T2>::template Append< _Rest >;
			};

			using _R = typename _EraseOne<T, Types...>::Result;
			using Result = typename _R::template Remove<Ts...>;
		};
		// re-construct list without specific type at given Index.
		template <int Index> struct _RemoveIndex
		{
			template <int N, typename... Ts> struct _RemoveAt
			{
				using Result = TypeList<>;
			};
			template <int N, typename T, typename... Ts> struct _RemoveAt<N, T, Ts...>
			{
				using _Rest = typename _RemoveAt<N + 1, Ts...>::Result;
				using Result = typename TypeList<T>::template Append<_Rest>;
			};
			template <typename T, typename... Ts> struct _RemoveAt<Index, T, Ts...>
			{
				using Result = typename _RemoveAt<Index + 1, Ts...>::Result;
			};

			static_assert(Index < sizeof...(Types), "Index must be less than count");
			static_assert(Index >= 0, "Index cannot be negative value");

			using Result = typename _RemoveAt<0, Types...>::Result;
		};
		// generate sub-list in range.
		template <int Begin, int End> struct _SubListT
		{
			static_assert(Begin < sizeof...(Types), "Index must be less than count");
			static_assert(End < sizeof...(Types), "Out of range");
			static_assert(Begin <= End, "Invalid range");

			static_assert(Begin >= 0, "Index cannot be negative value");
			static_assert(End >= 0, "Index cannot be negative value");

			template <int, typename...> struct _SubList;
			template <int Index, typename T, typename... Ts> struct _SubList<Index, T, Ts...>
			{
				using _Rest = typename _SubList<Index + 1, Ts...>::Result;
				using Result = CondType < Index < Begin, _Rest, typename TypeList<T>::template Append< _Rest > >;
			};
			template <typename T, typename... Ts> struct _SubList<End, T, Ts...>
			{
				using Result = TypeList<T>;
			};

			using Result = typename _SubList<0, Types...>::Result;
		};

		// insert specific type at given location Index.
		template <int Index, typename... Ts> struct _InsertTypesAt
		{
			static_assert(Index <= sizeof...(Types), "Index must be in range");
			static_assert(Index >= 0, "Index cannot be negative value");

			template <int, typename...> struct _InsertT;
			template <int N, typename T, typename... Ts2> struct _InsertT<N, T, Ts2...>
			{
				using Result = typename TypeList<T>::template Append< typename _InsertT<N + 1, Ts2...>::Result >;
			};
			template <typename T, typename... Ts2> struct _InsertT<Index, T, Ts2...>
			{
				using Result = TypeList<Ts..., T, Ts2...>;
			};
			template <typename... Ts2> struct _InsertT<Index, Ts2...>
			{
				using Result = TypeList<Ts..., Ts2...>;
			};

			using Result = typename _InsertT<0, Types...>::Result;
		};

		// generate reversed list.
		template <typename... Ts> struct _Reverse
		{
			using Result = TypeList<>;
		};
		template <typename T, typename... Ts> struct _Reverse<T, Ts...>
		{
			using _R = typename _Reverse<Ts...>::Result;
			using Result = typename _R::template Append<T>;
		};

	public:
		enum { Length = sizeof...(Types) }; ///< number of types in this class

		template <template <typename...> class T> using TypesInto = T<Types...>;

		// reversed list.
		using Reverse = typename _Reverse<Types...>::Result;

		// defines type at Index.
		template <int Index> using TypeAt = typename _TypeAt<Index>::Result;

		// get Index of specific type.
		template <typename T> constexpr static auto IndexOf() -> int
		{
			return _IndexOf<T>::Result;
		}

		// count number of given type T in list.
		template <typename T> constexpr static auto Count() -> int
		{
			return _CountType<T>::Result;
		}

		// append types into list.
		// @note
		//	If given arguments contains TypeList<...> with other types,
		//	the type list (TypeList<...>) will assumed one type.
		//	(With multiple arguments, type list will not be iterated for each elements.)
		template <typename... Ts> using Append = typename _Append<Ts...>::Result;

		// insert specific type at Index.
		// TypeList<...> assumed one type. If you need to insert types in other type list, you should use Append.
		template <int Index, typename... Ts> using InsertAt = typename _InsertTypesAt<Index, Ts...>::Result;

		// check lists are equal.
		template <typename... Ts> constexpr static auto IsSame() -> bool
		{
			return _IsSame<Ts...>::Result;
		}

		// check types in list can be convertible to others.
		template <typename... Ts> constexpr static auto IsConvertible() -> bool
		{
			return _IsConvertible<Ts...>::Result;
		}

		// remove specific types in list.
		template <typename... Ts> using Remove = typename _Remove<Ts...>::Result;

		// remove type at Index.
		template <int Index> using RemoveAt = typename _RemoveIndex<Index>::Result;

		// generate sub list with given range.
		template <int Begin, int End> using SubList = typename _SubListT<Begin, End>::Result;
	};
} // se::Private