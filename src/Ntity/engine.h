#pragma once

namespace Ntity
{
	class Engine
	{
	public:
		Engine();

		virtual void Run();
	};

	Engine* Create();
}
