#pragma once

class CHandle
{
public:
	uint32_t Data{ 0 };

public:
	size_t GetEntityListIndex() const;
	size_t GetEntityEntryIndex() const;
	bool IsValid() const;
};