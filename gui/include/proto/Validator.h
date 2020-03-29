#pragma once

namespace spvgentwo
{
	template <typename T>
	class Vector;
}

namespace proto
{
	class Validator
	{
	public:
		Validator();
		~Validator();

		bool validate(const spvgentwo::Vector<unsigned int>& _module);
	private:

	};
} // !proto