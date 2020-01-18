#pragma once

#include "proto/Window.h"

namespace proto
{
	class ProtoWindow : public Window
	{
	public:
		using Window::Window;

		//ProtoWindow();
		~ProtoWindow();

		bool updateUI() final;

	private:

	};

} // !proto