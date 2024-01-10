#pragma once

#include "Object/Private/TypeList.h"

namespace se::Private
{
	// type traits of global function pointer.
	template <typename> struct FunctionPointerTraits
	{
		enum { IsFunctionPointer = false };
	};
	// type traits of global function pointer.
	template <typename R, typename... Args> struct FunctionPointerTraits<R(*)(Args...)>
	{
		enum { IsFunctionPointer = true };
		enum { HasVariadicParameter = false };
		using ReturnType = R;
		using ParameterTypeList = TypeList<Args...>;
		using GenericType = R(Args...);
		using PointerType = R(*)(Args...);
	};
	// type traits of global function pointer.
	template <typename R, typename... Args> struct FunctionPointerTraits<R(*)(Args..., ...)>
	{
		enum { IsFunctionPointer = true };
		enum { HasVariadicParameter = true };
		using ReturnType = R;
		using ParameterTypeList = TypeList<Args...>;
		using GenericType = R(Args..., ...);
		using PointerType = R(*)(Args..., ...);
	};
	// type traits of class member functions.
	template <typename> struct MemberFunctionPointerTraits
	{
		enum { IsFunctionPointer = false };
	};
	// type traits of class member functions.
	template <typename R, class C, typename... Args> struct MemberFunctionPointerTraits<R(C::*)(Args...)>
	{
		enum { IsFunctionPointer = true };
		enum { IsConst = false };
		enum { IsVolatile = false };
		enum { HasVariadicParameter = false };
		using ClassType = C;
		using ReturnType = R;
		using ParameterTypeList = TypeList<Args...>;
		using GenericType = R(Args...);
		using PointerType = R(ClassType::*)(Args...);
	};
	// type traits of class member functions.
	template <typename R, class C, typename... Args> struct MemberFunctionPointerTraits<R(C::*)(Args..., ...)>
	{
		enum { IsFunctionPointer = true };
		enum { IsConst = false };
		enum { IsVolatile = false };
		enum { HasVariadicParameter = true };
		using ClassType = C;
		using ReturnType = R;
		using ParameterTypeList = TypeList<Args...>;
		using GenericType = R(Args..., ...);
		using PointerType = R(ClassType::*)(Args..., ...);
	};
	// type traits of class member functions.
	template <typename R, class C, typename... Args> struct MemberFunctionPointerTraits<R(C::*)(Args...) const>
	{
		enum { IsFunctionPointer = true };
		enum { IsConst = true };
		enum { IsVolatile = false };
		enum { HasVariadicParameter = false };
		using ClassType = C;
		using ReturnType = R;
		using ParameterTypeList = TypeList<Args...>;
		using GenericType = R(Args...);
		using PointerType = R(ClassType::*)(Args...);
	};
	// type traits of class member functions.
	template <typename R, class C, typename... Args> struct MemberFunctionPointerTraits<R(C::*)(Args..., ...) const>
	{
		enum { IsFunctionPointer = true };
		enum { IsConst = true };
		enum { IsVolatile = false };
		enum { HasVariadicParameter = true };
		using ClassType = C;
		using ReturnType = R;
		using ParameterTypeList = TypeList<Args...>;
		using GenericType = R(Args..., ...);
		using PointerType = R(ClassType::*)(Args..., ...);
	};
	// type traits of class member functions.
	template <typename R, class C, typename... Args> struct MemberFunctionPointerTraits<R(C::*)(Args...) volatile>
	{
		enum { IsFunctionPointer = true };
		enum { IsConst = false };
		enum { IsVolatile = true };
		enum { HasVariadicParameter = false };
		using ClassType = C;
		using ReturnType = R;
		using ParameterTypeList = TypeList<Args...>;
		using GenericType = R(Args...);
		using PointerType = R(ClassType::*)(Args...);
	};
	/// type traits of class member functions.
	template <typename R, class C, typename... Args> struct MemberFunctionPointerTraits<R(C::*)(Args..., ...) volatile>
	{
		enum { IsFunctionPointer = true };
		enum { IsConst = false };
		enum { IsVolatile = true };
		enum { HasVariadicParameter = true };
		using ClassType = C;
		using ReturnType = R;
		using ParameterTypeList = TypeList<Args...>;
		using GenericType = R(Args..., ...);
		using PointerType = R(ClassType::*)(Args..., ...);
	};
	/// type traits of class member functions.
	template <typename R, class C, typename... Args> struct MemberFunctionPointerTraits<R(C::*)(Args...) const volatile>
	{
		enum { IsFunctionPointer = true };
		enum { IsConst = true };
		enum { IsVolatile = true };
		enum { HasVariadicParameter = false };
		using ClassType = C;
		using ReturnType = R;
		using ParameterTypeList = TypeList<Args...>;
		using GenericType = R(Args...);
		using PointerType = R(ClassType::*)(Args...);
	};
	/// type traits of class member functions.
	template <typename R, class C, typename... Args> struct MemberFunctionPointerTraits<R(C::*)(Args..., ...) const volatile>
	{
		enum { IsFunctionPointer = true };
		enum { IsConst = true };
		enum { IsVolatile = true };
		enum { HasVariadicParameter = true };
		using ClassType = C;
		using ReturnType = R;
		using ParameterTypeList = TypeList<Args...>;
		using GenericType = R(Args..., ...);
		using PointerType = R(ClassType::*)(Args..., ...);
	};
	// basic type traits
	template <typename T> 
	class TypeTraits : public TypeTraitsCppExt<T>
	{
		template <typename U> struct ReferenceTraits
		{
			enum { Result = false };
			enum { IsLValueRef = false };
			enum { IsRValueRef = false };
			using ReferredType = U;
		};
		template <typename U> struct ReferenceTraits<U&>
		{
			enum { Result = true };
			enum { IsLValueRef = true };
			enum { IsRValueRef = false };
			using ReferredType = U;
		};
		template <typename U> struct ReferenceTraits<U&&>
		{
			enum { Result = true };
			enum { IsLValueRef = false };
			enum { IsRValueRef = true };
			using ReferredType = U;
		};
		template <typename U> struct PointerTraits
		{
			enum { Result = false };
			using PointeeType = U;
		};
		template <typename U> struct PointerTraits<U*>
		{
			enum { Result = true };
			using PointeeType = U;
		};
		template <typename U> struct PointerTraits<U*&>
		{
			enum { Result = true };
			using PointeeType = U;
		};
		template <typename U> struct PointerTraits<U*&&>
		{
			enum { Result = true };
			using PointeeType = U;
		};
		template <typename U> struct MemberPointerTraits
		{
			enum { Result = false };
		};
		template <typename U, typename V> struct MemberPointerTraits<U V::*>
		{
			enum { Result = true };
		};
		template <typename U, typename V> struct MemberPointerTraits<U V::*&>
		{
			enum { Result = true };
		};
		template <typename U, typename V> struct MemberPointerTraits<U V::*&&>
		{
			enum { Result = true };
		};
		template <typename U> struct UnConst
		{
			enum { IsConst = false };
			using Type = U;
		};
		template <typename U> struct UnConst<const U>
		{
			enum { IsConst = true };
			using Type = U;
		};
		template <typename U> struct UnConst<const U&>
		{
			enum { IsConst = true };
			using Type = U&;
		};
		template <typename U> struct UnConst<const U&&>
		{
			enum { IsConst = true };
			using Type = U&&;
		};
		template <typename U> struct UnVolatile
		{
			enum { IsVolatile = false };
			using Type = U;
		};
		template <typename U> struct UnVolatile<volatile U>
		{
			enum { IsVolatile = true };
			using Type = U;
		};
		template <typename U> struct UnVolatile<volatile U&>
		{
			enum { IsVolatile = true };
			using Type = U&;
		};
		template <typename U> struct UnVolatile<volatile U&&>
		{
			enum { IsVolatile = true };
			using Type = U&&;
		};

