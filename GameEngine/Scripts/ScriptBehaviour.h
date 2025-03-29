#pragma once

class ScriptBehaviour {
public:
	virtual ~ScriptBehaviour() = default;

	virtual void Run() = 0;
};