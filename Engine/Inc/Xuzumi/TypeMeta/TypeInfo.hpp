#pragma once

#include "Xuzumi/TypeMeta/TypeID.hpp"
#include "Xuzumi/TypeMeta/TypeName.hpp"
#include "Xuzumi/Core/Templates/SizeOf.hpp"
#include "Xuzumi/Core/Templates/AlignOf.hpp"

namespace Xuzumi
{
	/**
	 * @brief Type used to capture useful type characteristics including the type
	 * identifier.
	 * 
	 * Extensively used for type introspection.
	 */
	struct TypeInfo
	{
		// TODO: replace public fields with getters.

		/**
		 * @brief A type identifier.
		 */
		TypeID ID = kInvalidTypeID;

		/**
		 * @brief A type name.
		 */
		std::string Name;

		/**
		 * @brief The size of the type.
		 */
		std::size_t Size = SizeOfV<void>;

		/**
		 * @brief The alignment of the type.
		 */
		std::size_t Alignment = AlignOfV<void>;

		/**
		 * @brief Retrieves `TypeInfo` instance for @p T.
		 *
		 * @tparam T The type which type information is to be retrieved.
		 * 
		 * @return A `TypeInfo` instance containing @p T type information.
		 */
		template<typename T>
		static TypeInfo Get()
		{
			// TODO: add type constraints for the `T` type.
			TypeInfo instance;
			instance.ID = GetTypeID<T>();
			instance.Name = GetTypeName<T>();
			instance.Size = SizeOfV<T>;
			instance.Alignment = AlignOfV<T>;
			instance.mDestructor = [](void* object)
				{
					reinterpret_cast<T*>(object)->~T();
				};

			return instance;
		}

		/**
		 * @brief Converts `*this` into human-readable format.
		 *
		 * @return An `std::string` instance containing a string representation of
		 * `*this`.
		 */
		std::string ToString() const;

		void Destruct(void* object) const;

		/**
		 * @brief Checks whether `*this` references a valid type.
		 *
		 * @retval true `*this` contains valid type identifier.
		 * @retval false `*this` contains invalid type identifier.
		 */
		explicit operator bool() const;

		/**
		 * @brief Checks whether `*this` and @p other refer to the same type.
		 *
		 * @retval true @p other refers to the same type as `*this`.
		 * @retval false @p other and `*this` refer to different types.
		 */
		bool operator==(const TypeInfo& other) const;
		
		/**
		 * @brief Checks whether `*this` and @p other refer to distinct types.
		 *
		 * @retval true @p other and `*this` refer to different types.
		 * @retval false @p other refers to the same type as `*this`.
		 */
		bool operator!=(const TypeInfo& other) const;

	private:
		void(*mDestructor)(void* object) = nullptr;
	};	
}
