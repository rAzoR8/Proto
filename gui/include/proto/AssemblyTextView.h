#pragma once

// forward delcs

namespace spvgentwo
{
	class Module;
}

namespace proto
{
	class AssemblyTextView
	{
	public:
		AssemblyTextView();
		~AssemblyTextView();

		void update(spvgentwo::Module& _module);
	private:
	};
} // !proto