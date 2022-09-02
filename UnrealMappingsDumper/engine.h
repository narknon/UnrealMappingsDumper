#pragma once

#include "uobjectDependency.h"
#include "unrealEnums.h"
#include "unrealContainers.h"

typedef int FThreadSafeCounter;

template <typename UObjectBase = UObjectDependency>
class DefaultEngine
{
public:

	class UObject
	{
		void* Vtbl;
		EObjectFlags ObjectFlags;
		int32_t InternalIndex;
		class UClass* ClassPrivate;
		UObjectBase::FName NamePrivate;
		UObject* OuterPrivate;

	public:

		FORCEINLINE UClass* Class()
		{
			return ClassPrivate;
		}
	};

	class UField
	{
		UField* Next;

	public:

		FORCEINLINE UField* GetNext()
		{
			return Next;
		}
	};

	class FFieldClass
	{
		UObjectBase::FName Name;
		uint64_t Id;
		uint64_t CastFlags;
		EClassFlags ClassFlags;
		FFieldClass* SuperClass;
		class FField* DefaultObject;
		class FField* (*ConstructFn)(class FFieldVariant&, UObjectBase::FName&, EObjectFlags);
		FThreadSafeCounter UnqiueNameIndexCounter;

	public:

		inline std::string GetName() const
		{
			return Name.ToString();
		}

		inline UObjectBase::FName GetFName() const
		{
			return Name;
		}

		inline uint64_t GetId() const
		{
			return Id;
		}

		inline uint64_t GetCastFlags() const
		{
			return CastFlags;
		}

		inline bool HasAnyCastFlags(const uint64_t InCastFlags) const
		{
			return !!(CastFlags & InCastFlags);
		}

		inline bool HasAllCastFlags(const uint64_t InCastFlags) const
		{
			return (CastFlags & InCastFlags) == InCastFlags;
		}
	};

	class FFieldVariant
	{
		union FFieldObjectUnion
		{
			class FField* Field;
			UObject* Object;
		}Container;

		bool bIsUObject;
	};

	class FField
	{
		void* Vtbl;
		FFieldClass* ClassPrivate;
		FFieldVariant Owner;
		FField* Next;
		UObjectBase::FName NamePrivate;
		EObjectFlags FlagsPrivate;

	public:

		FORCEINLINE FField* GetNext()
		{
			return Next;
		}

		FORCEINLINE FFieldClass* GetClass() const
		{
			return ClassPrivate;
		}

		FORCEINLINE EObjectFlags GetFlags() const
		{
			return FlagsPrivate;
		}

		FORCEINLINE uint64_t GetCastFlags() const
		{
			return GetClass()->GetCastFlags();
		}
	};

	class FProperty : public FField
	{
		int32_t	ArrayDim;
		int32_t	ElementSize;
		EPropertyFlags PropertyFlags;
		uint16_t RepIndex;
		TEnumAsByte<ELifetimeCondition> BlueprintReplicationCondition;
		int32_t Offset_Internal;

	public:

		UObjectBase::FName RepNotifyFunc;
		FProperty* PropertyLinkNext;
		FProperty* NextRef;
		FProperty* DestructorLinkNext;
		FProperty* PostConstructLinkNext;
	};

	class UStruct : public UField
	{
	private:

		UStruct* SuperStruct;

	public:

		UField* Children;
		FField* ChildProperties;
		int32_t PropertiesSize;
		int32_t MinAlignment;
		TArray<uint8_t> Script;
		FProperty* PropertyLink;
		FProperty* RefLink;
		FProperty* DestructorLink;
		FProperty* PostConstructLink;
		TArray<UObject*> ScriptAndPropertyObjectReferences;

		FORCEINLINE UStruct* Super()
		{
			return SuperStruct;
		}
	};

	class UClass : public UStruct
	{
		typedef void (*ClassConstructorType)(const class FObjectInitializer&);
		typedef UObject* (*ClassVTableHelperCtorCallerType)(class FVTableHelper& Helper);
		typedef UClass* (*StaticClassFunctionType)();

	public:

		ClassConstructorType ClassConstructor;
		ClassVTableHelperCtorCallerType ClassVTableHelperCtorCaller;
		void* CppClassStaticFunctions;
		mutable int32_t ClassUnique;
		int32_t FirstOwnedClassRep = 0;
		bool bCooked;
		bool bLayoutChanging;
		EClassFlags ClassFlags;
		EClassCastFlags ClassCastFlags;
		UClass* ClassWithin;
		UObjectBase::FName ClassConfigName;
		TUndefinedArray ClassReps;
		TArray<UField*> NetFields;
	};
};

class Engine_UE5 : public DefaultEngine<>
{
public:

};

class Engine_Fortnite : public DefaultEngine<FortniteUObjectBase>
{
public:

};