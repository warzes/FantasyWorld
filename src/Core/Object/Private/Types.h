#pragma once

namespace se::Private
{
	// C++ compiler extensions
	// MSC: http://msdn.microsoft.com/en-us/library/vstudio/ms177194.aspx
	// GCC: http://gcc.gnu.org/onlinedocs/gcc/Type-Traits.html
	// Clang : http://clang.llvm.org/docs/LanguageExtensions.html
	template <typename T> 
	struct TypeTraitsCppExt
	{
		constexpr static bool HasNothrowAssign() { return __has_nothrow_assign(T); }
		constexpr static bool HasNothrowCopy() { return __has_nothrow_copy(T); }
		constexpr static bool HasNothrowConstructor() { return __has_nothrow_constructor(T); }
		constexpr static bool HasTrivialAssign() { return __has_trivial_assign(T); }
		constexpr static bool HasTrivialCopy() { return __has_trivial_copy(T); }
		constexpr static bool HasTrivialConstructor() { return __has_trivial_constructor(T); }
		constexpr static bool HasTrivialDestructor() { return __has_trivial_destructor(T); }
		constexpr static bool HasVirtualDestructor() { return __has_virtual_destructor(T); }
		constexpr static bool IsAbstract() { return __is_abstract(T); }
		constexpr static bool IsClass() { return __is_class(T); }
		constexpr static bool IsEmpty() { return __is_empty(T); }
		constexpr static bool IsEnum() { return __is_enum(T); }
		constexpr static bool IsPod() { return __is_pod(T); }
		constexpr static bool IsPolymorphic() { return __is_polymorphic(T); }
		constexpr static bool IsUnion() { return __is_union(T); }
		template <typename U> constexpr static bool IsBaseOf() { return __is_base_of(U, T); } // __is_base_of(base, derived)
	};

	// type number
	template <int i> struct Number { enum { Value = i }; };
	// boolean type value (for SFINAE)
	using SETrue = Number<1>;
	using SEFalse = Number<0>;

	template <typename T, bool isPolymorphic = TypeTraitsCppExt<T>::IsPolymorphic()> struct BaseAddress;
	
	template <typename T> 
	struct BaseAddress<T, true>
	{
		enum { IsPolymorphic = 1 };
		static void* Cast(T* p)
		{
			return dynamic_cast<void*>(p);
		}
	};
	template <typename T> 
	struct BaseAddress<T, false>
	{
		enum { IsPolymorphic = 0 };
		static void* Cast(T* p)
		{
			return static_cast<void*>(p);
		}
	};

	// returns object's base address.
	// In multiple inheritance, base address may be differ.
	template <typename T> 
	void* TypeBaseAddressCast(T* p)
	{
		return Private::BaseAddress<T>::Cast(p);
	}

	// conditional type
	template <bool C, typename T, typename U> 
	struct SECondType
	{
		using Result = T;
	};
	template <typename T, typename U> 
	struct SECondType<false, T, U>
	{
		using Result = U;
	};

	// check T1 is convertible into T2.
	template <typename T1, typename T2> struct ConversionTest
	{
		template <typename U> struct _IsPointerOrReference { enum { Result = false }; };
		template <typename U> struct _IsPointerOrReference<U*> { enum { Result = true }; };
		template <typename U> struct _IsPointerOrReference<U&> { enum { Result = true }; };
		template <typename U> struct _IsPointerOrReference<U&&> { enum { Result = true }; };

		// Use given type if type is pointer or reference,
		// otherwise change into reference-type.
		using Type1Ref = typename SECondType<_IsPointerOrReference<T1>::Result, T1, T1&>::Result;
		using Type2Ref = typename SECondType<_IsPointerOrReference<T2>::Result, T2, const T2&>::Result;

		static SETrue _Test(Type2Ref);
		static SEFalse _Test(...);
		static Type1Ref _TypeSub();

		enum { Result = decltype(_Test(_TypeSub()))::Value };
	};
	template <typename T> struct ConversionTest<T, T> { enum { Result = true }; };
	template <typename T> struct ConversionTest<void, T> { enum { Result = false }; };
	template <typename T> struct ConversionTest<T, void> { enum { Result = false }; };
	template <> struct ConversionTest<void, void> { enum { Result = true }; };

	// conditional type definition, if C is true result is T, else U.
	template <bool C, typename T, typename U> using CondType = typename SECondType<C, T, U>::Result;

} // namespace se::Private