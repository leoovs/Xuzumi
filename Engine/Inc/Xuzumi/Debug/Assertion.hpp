/**
 * @file Xuzumi/Debug/Assertion.hpp
 * 
 * @brief Defines classes and structures related to assertion handling.
 */

#pragma once

#include "Xuzumi/Precompiled.hpp"
#include "Xuzumi/Core/Delegate.hpp"
#include "Xuzumi/Memory/ObserverPtr.hpp"

namespace Xuzumi
{
	/**
	 * @brief Structure containing assertion metadata. 
	 */
	struct AssertionData
	{
		/**
		 * @brief The name of the source file where an assertion occurred.
		 */
		std::string SourceFileName;
		
		/**
		 * @brief The line number in the source file where an assertion occurred.
		 */
		std::uint32_t Line = -1;

		/**
		 * @brief A descriptive message associated with an assertion.
		 * 
		 * Should provide a context information that would help understand why an
		 * error occurred.
		 */
		std::string Message;
		
		/**
		 * @brief A string representation of an expression that evaluated to false
		 * in an assertion. 
		 */
		std::string FalseExpression;

		/**
		 * @brief The name of the function or method scope where an assertion
		 * occurred. 
		 */
		std::string ScopeName;

		/**
		 * @brief Default constructor.
		 * 
		 * Constructs an `AssertionData` object that contains meaningless assertion
		 * metadata. 
		 */
		AssertionData() = default;

		/**
		 * Constructs an `AssertionData` object with provided information.
		 * 
		 * @param sourceFileName The name of the source file where an assertion
		 * occurred.
		 * @param line The line number in the source file where an assertion
		 * occurred.
		 * @param message A descriptive message providing context information for
		 * the error. 
		 * @param falseExpression An expression that evaluated to false.
		 * @param scopeName The name of the function or method scope.
		 */
		AssertionData(
			std::string_view sourceFileName,
			std::uint32_t line,
			std::string_view message,
			std::string_view falseExpression,
			std::string_view scopeName
		)
			: SourceFileName(sourceFileName)
			, Line(line)
			, Message(message)
			, FalseExpression(falseExpression)
			, ScopeName(scopeName)
		{
		}
	};

	/**
	 * @typedef AssertionHandler
	 * 
	 * @brief Alias for the delegate type used for handling assertions. 
	 */
	using AssertionHandler = Delegate<void(const AssertionData&)>;

	/**
	 * @brief Dispatches and handles assertions.
	 * 
	 * This class is a key component of the assertion system. It manages handlers
	 * and raises assertions. It can handle cyclic-assertion cases, when an
	 * assertion occurs in a custom assertion handler itself. In such cases, all
	 * `AssertionData` instances are pushed to the stack, and a fallback to the
	 * default assertion handler occurs. The default handler outputs all the
	 * information to the `stderr` (`std::cerr`) standard stream, clearing the
	 * stack.
	 * Typically, users should not access objects of this class directly. If any
	 * modification to how an assertion is handled required, then
	 * `AssertionConfigurator` must be used instead.
	 * 
	 * @see Xuzumi::AssertionConfigurator
	 * @see Xuzumi::DebugSession::ConfigureAssertion
	 */
	class AssertionDispatcher
	{
	public:
		/**
		 * @brief Default constructor.
		 * 
		 * Constructs an `AssertionDispatcher` object with default handler and no
		 * metadata. 
		 */
		AssertionDispatcher();

		/**
		 * @brief Prevents unintended copying of an `AssertionDispatcher` object. 
		 */
		AssertionDispatcher(const AssertionDispatcher&) = delete;

		/**
		 * @brief Raises an assertion with the provided `AssertionData`.
		 * 
		 * Pushes @p data to the stack and invokes an assertion handler.
		 * 
		 * @param data An `AssertionData` object containing the information about
		 * the assertion.
		 */
		void Raise(AssertionData data);

		/**
		 * @brief Sets a new assertion handler.
		 * 
		 * If @p handler is empty, the default assertion handler is set.
		 * 
		 * @param handler A custom assertion handler.
		 */
		void SetHandler(AssertionHandler handler);

	private:
		void DefaultAssertionHandler(const AssertionData& data);
		void FallbackToDefaultHandler();

		AssertionHandler mHandler;
		std::stack<AssertionData> mAssertionStack;
	};

	/**
	 * @brief Provides a convenient interface for configuring assertions.
	 * 
	 * This class provides a clearer interface for assertion modification as well
	 * as removes necessity for some boilerplate code.
	 */
	class AssertionConfigurator
	{
	public:
	 	/**
		 * @brief Constructs an `AssertionConfigurator` object that references an
		 * `AssertionDispatcher` object.
		 * 
		 * Internally, the `AssertionDispatcher` object is referenced via a raw
		 * pointer. For that reason, it should not be constructed directly by user,
		 * neither used globally. Usage of this class's instances should be brought
		 * to as small scope as possible. The most convenient way to access this
		 * class's functionality is via the `DebugSession` class.
		 * 
		 * @param configuredDispatcher A pointer to an `AssertionDispatcher` object
		 * that is to be configured.
		 * 
		 * @see Xuzumi::DebugSession::ConfigureAssertion
		 */
		explicit AssertionConfigurator(
			ObserverPtr<AssertionDispatcher> configuredDispatcher
		);

		/**
		 * @brief Sets a new assertion handler for the configured dispatcher.
		 * 
		 * @param handler A custom assertion handler. 
		 */
		void SetHandler(AssertionHandler handler);

		/**
		 * @brief Bind logging system to display assertion failures.
		 * 
		 * Encapsulates boilerplate code that sets up logging system to display
		 * assertion information.
		 */
		void SetLoggerAsHandler();

	private:
		ObserverPtr<AssertionDispatcher> mConfiguredDispatcher;
	};
}