		template <typename U> struct AddPointer { using Type = U*; };
		template <typename U> struct AddPointer<U&> { using Type = U*; };
		template <typename U> struct AddPointer<U&&> { using Type = U*; };
		template <typename U> struct AddReference { using Type = U&; };
		template <typename U> struct AddReference<U&> { using Type = U&; };
		template <typename U> struct AddReference<U&&> { using Type = U&&; };

	public:
		using NonConstType = typename UnConst<T>::Type;
		using NonVolatileType = typename UnVolatile<T>::Type;
		using UnqualifiedType = typename UnVolatile<typename UnConst<T>::Type>::Type;
		using PointeeType = typename PointerTraits<UnqualifiedType>::PointeeType;
		using ReferredType = typename ReferenceTraits<T>::ReferredType;
		using UnqualifiedReferredType = typename ReferenceTraits<UnqualifiedType>::ReferredType;

		using FunctionTraits = FunctionPointerTraits<UnqualifiedReferredType>;
		using MemberFunctionTraits = MemberFunctionPointerTraits<UnqualifiedReferredType>;

		enum { IsConst = UnConst<T>::IsConst };
		enum { IsVolatile = UnVolatile<T>::IsVolatile };
		enum { IsReference = ReferenceTraits<UnqualifiedType>::Result };
		enum { IsLValueReference = ReferenceTraits<UnqualifiedType>::IsLValueRef };
		enum { IsRValueReference = ReferenceTraits<UnqualifiedType>::IsRValueRef };
		enum { IsFunction = FunctionPointerTraits<typename AddPointer<T>::Type>::IsFunctionPointer };
		enum { IsFunctionPointer = FunctionTraits::IsFunctionPointer };
		enum { IsMemberFunctionPointer = MemberFunctionTraits::IsFunctionPointer };
		enum { IsMemberPointer = MemberPointerTraits<UnqualifiedReferredType>::Result || IsMemberFunctionPointer };
		enum { IsPointer = PointerTraits<UnqualifiedReferredType>::Result || IsFunctionPointer };
	};
} // namespace se::Private